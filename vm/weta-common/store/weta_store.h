#ifndef __WETA_STORE_H__
#define __WETA_STORE_H__
/** 
 * @file weta_store.h
 * @author Murray Lang
 * @brief Functions for accessing byte codes in flash memory.
 */

#include "weta_platform.h"

typedef enum
{
    STORAGE_NONE,
    STORAGE_FLASH,
    STORAGE_PROM,
    STORAGE_RAM
} WetaStorage;

struct _STORE;

typedef struct _STORE *PSTORE;

typedef struct
{
    bool (*start_write)(PSTORE, WetaCodePtr, WetaCodePtr);
    bool (*start_read)(PSTORE, WetaCodePtr);
    bool (*flush)(PSTORE);
    void (*close)(PSTORE);
    WetaCodePtr (*write_byte)(PSTORE, uint8_t);
    WetaCodePtr (*write_bytes)(PSTORE, uint8_t*, WetaCodePtr);
    WetaCodePtr (*read_byte)(PSTORE, WetaCodePtr,  uint8_t*);
    WetaCodePtr (*read_bytes)(PSTORE, WetaCodePtr, uint8_t*, WetaCodePtr);

} StoreMethods;

struct _STORE
{
    WetaStorage   type;
    StoreMethods* methods;
};

#define INVALID_CODEPTR ((WetaCodePtr)~0)


/**	
 * @brief Open storage for writing.
 * 
 * Creates a handle that can only be used for write operations. Write operations
 * will be cached, and not actually written to flash until @ref weta_store_flush()
 * is called.
 * 
 * @param flashAddr Start address of flash memory to write to
 * @param length Maximum length of data to be written
 * @return Handle to be used in subsequent operations (@c NULL if failed)
 */
extern	bool weta_store_start_write(PSTORE store, WetaCodePtr startAddress, WetaCodePtr length);

/**
 * @brief Open flash storage for reading.
 * 
 * Creates a handle that can only be used for read operations.
 * 
 * @param flashAddrc Start address of flash memory to read
 * @return Handle to be used in subsequent read operations (@c NULL if failed)
 */
extern	bool weta_store_start_read(PSTORE store, WetaCodePtr startAddress);

/**
 * @brief Write previously cached data to flash memory.
 * @param hStore
 * @return Offset of flash memory following flushed data (@c INVALID_CODEPTR if failed)
 */
extern	bool weta_store_flush(PSTORE store);

/**
 * @brief Release resources used for this handle.
 * @param hStore
 */
extern	void weta_store_close(PSTORE store);

/**
 * @brief Write a single byte. 
 * @param hStore Handle returned by @ref weta_store_open_write()
 * @param val Byte value to write
 * @return Number of bytes written
 */
extern	WetaCodePtr weta_store_write_byte(PSTORE store, uint8_t val);

/**
 * @brief Write the given bytes.
 * @param hStore Handle returned by @ref weta_store_open_write()
 * @param buf Array of bytes to write
 * @param length Number of bytes to write
 * @return Number of bytes written
 */
extern	WetaCodePtr weta_store_write_bytes(PSTORE store, uint8_t* buf, WetaCodePtr length);

/**
 * @brief Write a uint16_t value in network byte order.
 * @param hStore Handle returned by @ref weta_store_open_write()
 * @param val Value to write
 * @return Number of bytes written
 */
extern	WetaCodePtr weta_store_write_uint16(PSTORE store, uint16_t val);

/**
 * @brief Write a uint32_t value in network byte order.
 * @param hStore Handle returned by @ref weta_store_open_write()
 * @param val Value to write
 * @return Number of bytes written
 */
extern	WetaCodePtr weta_store_write_uint32(PSTORE store, uint32_t val);

/**
 * @brief Write a float value in network byte order.
 * @param hStore Handle returned by @ref weta_store_open_write()
 * @param val Value to write
 * @return Offset to the data immediately following the data written
 */
extern	WetaCodePtr weta_store_write_float(PSTORE store, float val);

/**
 * @brief Write a double value in network byte order.
 * @param hStore Handle returned by @ref weta_store_open_write()
 * @param val Value to write
 * @return Number of bytes written
 */
extern	WetaCodePtr weta_store_write_double(PSTORE store, double val);

/**
 * @brief Read a single byte. 
 * @param hStore Handle returned by @ref weta_store_open_read()
 * @param Address Offset from the flash start address from which to read the byte.
 * @param pval Byte value to write
 * @return Offset to the data immediately following the byte written
 */
extern	WetaCodePtr weta_store_read_byte(PSTORE store, WetaCodePtr address,  uint8_t*pval);

/**
 * @brief Read the given number of bytes.
 * @param hStore Handle returned by @ref weta_store_open_read()
 * @param address Offset from the flash start address from which to read the bytes.
 * @param buf Buffer to place the bytes read
 * @param length Number of bytes to read
 * @return Offset to the data immediately following the data read
 */
extern	WetaCodePtr weta_store_read_bytes(PSTORE store, WetaCodePtr address, uint8_t* buf, WetaCodePtr length); 

/**
 * @brief Read a uint16_t value (stored in network byte order) and convert it to host byte order.
 * @param hStore Handle returned by @ref weta_store_open_read()
 * @param address address Offset from the flash start address fromt which to read the value.
 * @param pval Value read from flash
 * @return Offset to the data immediately following the data read
 */
extern	WetaCodePtr weta_store_read_uint16(PSTORE store, WetaCodePtr address, uint16_t* pval);

/**
 * @brief Read a uint32_t value (stored in network byte order) and convert it to host byte order.
 * @param hStore Handle returned by @ref weta_store_open_read()
 * @param address address Offset from the flash start address fromt which to read the value.
 * @param pval Value read from flash
 * @return Offset to the data immediately following the data read
 */
extern	WetaCodePtr weta_store_read_uint32(PSTORE store, WetaCodePtr address, uint32_t* pval);

/**
 * @brief Read a float value (stored in network byte order) and convert it to host byte order.
 * @param hStore Handle returned by @ref weta_store_open_read()
 * @param address address Offset from the flash start address fromt which to read the value.
 * @param pval Value read from flash
 * @return Offset to the data immediately following the data read
 */
extern	WetaCodePtr weta_store_read_float(PSTORE store, WetaCodePtr address,  float* pval);

/**
 * @brief Read a double value (stored in network byte order) and convert it to host byte order.
 * @param hStore Handle returned by @ref weta_store_open_read()
 * @param address address Offset from the flash start address fromt which to read the value.
 * @param pval Value read from flash,
 * @return Offset to the data immediately following the data read
 */
extern	WetaCodePtr weta_store_read_double(PSTORE store, WetaCodePtr address, double* pval);

extern	WetaCodePtr weta_store_read_string(PSTORE store, WetaCodePtr address, uint8_t* psz, uint8_t length);

#endif //__WETA_STORE_H__
