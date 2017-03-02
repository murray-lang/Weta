/**
 * @file weta_flash.c
 * @author Murray Lang
 * @date 05/15/15
 * @brief Implementation of flash storage for byte codes
 */
#include "weta_store.h"
#include "./hw/hw_endian.h"

//#include <ch.h>				// For debugging
//#include <hal.h>			// For debugging
//#include "./hw/hw_serial.h" // For debugging
//#include <stdio.h>          // For debugging (printf())


struct _RAM
{
	WetaStorage   	type;
    StoreMethods*   methods;
	uint8_t* 	    buffer;
	WetaCodePtr     buffer_length;
    WetaCodePtr     code_length;
        // When writing, the cursor is incremented to point to the next write
        // location. When reading, the cursor stays at the given start address
        // and is added to every "address" provided.
    WetaCodePtr     cursor;
};

static bool        weta_ram_start_write(PSTORE store, WetaCodePtr startAddress, WetaCodePtr length);
static bool        weta_ram_start_read(PSTORE store, WetaCodePtr startAddress);
static bool        weta_ram_flush(PSTORE store);
static void        weta_ram_close(PSTORE store);
static WetaCodePtr weta_ram_write_byte(PSTORE store, uint8_t val);
static WetaCodePtr weta_ram_write_bytes(PSTORE store, uint8_t* buf, WetaCodePtr length);
static WetaCodePtr weta_ram_read_byte(PSTORE store, WetaCodePtr address, uint8_t* pval);
static WetaCodePtr weta_ram_read_bytes(PSTORE store, WetaCodePtr address, uint8_t* buf, WetaCodePtr length);

static StoreMethods methods =
    {
        .start_write = weta_ram_start_write,
        .start_read  = weta_ram_start_read,
        .flush       = weta_ram_flush,
        .close       = weta_ram_close,
        .write_byte  = weta_ram_write_byte,
        .write_bytes = weta_ram_write_bytes,
        .read_byte   = weta_ram_read_byte,
        .read_bytes  = weta_ram_read_bytes
    };

PSTORE WETAFUNCATTR 
weta_store_open_ram(void)
{
    DEBUGMSG("weta_store_open_ram()\r\n");
	struct _RAM * store = (struct _RAM *)weta_malloc(sizeof(struct _RAM));
    if (!store)
    {
        DEBUGMSG("Error allocating memory for program info.\n\r");
        return 0;
    }

	store->type    = STORAGE_RAM;
    store->methods = &methods;
    store->buffer  = NULL;
    store->buffer_length = 0;
    store->code_length  = 0;
    store->cursor  = 0;
    return (PSTORE)store;
}

//*******************************************************************************
static bool WETAFUNCATTR
weta_ram_start_read(PSTORE _store, WetaCodePtr startAddress)
{
    DEBUGMSG("weta_ram_start_read(%d)\r\n", startAddress);

    struct _RAM* store = (struct _RAM*)_store;

	if (sizeof(uint16_t) + startAddress >= store->buffer_length)
	{
		DEBUGMSG("Given start address is past the buffer\n\r");
		return false;
	}
		
		// Read the data length from the first 2 bytes
	store->code_length = ntoh_uint16(store->buffer + startAddress);
	if (sizeof(uint16_t) + startAddress + store->code_length > store->buffer_length)
	{
        DEBUGMSG("Code length (%d + %d) goes past the end of the buffer\n\r",startAddress,  store->code_length);
		return false;
	}
	DEBUGMSG("Code length is %d\n\r", store->code_length);
    store->cursor = sizeof(uint16_t) + startAddress; // Step past length info
	return true;
}

//*******************************************************************************
static bool WETAFUNCATTR
weta_ram_start_write(PSTORE _store, WetaCodePtr startAddress, WetaCodePtr length)
{
    DEBUGMSG("weta_ram_start_write(%d, %d)\r\n", startAddress, length);
    struct _RAM* store = (struct _RAM*)_store;

    // Allocate extra space for length info at the beginning
    WetaCodePtr new_buffer_length = sizeof(uint16_t) + startAddress +length;
    if (store->buffer)
    {
        if (new_buffer_length > store->buffer_length)
        {
            weta_free(store->buffer);
            store->buffer = 0;
            store->buffer_length = 0;
        }
    }
    if (!store->buffer)
    {
        store->buffer = weta_malloc(new_buffer_length);
    }
    if (!store->buffer)
    {
        DEBUGMSG("Error allocating memory for program.\n\r");
        weta_free(store);
        return 0;
    }
    store->buffer_length = new_buffer_length;

		// Convert the code length to network byte order and store at the front
	hton_uint16((uint16_t)length, store->buffer + startAddress);
    store->cursor = sizeof(uint16_t) + startAddress;
	return true;
}

//*******************************************************************************
static bool WETAFUNCATTR
weta_ram_flush(PSTORE _store)
{
        // Nothing to do since no caching
	return true;
}

//*******************************************************************************
static void WETAFUNCATTR
weta_ram_close(PSTORE _store)
{
    struct _RAM* store = (struct _RAM*)_store;
	weta_free(store->buffer);	// Free the data buffer first
	weta_free(store);
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_ram_write_byte(PSTORE _store, uint8_t val)
{
    struct _RAM* store = (struct _RAM*)_store;
    if (store->cursor >= store->buffer_length)
    {
        DEBUGMSG("Attempt to write past the buffer\r\n");
        return 0;
    }
    store->buffer[store->cursor++] = val;
	return 1;
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_ram_write_bytes(PSTORE _store, uint8_t* buf, WetaCodePtr length)
{
    struct _RAM* store = (struct _RAM*)_store;
    if (store->cursor + length >= store->buffer_length)
    {
        DEBUGMSG("Attempt to write past the buffer\r\n");
        return INVALID_CODEPTR;
    }
    WetaCodePtr i;
    for (i = 0; i < length; i++)
    {
        store->buffer[store->cursor++] = buf[i];
    }
	return length;
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_ram_read_byte(
	PSTORE      _store,
	WetaCodePtr     address,
	uint8_t*        pval)
{
    struct _RAM* store = (struct _RAM*)_store;
    if (address >= store->buffer_length)
    {
        DEBUGMSG("Attempt to read past the buffer (address = %d, buffer length = %d)\r\n", address, store->buffer_length);
        return INVALID_CODEPTR;
    }
	*pval = store->buffer[store->cursor + address];

	return 1;
}

//*******************************************************************************
static WetaCodePtr WETAFUNCATTR
weta_ram_read_bytes(
	PSTORE      _store,
	WetaCodePtr     address,
	uint8_t*        buf,
	WetaCodePtr     length)
{
    struct _RAM* store = (struct _RAM*)_store;
    if (store->cursor + address + length > store->buffer_length)
    {
        DEBUGMSG("Attempt to read past the buffer\r\n");
        return INVALID_CODEPTR;
    }
	uint8_t* to = buf;
	uint8_t* from = store->buffer + store->cursor + address;
    WetaCodePtr i;
	for (i = 0; i < length; i++)
		*to++ = *from++;
	return length;
}
