/**
 * @file weta_flash.c
 * @author Murray Lang
 * @date 05/15/15
 * @brief Implementation of flash storage for byte codes
 */
#include "weta_store.h"
#include "./hw/hw_endian.h"
#include "./hw/hw_flash.h"

//#include <ch.h>				// For debugging
//#include <hal.h>			// For debugging
//#include "./hw/hw_serial.h" // For debugging
//#include <stdio.h>          // For debugging (printf())

//#include <stdlib.h>	// for malloc() and free()
//#include <mem.h>	// for os_malloc() and os_free()

struct _STORE
{
	PFLASH      flash;
	uint8_t* 	data;
	WetaCodePtr length;
};

PSTORE WETAFUNCATTR 
weta_store_init_flash(uint8_t* flashAddr, WetaCodePtr length)
{
	PFLASH flash = hw_flash_open(flashAddr);
	if (!flash)
		return 0;
		
	PSTORE store = (PSTORE)weta_malloc(sizeof(STORE));

	store->flash  = flash;
	store->data   = 0;
	store->length = 0;
	
	return store;
}

//*******************************************************************************
bool WETAFUNCATTR 
weta_store_start_read(PSTORE store, WetaCodePtr startAddress)
{
		// Firstly prepare the flash for reading
	if (!hw_flash_start_read(store->flash, startAddress))
	{
		//printf("hw_flash_start_read() failed\n");
		return false;
	}
		
		// Read the data length from the first 2 bytes
	uint8_t buff[2];
	WetaFlashPtr bytesRead = hw_flash_read_bytes(store->flash, 0, buff, 2);
	if (bytesRead != 2)
	{
		//printf("hw_flash_read_bytes() returned %d bytes. Should be 2.\n", bytesRead);
		return false;
	}
	
	store->length = ntoh_uint16(buff);
		// If the length is 0xFFFF then this probably indicates that the flash
		// is still erased. Return failure.
	if (store->length == 0xFFFF)
	{
        //printf("Store Length field is 0xFFFF, indicating erased flash.\n");
		return false;
	}
	
	if (store->data)
		weta_free(store->data);
		
	//hw_serial_write(0, buff, 2); // Debugging
	
	store->data = (uint8_t*)weta_malloc(store->length);
	if (!store->data)
	{
		store->length = 0;
		return false;
	}
	
		// Read ALL of the data into the cache! (Skip the length data)
	bytesRead = hw_flash_read_bytes(store->flash, 2, store->data, store->length);
	//bytesRead = hw_flash_read_bytes(store->flash, 0, store->data, store->length);
		// Make sure it was all read
	if (bytesRead != store->length)
	{
		weta_free(store->data);
		store->data = 0;
		store->length = 0;
		return false;
	}
	//hw_serial_write(0, store->data, store->length); //Debugging
	return true;
}

//*******************************************************************************
bool WETAFUNCATTR 
weta_store_start_write(PSTORE store, WetaCodePtr startAddress, WetaCodePtr length)
{
	//hw_flash_erase(store->flash); // Debugging TODO: remove this line
	
	if (!hw_flash_start_write(store->flash, (WetaFlashPtr)startAddress, (WetaFlashPtr)length))
		return false;

		// Convert the length to network byte order and store at the front
	uint8_t buff[2];
	hton_uint16((uint16_t)length, buff);
	WetaFlashPtr bytesWritten = hw_flash_write_bytes(store->flash, buff, 2);
	
	return bytesWritten == 2;
}

//*******************************************************************************
bool WETAFUNCATTR 
weta_store_flush(PSTORE store)
{
	return hw_flash_flush(store->flash); 
}

//*******************************************************************************
void WETAFUNCATTR        
weta_store_close(PSTORE store)
{
	hw_flash_close(store->flash);
	weta_free(store->data);	// Free the data buffer first
	weta_free(store);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_byte(PSTORE store, uint8_t val)
{
	return (WetaCodePtr)hw_flash_write_byte(store->flash, val);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_bytes(PSTORE store, uint8_t* buf, WetaCodePtr length)
{
	return hw_flash_write_bytes(store->flash, buf, (WetaFlashPtr)length);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_uint16(PSTORE store, uint16_t val)
{
	uint8_t  buf[sizeof(val)];
	hton_uint16(val, buf);
	return weta_store_write_bytes(store, buf, sizeof(buf));
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_uint32(PSTORE store, uint32_t val)
{
	uint8_t  buf[sizeof(val)];
	hton_uint32(val, buf);
	return weta_store_write_bytes(store, buf, sizeof(buf));
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_float(PSTORE store, float val)
{
	uint8_t  buf[sizeof(val)];
	hton_float(val, buf);
	return weta_store_write_bytes(store, buf, sizeof(buf));
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_double(PSTORE store, double val)
{
	uint8_t  buf[sizeof(val)];
	hton_double(val, buf);
	return weta_store_write_bytes(store, buf, sizeof(buf));
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_byte(
	PSTORE      store, 
	WetaCodePtr     address,  
	uint8_t*        pval)
{
	*pval = store->data[address];
	
	return 1;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_bytes(
	PSTORE      store, 
	WetaCodePtr     address, 
	uint8_t*        buf, 
	WetaCodePtr     length)
{
	uint8_t* to = buf;
	uint8_t* from = store->data + address;
	uint16_t i;
	for (i = 0; i < length; i++)
		*to++ = *from++;
	return length;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_uint16(PSTORE store, WetaCodePtr address, uint16_t* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead = weta_store_read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_uint16(buf);

	return bytesRead;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_uint32(PSTORE store, WetaCodePtr address, uint32_t* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead = weta_store_read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_uint32(buf);

	return bytesRead;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_float(PSTORE store, WetaCodePtr address,  float* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead = weta_store_read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_float(buf);

	return bytesRead;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_double(PSTORE store, WetaCodePtr address, double* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead = weta_store_read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_double(buf);

	return bytesRead;
}

WetaCodePtr WETAFUNCATTR 
weta_store_read_string(PSTORE store, WetaCodePtr address, uint8_t* psz, uint8_t length)
{
	uint8_t count = 0;
	uint8_t next;
	while (weta_store_read_byte(store, address++, &next) == 1 && count < length)
	{
		psz[count++] = next;
		if (!next)
			break;
	}
	return (WetaCodePtr)count;
}
