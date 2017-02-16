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

//*******************************************************************************
bool WETAFUNCATTR 
weta_store_start_read(PSTORE store, WetaCodePtr startAddress)
{
    return store->methods->start_read(store, startAddress);
}

//*******************************************************************************
bool WETAFUNCATTR 
weta_store_start_write(PSTORE store, WetaCodePtr startAddress, WetaCodePtr length)
{
    return store->methods->start_write(store, startAddress, length);
}

//*******************************************************************************
bool WETAFUNCATTR 
weta_store_flush(PSTORE store)
{
    return store->methods->flush(store);
}

//*******************************************************************************
void WETAFUNCATTR        
weta_store_close(PSTORE store)
{
    store->methods->close(store);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_byte(PSTORE store, uint8_t val)
{
    return store->methods->write_byte(store, val);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_bytes(PSTORE store, uint8_t* buf, WetaCodePtr length)
{
    return store->methods->write_bytes(store, buf, length);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR
weta_store_read_byte(
    PSTORE      store,
    WetaCodePtr     address,
    uint8_t*        pval)
{
    return store->methods->read_byte(store, address, pval);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR
weta_store_read_bytes(
    PSTORE      store,
    WetaCodePtr     address,
    uint8_t*        buf,
    WetaCodePtr     length)
{
    return store->methods->read_bytes(store, address, buf, length);
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_uint16(PSTORE store, uint16_t val)
{

	uint8_t  buf[sizeof(val)];
	hton_uint16(val, buf);
    return store->methods->write_bytes(store, buf, sizeof(buf));
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_uint32(PSTORE store, uint32_t val)
{
	uint8_t  buf[sizeof(val)];
	hton_uint32(val, buf);
	return store->methods->write_bytes(store, buf, sizeof(buf));
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_float(PSTORE store, float val)
{
	uint8_t  buf[sizeof(val)];
	hton_float(val, buf);
	return store->methods->write_bytes(store, buf, sizeof(buf));
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_write_double(PSTORE store, double val)
{
	uint8_t  buf[sizeof(val)];
	hton_double(val, buf);
	return store->methods->write_bytes(store, buf, sizeof(buf));
}



//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_uint16(PSTORE store, WetaCodePtr address, uint16_t* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead = store->methods->read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_uint16(buf);

	return bytesRead;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_uint32(PSTORE store, WetaCodePtr address, uint32_t* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead =store->methods->read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_uint32(buf);

	return bytesRead;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_float(PSTORE store, WetaCodePtr address,  float* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead = store->methods->read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_float(buf);

	return bytesRead;
}

//*******************************************************************************
WetaCodePtr WETAFUNCATTR 
weta_store_read_double(PSTORE store, WetaCodePtr address, double* pval)
{
	uint8_t  buf[sizeof(*pval)];
	WetaCodePtr bytesRead = store->methods->read_bytes(store, address, buf, sizeof(buf));
	if (bytesRead == sizeof(buf))
		*pval = ntoh_double(buf);

	return bytesRead;
}

WetaCodePtr WETAFUNCATTR 
weta_store_read_string(PSTORE store, WetaCodePtr address, uint8_t* psz, uint8_t length)
{
	uint8_t count = 0;
	uint8_t next;
	while (store->methods->read_byte(store, address++, &next) == 1 && count < length)
	{
		psz[count++] = next;
		if (!next)
			break;
	}
	return (WetaCodePtr)count;
}
