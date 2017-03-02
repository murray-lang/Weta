/**
 * @file weta_flash.c
 * @author Murray Lang
 * @date 05/15/15
 * @brief Implementation of flash storage for byte codes
 */
#include "weta_store_flash.h"
#include "./hw/hw_endian.h"
#include "./hw/hw_flash.h"

//#include <ch.h>				// For debugging
//#include <hal.h>			// For debugging
//#include "./hw/hw_serial.h" // For debugging
//#include <stdio.h>          // For debugging (printf())


struct _FLASH
{
	WetaStorage   	type;
    StoreMethods*   methods;
	PFLASH          flash;
	uint8_t* 	    data;
	WetaCodePtr     length;
};

static bool weta_flash_start_write(PSTORE store, WetaCodePtr startAddress, WetaCodePtr length);
static bool weta_flash_start_read(PSTORE store, WetaCodePtr startAddress);
static bool weta_flash_flush(PSTORE store);
static void weta_flash_close(PSTORE store);
static WetaCodePtr weta_flash_write_byte(PSTORE store, uint8_t val);
static WetaCodePtr weta_flash_write_bytes(PSTORE store, uint8_t* buf, WetaCodePtr length);
static WetaCodePtr weta_flash_read_byte(PSTORE store, WetaCodePtr address, uint8_t* pval);
static WetaCodePtr weta_flash_read_bytes(PSTORE store, WetaCodePtr address, uint8_t* buf, WetaCodePtr length);

static StoreMethods methods =
    {
        .start_write = weta_flash_start_write,
        .start_read  = weta_flash_start_read,
        .flush       = weta_flash_flush,
        .close       = weta_flash_close,
        .write_byte  = weta_flash_write_byte,
        .write_bytes = weta_flash_write_bytes,
        .read_byte   = weta_flash_read_byte,
        .read_bytes  = weta_flash_read_bytes
    };

PSTORE WETAFUNCATTR 
weta_store_open_flash(uint8_t* flashAddr)
{
	PFLASH flash = hw_flash_open(flashAddr);
	if (!flash)
		return 0;
		
	struct _FLASH * store = (struct _FLASH *)weta_malloc(sizeof(struct _FLASH));

	store->type    = STORAGE_FLASH;
    store->methods = &methods;
	store->flash   = flash;
	store->data    = 0;
	store->length  = 0;
	
	return (PSTORE)store;
}

//*******************************************************************************
static bool WETAFUNCATTR
weta_flash_start_read(PSTORE _store, WetaCodePtr startAddress)
{
    struct _FLASH* store = (struct _FLASH*)_store;
		// Firstly prepare the flash for reading
	if (!hw_flash_start_read(store->flash, startAddress))
	{
		DEBUGMSG("hw_flash_start_read() failed\n\r");
		return false;
	}
		
		// Read the data length from the first 2 bytes
	uint8_t buff[2];
	WetaFlashPtr bytesRead = hw_flash_read_bytes(store->flash, 0, buff, 2);
	if (bytesRead != 2)
	{
		DEBUGMSG("hw_flash_read_bytes() returned %d bytes. Should be 2.\n\r", bytesRead);
		return false;
	}

	store->length = ntoh_uint16(buff);
		// If the length is 0xFFFF then this probably indicates that the flash
		// is still erased. Return failure.
	if (store->length == 0xFFFF)
	{
        DEBUGMSG("Store Length field is 0xFFFF, indicating erased flash.\n\r");
		return false;
	}
	DEBUGMSG("Program length is %d\n\r", store->length);
	if (store->data)
		weta_free(store->data);
		
	//hw_serial_write(0, buff, 2); // Debugging

	store->data = (uint8_t*)weta_malloc(store->length);
	if (!store->data)
	{
		DEBUGMSG("Error allocating memory for program cache.\n\r");
		store->length = 0;
		return false;
	}
	
		// Read ALL of the data into the cache! (Skip the length data)
	bytesRead = hw_flash_read_bytes(store->flash, 2, store->data, store->length);
	//bytesRead = hw_flash_read_bytes(store->flash, 0, store->data, store->length);
		// Make sure it was all read
	if (bytesRead != store->length)
	{
		DEBUGMSG("Error reading program cache.\n\r");
		weta_free(store->data);
		store->data = 0;
		store->length = 0;
		return false;
	}
	//hw_serial_write(0, store->data, store->length); //Debugging
	return true;
}

//*******************************************************************************
static bool WETAFUNCATTR
weta_flash_start_write(PSTORE _store, WetaCodePtr startAddress, WetaCodePtr length)
{
    struct _FLASH* store = (struct _FLASH*)_store;
	//hw_flash_erase(store->flash); // Debugging TODO: remove this line
	
	if (!hw_flash_start_write(store->flash, (WetaFlashPtr)startAddress, (WetaFlashPtr)length))
    {
        DEBUGMSG("hw_flash_start_write() failed\r\n");
        return false;
    }

		// Convert the length to network byte order and store at the front
	uint8_t buff[2];
	hton_uint16((uint16_t)length, buff);
	WetaFlashPtr bytesWritten = hw_flash_write_bytes(store->flash, buff, 2);
	
	return bytesWritten == 2;
}

//*******************************************************************************
static bool WETAFUNCATTR
weta_flash_flush(PSTORE _store)
{
    struct _FLASH* store = (struct _FLASH*)_store;
	return hw_flash_flush(store->flash); 
}

//*******************************************************************************
static void WETAFUNCATTR
weta_flash_close(PSTORE _store)
{
    struct _FLASH* store = (struct _FLASH*)_store;
	hw_flash_close(store->flash);
	weta_free(store->data);	// Free the data buffer first
	weta_free(store);
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_flash_write_byte(PSTORE _store, uint8_t val)
{
    struct _FLASH* store = (struct _FLASH*)_store;
	return (WetaCodePtr)hw_flash_write_byte(store->flash, val);
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_flash_write_bytes(PSTORE _store, uint8_t* buf, WetaCodePtr length)
{
    struct _FLASH* store = (struct _FLASH*)_store;
	return hw_flash_write_bytes(store->flash, buf, (WetaFlashPtr)length);
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_flash_read_byte(
	PSTORE      _store,
	WetaCodePtr     address,
	uint8_t*        pval)
{
    struct _FLASH* store = (struct _FLASH*)_store;
	*pval = store->data[address];

	return 1;
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_flash_read_bytes(
	PSTORE      _store,
	WetaCodePtr     address,
	uint8_t*        buf,
	WetaCodePtr     length)
{
    struct _FLASH* store = (struct _FLASH*)_store;
	uint8_t* to = buf;
	uint8_t* from = store->data + address;
	uint16_t i;
	for (i = 0; i < length; i++)
		*to++ = *from++;
	return length;
}
