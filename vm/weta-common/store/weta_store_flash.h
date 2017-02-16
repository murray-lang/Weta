#ifndef __WETA_STORE_FLASH_H__
#define __WETA_STORE_FLASH_H__
/** 
 * @file weta_store.h
 * @author Murray Lang
 * @brief Functions for accessing byte codes in flash memory.
 */

#include <weta_platform.h>
#include "weta_store.h"

extern	PSTORE weta_store_init_flash(uint8_t* flashStart, WetaCodePtr length);

#endif //__WETA_STORE_FLASH_H__
