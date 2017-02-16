#ifndef __WETA_STORE_RAM_H__
#define __WETA_STORE_RAM_H__
/** 
 * @file weta_store.h
 * @author Murray Lang
 * @brief Functions for accessing byte codes in flash memory.
 */

#include <weta_platform.h>
#include "weta_store.h"

extern	PSTORE weta_store_init_ram(WetaCodePtr length);

#endif //__WETA_STORE_RAM_H__
