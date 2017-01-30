/**
 * @file hw_flash.c
 * @author Murray Lang
 * @brief Routines for reading and writing to flash memory
 *
 * The code in this file has two major roles to perform.
 * The first role is to ensure that byte alignments are not violated. This is 
 * achieved by using a buffer with the same size as the alignment boundary to
 * marshal bytes that are addressed outside of the alignment boundaries.
 * 
 * The second role is to cycle through the available flash space to minimise
 * the number of writes and erases in any particular area and maximise the
 * useful life of the flash memory. 
 * 
 * This is achieved by employing something similar to a circular buffer. At the
 * start of the flash area there are two sets of flags: One set indicating the 
 * amount that has been written so far, and the other indicating what is "old"
 * (i.e. of no interest). 
 * 
 * Conceptually these are counters, but they cannot simply be numbers because 
 * then they would need to be erased and rewritten every time an update was 
 * performed. Apart from negating the whole idea of minimising erase/write 
 * cycles, this wouldn't work because erasing of flash memory usually involves
 * a minimum "segment" size that would erase much more than the counters.
 *
 * So the values are interpreted as flags, each indicating a "block" of flash
 * memory. When flash is erased, all bits are 1. When it is written, any 1s
 * in the data simply result in the corresponding bit in flash being untouched.
 * Any 0s in the data cause the corresponding bit in flash to be written to 0.
 * This means that an area can be rewritten without intervening erases so long
 * as the data is interpreted as an increasing number of zeros. 
 * 
 * As each "block" is sequentially written to, the next most significant bit in
 * the "written' flags is cleared (by writing the entire value of the flags).If
 * the start address of the write sequence is 0, then this indicates that any
 * existing written data is out-of-date and the "written" flags should be marked
 * as old by copying them to the "old" flags.
 *
 * The base of the written data is thus at the segment following the segments
 * marked as old, and will correspond to a logical address of zero. 
 */
#include "hw_flash.h"
#include "spi_flash.h"
//#include "hw_serial.h"	// For debugging
/**
 * SPI flash on the ESP8266 Espressif boards can only be read from and written
 * to 4 bytes at a time. This structure helps in marshalling bytes for reading
 * and writing other quantities and at unaligned addresses.
 */
typedef union
{
#if FLASH_ALIGN == 4
	uint32 asStdInt;
#elif FLASH_ALIGN == 2
	uint16 asStdInt;
#endif	
	struct
	{
		uint8_t  asBytes[FLASH_ALIGN];
		uint8_t  numBytes;
		WetaFlashPtr addr;
	};
} BUFFER, *PBUFFER;

typedef struct _FLASH
{
	WetaFlashPtr	base;
	WetaFlashFlags	writtenBlocks;
	WetaFlashFlags	oldBlocks;
	WetaFlashPtr	start;
	BUFFER			buffer;
	//uint32_t		bufferOffset;
} FLASH;

// For now avoid the need for dynamic memory allocation of flash
// structures by having only one static one that gets passed back to the
// application. This can be reviewed at a later time if the need arises for more
// than one handle at a time. (eg. more than one VM running)
static FLASH flash;

// Utility routines local to this file.
uint8_t			countFlags(WetaFlashFlags flags);
bool			readFlags(PFLASH flash);
bool			writeFlags(PFLASH flash);
uint8_t			availableBlocks(PFLASH flash);
WetaFlashPtr	calcReadOffset(PFLASH flash);
WetaFlashPtr	calcWriteOffset(PFLASH flash);
void			updateUsedBlocks(PFLASH flash);
void			initBuffer(PBUFFER buffer);
void			updateBufferAddress(PFLASH flash);
bool			readBuffer(PBUFFER buffer);
bool			writeBuffer(PBUFFER buffer);
bool			flushBuffer(PFLASH flash);
bool			marshalByte(PFLASH flash, uint8_t val);
uint8_t			marshalBytes(PFLASH flash, uint8_t* vals, WetaFlashPtr count);

bool WETAFUNCATTR				
hw_flash_init(
	uint8_t*     base, 
	WetaFlashPtr length, 
	uint16_t     flags
)
{
	return true;
}

/** *****************************************************************************
 * @brief Initialise and return a flash handle for the given base address 
 * @param base Required flash base address
 * @return a handle that can be used for subsequent operations
 *******************************************************************************/
PFLASH WETAFUNCATTR			
hw_flash_open(uint8_t* base)
{
	flash.base = (WetaFlashPtr)base;
	if (!readFlags((PFLASH)&flash))
		return 0;
		// For now, set the start address to just after the flags
	flash.start = 2 * sizeof(WetaFlashFlags);
	initBuffer(&flash.buffer);
	updateBufferAddress((PFLASH)&flash);
	return (PFLASH)&flash;
}

bool WETAFUNCATTR				
hw_flash_close(PFLASH flash)
{
	flash->base       = 0;
	flash->writtenBlocks = ~0;
	flash->oldBlocks  = ~0;
	flash->start      = 0;
	initBuffer(&flash->buffer);
	return true;
}

/** *****************************************************************************
 * @brief Get the number of bytes available for writing
 * @param flash A handle returned by hw_flash_init()
 * @return The number of available bytes
 *******************************************************************************/
WetaFlashPtr WETAFUNCATTR 	
hw_flash_free_bytes(PFLASH flash)
{
	return (WetaFlashPtr)(availableBlocks(flash) * FLASH_BLOCK_SIZE);
}

/** *****************************************************************************
 * @brief Prepare for writing to flash
 * @param flash A handle returned by hw_flash_init()
 * @param requiredBytes The amount of data that needs to be written
 * @return true if writing can proceed for the given amount 
 *******************************************************************************/
bool WETAFUNCATTR			
hw_flash_start_write(
	PFLASH       flash, 
	WetaFlashPtr startAddress,
	WetaFlashPtr requiredBytes)
{
	if (!flash->base)
	{
			// hw_flash_open() was not called or failed. flash in uninitialised.
		return false;
	}
	initBuffer(&flash->buffer);
		// Quick sanity check now to see if it's even worth erasing
		// to make space available.
	if (startAddress + requiredBytes > hw_flash_total_size(flash))
		return false;
		// See if the program will fit one way or another.
	if (hw_flash_free_bytes(flash) < startAddress + requiredBytes)
	{
			// Need to erase to make room.
			// TODO: Be smarter about erasing
		hw_flash_erase(flash);
	}
	else
	{
			// There is enough room in the unwritten blocks
			// If the start address is zero, then it's time to mark the currently
			// written blocks (old blocks plus current program blocks) as all old 
			// blocks (not available at all).
		if (startAddress == 0)
		{
				// The written blocks are now all old blocks
			flash->oldBlocks = flash->writtenBlocks;
			if (!writeFlags(flash))
				return false;
			
		}
	}
		// If the start address is 0 then start at the next available block.
		// Otherwise assume the caller knows where to safely continue writing to
		// in the current block.
	if (startAddress == 0)
		flash->start = flash->base + calcWriteOffset(flash);
	else
		flash->start = flash->base + calcReadOffset(flash) + startAddress;
		
		// Set the buffer address to the nearest 4-byte segment that will cover
		// the start address.
	updateBufferAddress(flash);
	uint8_t segOffset = flash->start % FLASH_ALIGN;
		// If the start address isn't on an aligned boundary then the buffer will
		// be involved in the first write.
	if (segOffset != 0)
	{
			// Read any existing values into the buffer so that when it is next
			// written, any values preceding the start address are not changed.
		if (!readBuffer(&flash->buffer))
			return false;
			// Set the current index into the buffer for the first write.
			// i.e. if the start address is not on a 4 byte boundary then find
			// its index into the buffer
		flash->buffer.numBytes = segOffset; 
	}
	return true;
}

/** *****************************************************************************
 * @brief Flush any write data held in the buffer and update flags
 * @param flash A handle returned by hw_flash_init()
 * @return true if the data was flushed and the flags were written
 *******************************************************************************/
bool WETAFUNCATTR
hw_flash_flush(PFLASH flash)
{
	if (!flushBuffer(flash))
		return false;
		
	updateUsedBlocks(flash);
	return writeFlags(flash);
}

/** *****************************************************************************
 * @brief Prepare for reading data from flash
 * @param flash A handle returned by hw_flash_init()
 * @return always true (for now)
 *******************************************************************************/
bool WETAFUNCATTR			
hw_flash_start_read(PFLASH flash, WetaFlashPtr startAddress)
{
	initBuffer(&flash->buffer);
	flash->start = flash->base + calcReadOffset(flash) + startAddress;
	updateBufferAddress(flash);
	return true;
}

/** *****************************************************************************
 * @brief Write a byte to the current location and increment location
 * @param flash A handle returned by hw_flash_init()
 * @param val value to write
 * @return The number of bytes written 
 *******************************************************************************/
WetaFlashPtr WETAFUNCATTR		
hw_flash_write_byte(PFLASH flash, uint8_t val)
{
	if (marshalByte(flash, val))
	{
		updateUsedBlocks(flash);
		return 1;
	}
	return 0;
}

/** *****************************************************************************
 * @brief Write the given bytes to the current location and update location
 * @param flash A handle returned by hw_flash_init()
 * @param vals Array of bytes to write
 * @param count The number of bytes to write
 * @return The number of bytes written
 *******************************************************************************/
WetaFlashPtr WETAFUNCATTR		
hw_flash_write_bytes(PFLASH flash, uint8_t* vals, WetaFlashPtr count)
{
		// Need to append this data to whatever might be in the buffer.
		// Sort that out first.
	uint8_t consumed = marshalBytes(flash, vals, count);
		// If all of the data was consumed then nothing else to do
	if ((WetaFlashPtr)consumed == count)
	{
		updateUsedBlocks(flash);
		return (WetaFlashPtr)consumed;
	}
		// There is more data to write. Write any multiples of 4 bytes first.
	WetaFlashPtr remaining = count - (WetaFlashPtr)consumed;
		// How many bytes will be left over to be buffered?
	uint8_t leftOver = remaining % FLASH_ALIGN;
		// How many bytes can we send now in one hit?
	WetaFlashPtr bulk = remaining - leftOver;
		// Do the bulk write
	if (bulk > 0)
	{
		SpiFlashOpResult rc;
		rc = spi_flash_write(
			flash->buffer.addr, 
			(WetaFlashPtr*)&vals[consumed], // Point past data already written
			bulk);
			 
		if (rc != SPI_FLASH_RESULT_OK)
		{
			// Oops! return 0 bytes written, even though there might have been data
			// that was consumed by the buffer and written. The situation is bad so
			// just ignore the buffered data.
			return 0;
		}
			
			// Update the offset for the buffer to write to next
		flash->buffer.addr += bulk;
			// Mark the buffer as empty so that it gets read
		flash->buffer.asStdInt = 0;
		flash->buffer.numBytes = 0;
	}
	
		// Put the leftover bytes into the buffer
	if (leftOver > 0)
	{
		marshalBytes(flash, &vals[consumed + bulk], leftOver);
	}
	updateUsedBlocks(flash);
	return count;
}

/** *****************************************************************************
 * @brief Read a byte from the given location
 * @param flash A handle returned by hw_flash_init()
 * @param loc Location to read the byte from
 * @param pval store the read byte here
 * @return the next location after this read 
 *******************************************************************************/
WetaFlashPtr WETAFUNCATTR		
hw_flash_read_byte(PFLASH flash, WetaFlashPtr loc, uint8_t* pval)
{
		// We can't read any old byte - we need to read 4 bytes at a 4 byte
		// boundary. So work out where we need to read the 4 bytes and what
		// offset in that 4 bytes is the byte we want.
	uint8_t      byteOffset = (flash->start + loc) % FLASH_ALIGN;
	WetaFlashPtr nearest    = (flash->start + loc) - byteOffset;
		// If the buffer offset is already at the required location and it
		// has the data then we don't need to read the flash again - just 
		// get the byte from the buffer. 
	if (nearest == flash->buffer.addr && flash->buffer.numBytes > byteOffset)
	{
		*pval = flash->buffer.asBytes[byteOffset];
		return 1;
	}
		// Need to fill the buffer from the new location
	flash->buffer.addr = nearest;
	if (readBuffer(&flash->buffer))
	{
		flash->buffer.numBytes = FLASH_ALIGN;
		*pval = flash->buffer.asBytes[byteOffset];
		return 1;
	}
	else
	{
		flash->buffer.numBytes = 0;
		return 0;
	}
}

/** *****************************************************************************
 * @brief Read bytes from the given location
 * @param flash A handle returned by hw_flash_init()
 * @param loc The location to read from
 * @param vals Array to store the received bytes in
 * @param count The number of bytes to read
 * @return The next location after this read 
 *******************************************************************************/
/*
WetaFlashPtr WETAFUNCATTR		
hw_flash_read_bytes(PFLASH flash, WetaFlashPtr loc, uint8_t* vals, WetaFlashPtr count)
{
	
	uint8 bytes[4];
	uint16 bytesRead = 0;
	while (bytesRead < count)
	{
		SpiFlashOpResult rc = spi_flash_read(loc + bytesRead, (uint32 *)bytes, 4);
		if (rc != SPI_FLASH_RESULT_OK)
			break;
		vals[bytesRead]   = bytes[0];
		vals[bytesRead+1] = bytes[1];
		vals[bytesRead+2] = bytes[2];
		vals[bytesRead+3] = bytes[3];
		bytesRead += 4;
	}
	return bytesRead;
}
*/

WetaFlashPtr WETAFUNCATTR		
hw_flash_read_bytes(PFLASH flash, WetaFlashPtr loc, uint8_t* vals, WetaFlashPtr count)
{
		// We can't read bytes from any location. Data must be read from a 4
		// byte boundary and the length must be a multiple of 4 bytes. Use the
		// buffer to read any data that falls outside these boundaries.
	uint8_t      byteOffset = (flash->start + loc) % FLASH_ALIGN;
	WetaFlashPtr nearest    = (flash->start + loc) - byteOffset;
	WetaFlashPtr copied     = 0;
		// If we're not on a 4 byte boundary then use the buffer to get the
		// preceding fragment.
	if (byteOffset > 0)
	{
			// If the buffer has data from elsewhere or is not full, then fill
			// it from flash now.
		if (nearest != flash->buffer.addr || flash->buffer.numBytes < FLASH_ALIGN)
		{
				// The buffer doesn't already have the first part of the data we
				// require so read it now
			flash->buffer.addr = nearest;
			if (!readBuffer(&flash->buffer))
				return 0;
				
			flash->buffer.numBytes = FLASH_ALIGN;
		}
			// Copy data from the buffer
		while (byteOffset + copied < FLASH_ALIGN && copied < count)
		{
			vals[copied] = flash->buffer.asBytes[byteOffset + copied];
			copied++;
		}
		nearest += FLASH_ALIGN; // Point to the next boundary
		//hw_serial_write(0, flash->buffer.asBytes, flash->buffer.numBytes);
		//return count;
	}
	
		// Now we can read the middle part of the data.
		// (If there is any middle part)
	WetaFlashPtr remaining = count - copied;
		// Get the amount of tail past the last boundary
	uint8_t last = remaining % FLASH_ALIGN;
		//Is there enough data to have a middle part?
	if (remaining >= FLASH_ALIGN)
	{
			// More than 4 bytes left to read.
		WetaFlashPtr middle = remaining - last;
		while (middle)
		{
			flash->buffer.addr = nearest;
			if (!readBuffer(&flash->buffer))
				return 0;
			flash->buffer.numBytes = FLASH_ALIGN;
			vals[copied++] = flash->buffer.asBytes[0];
			vals[copied++] = flash->buffer.asBytes[1];
			vals[copied++] = flash->buffer.asBytes[2];
			vals[copied++] = flash->buffer.asBytes[3];
			nearest += FLASH_ALIGN;
			middle  -= FLASH_ALIGN;
			//hw_serial_write(0, flash->buffer.asBytes, flash->buffer.numBytes);
		}
	}
	else
	{
			// last is wrong in this case - remaining is the last amount.
		last = remaining;
	}

		// Any remaining data to be read can use the buffer.
		// It will be less that FLASH_ALIGN.
	if (last > 0)
	{
		flash->buffer.addr = nearest;
		if (readBuffer(&flash->buffer))
		{
			flash->buffer.numBytes = FLASH_ALIGN;
			byteOffset = 0;
				// Copy the data we need from the buffer
			while (byteOffset < last)
			{
				vals[copied++] = flash->buffer.asBytes[byteOffset++];
			}
			//hw_serial_write(0, flash->buffer.asBytes, flash->buffer.numBytes);
		}
		else
		{
			flash->buffer.numBytes = 0;
			return 0;
		}
	}
	return count;
}

/** *****************************************************************************
 * @brief Erase flash 
 * @param flash A handle returned by hw_flash_init()
 * @return True if the flash was successfully erased 
 *******************************************************************************/
bool WETAFUNCATTR				
hw_flash_erase(PFLASH flash)
{
		// Just erase one sector for now
		// TODO: Deal with larger flash amounts
	SpiFlashOpResult rc = spi_flash_erase_sector(flash->base >> 12);
	
	return rc == SPI_FLASH_RESULT_OK;
}

/** *****************************************************************************
 * @brief Get the total amount of flash memory available
 * @param flash A handle returned by hw_flash_init()
 * @return The total number of bytes of flash
 *******************************************************************************/
WetaFlashPtr WETAFUNCATTR 
hw_flash_total_size(PFLASH flash)
{
		// This isn't necessarily true, byt it's the amount we can manage with
		// 32 bit usage flags and the given block size.
		// (the 8 is to get the number of bits in the flags)
	return sizeof(WetaFlashFlags) * 8 * FLASH_BLOCK_SIZE;
}

/** *****************************************************************************
 * @brief Count the number of "set" flags
 * @param flags Flags to count
 * @return The number of set flags 
 *******************************************************************************/
uint8_t WETAFUNCATTR
countFlags(WetaFlashFlags flags)
{
		// Note that "set" flags are zero because flash is erased to all 1s.
		// The flags are written repeatedly without an erase but with 0's
		// accumulating from the lsb.
	WetaFlashFlags tempflags = flags; 
	uint8_t result = 0;
	while ((tempflags & 0x0001) == 0)
	{
		result++;
		tempflags >>= 1;
	}
	return result;
}

/** *****************************************************************************
 * @brief Read the usage flags from the beginning of flash
 * @param flash A handle returned by hw_flash_init()
 * @return true if the flags were successfully read. 
 *******************************************************************************/
bool WETAFUNCATTR			
readFlags(PFLASH flash)
{
	SpiFlashOpResult rc;
 
	rc = spi_flash_read(
			flash->base, 
			&flash->writtenBlocks, 
			sizeof(WetaFlashFlags)
	);
	if (rc != SPI_FLASH_RESULT_OK)
		return false;
		
	rc = spi_flash_read(
		flash->base + sizeof(WetaFlashFlags), 
		&flash->oldBlocks, 
		sizeof(WetaFlashFlags)
	);	
	return rc == SPI_FLASH_RESULT_OK;	
}

/** *****************************************************************************
 * @brief Write the usage flags to the beginning of flash
 * @param flash A handle returned by hw_flash_init()
 * @return True if the flags were successfully written
 *******************************************************************************/
bool WETAFUNCATTR			
writeFlags(PFLASH flash)
{
	SpiFlashOpResult rc;
 
	rc = spi_flash_write(
			flash->base, 
			&flash->writtenBlocks, 
			sizeof(WetaFlashFlags)
	);
	if (rc != SPI_FLASH_RESULT_OK)
		return false;
		
	rc = spi_flash_write(
		flash->base + sizeof(WetaFlashFlags), 
		&flash->oldBlocks, 
		sizeof(WetaFlashFlags)
	);	
	return rc == SPI_FLASH_RESULT_OK;
}

/** *****************************************************************************
 * @brief Return the number of blocks available for writing
 * @param flash A handle returned by hw_flash_init()
 * @return The number of available blocks
 *******************************************************************************/
uint8_t WETAFUNCATTR 
availableBlocks(PFLASH flash)
{
	return (sizeof (WetaFlashFlags) * 8)	//i.e. the number of bits
			- countFlags(flash->writtenBlocks);		// minus the number written
}

/** *****************************************************************************
 * @brief Calculate the offset from the flash base address to the current data. 
 * @param flash A handle returned by hw_flash_init()
 * @return Offset to the beginning of the current existing data
 *
 * Old blocks are those that were written just before the last write operation.
 * When hw_flash_open_write() is called, the blocks currently flagged as written
 * are flagged as old, and the new start address for data is the next
 * sequential block.  
 *******************************************************************************/
WetaFlashPtr WETAFUNCATTR 
calcReadOffset(PFLASH flash)
{
		// See if we need to skip past any old code
	uint8_t numOldBlocks  = countFlags(flash->oldBlocks);
	if (numOldBlocks > 0)
		return (WetaFlashPtr)(numOldBlocks * FLASH_BLOCK_SIZE);
		
		// We're reading the first program written after an erase.
		// Point to just after the two 32-bit flags.
	return 2 * sizeof(WetaFlashFlags);
}

/** *****************************************************************************
 * @brief Calculate the offset from the flash base address to write new data. 
 * @param flash A handle returned by hw_flash_init()
 * @return Offset to the address at which new flash data is to be written.
 *
 * Used blocks are those that have been written to, with old blocks being a
 * subset of these. So the current data set is indicated by the written blocks
 * that are not also marked as old. Writing starts at the next block that is
 * not written.
 *******************************************************************************/
WetaFlashPtr WETAFUNCATTR 
calcWriteOffset(PFLASH flash)
{
		// See if we need to skip past any old code
	uint8_t numUsedBlocks  = countFlags(flash->writtenBlocks);
	if (numUsedBlocks > 0)
		return (WetaFlashPtr)(numUsedBlocks * FLASH_BLOCK_SIZE);
		
		// We're reading the first program written after an erase.
		// Point to just after the two 16-bit flags.
	return 2 * sizeof(WetaFlashFlags);
}

/** *****************************************************************************
 * @brief Update the "written" flags based on data being written up to the current
 * 			location.
 * @param flash A handle returned by hw_flash_init()
 * @param loc The location up to which data has been written
 * @return 
 *******************************************************************************/
void WETAFUNCATTR			
updateUsedBlocks(PFLASH flash)
{
	uint8_t block = 0;
	while (((flash->buffer.addr) - flash->base) > (WetaFlashPtr)block * FLASH_BLOCK_SIZE)
	{
		flash->writtenBlocks &= ~(1 << block);
		block++;
	}
}

/** *****************************************************************************
 * @brief Initialise the buffer used to manage reads outside of 4 byte segments
 * @param flash A handle returned by hw_flash_init()
 *******************************************************************************/
void WETAFUNCATTR			
initBuffer(PBUFFER buffer)
{
	buffer->asStdInt = 0;
	buffer->numBytes = 0;
	buffer->addr     = 0;
}

void WETAFUNCATTR			
updateBufferAddress(PFLASH flash)
{
	uint8_t byteOffset = flash->start % FLASH_ALIGN;
	flash->buffer.addr = flash->start - byteOffset;
}

bool WETAFUNCATTR			
readBuffer(PBUFFER buffer)
{
	SpiFlashOpResult rc;
	rc = spi_flash_read(buffer->addr, &buffer->asStdInt, FLASH_ALIGN);
	return rc == SPI_FLASH_RESULT_OK;
}

bool WETAFUNCATTR			
writeBuffer(PBUFFER buffer)
{
	//hw_serial_write(0, buffer->asBytes, FLASH_ALIGN);
	//return true;
	
	SpiFlashOpResult rc;
	rc = spi_flash_write(buffer->addr, (WetaFlashPtr*)buffer->asBytes, FLASH_ALIGN);
	return rc == SPI_FLASH_RESULT_OK;
	
}

/** *****************************************************************************
 * @brief Write any outstanding data in the buffer to flash
 * @param flash A handle returned by hw_flash_init()
 * @return True if the buffer was successfully written (or it wasn't required)
 *******************************************************************************/
bool WETAFUNCATTR			
flushBuffer(PFLASH flash)
{
		// If there is nothing in the buffer then do nothing
	if (flash->buffer.numBytes == 0)
		return true;
		
		// Fill any unsupplied bytes with 0xFF to look like
		// erased flash.
	uint8_t i;
	for (i = flash->buffer.numBytes; i < FLASH_ALIGN; i++)
		flash->buffer.asBytes[i] = 0xFF;
		
		// Write the buffer to flash
	if (!writeBuffer(&flash->buffer))
		return false;
		
		// Clear the buffer
	flash->buffer.asStdInt = 0;
	flash->buffer.numBytes = 0;	
		// Increment the offset to the next boundary
	flash->buffer.addr += FLASH_ALIGN;
	return true;
}

/** *****************************************************************************
 * @brief Store a byte into the next location in the buffer, flushing if full.
 * @param flash A handle returned by hw_flash_init()
 * @param val byte to store
 * @return true if a flush was successful (or was not necessary)
 *******************************************************************************/
bool WETAFUNCATTR			
marshalByte(PFLASH flash, uint8_t val)
{
	if (flash->buffer.numBytes == FLASH_ALIGN)
		if (!flushBuffer(flash))
			return false;
			
	flash->buffer.asBytes[flash->buffer.numBytes++] = val;
	
	if (flash->buffer.numBytes == FLASH_ALIGN)
		if (!flushBuffer(flash))
			return false;
			
	return true;
}

/** *****************************************************************************
 * @brief Fill the buffer using the given bytes
 * @param flash A handle returned by hw_flash_init()
 * @param vals bytes to use
 * @param count the number of available bytes
 * @return The number of bytes actually consumed from the data (<=4)
 *******************************************************************************/
uint8_t WETAFUNCATTR			
marshalBytes(PFLASH flash, uint8_t* vals, WetaFlashPtr count)
{
	uint8_t consumed = 0;
	while (flash->buffer.numBytes < FLASH_ALIGN && consumed < count)
		flash->buffer.asBytes[flash->buffer.numBytes++] = vals[consumed++];
	
	if (flash->buffer.numBytes == FLASH_ALIGN)
		flushBuffer(flash);
		
	return consumed;
}