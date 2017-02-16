#ifndef __WETA_PLATFORM_H__
#define __WETA_PLATFORM_H__

#include <c_types.h>
#include <stdbool.h>

#define FLASH_BLOCK_SIZE       128
#define FLASH_ALIGN			   4

#define WETAFUNCATTR __attribute__((section(".irom0.text")))

typedef uint32	WetaFlashPtr;
typedef uint32	WetaFlashFlags;

/**
 * @brief The type of a byte code address.
 */

typedef uint16_t	WetaCodePtr;
typedef int16_t		WetaStackPtr;


/**
 * @brief define the platform-specific function for reading stack pointers.
 */
#define weta_store_read_stackptr(sto,i,val)  weta_store_read_uint16(sto, (i), (uint16_t*)(val))

/**
 * @brief define the platform-specific function for reading code pointers.
 */
#define weta_store_read_codePtr weta_store_read_uint16

#define weta_stack_pushCodePtr weta_stack_pushUint16
#define weta_stack_popCodePtr  weta_stack_popUint16
#define weta_stack_topCodePtr  weta_stack_topUint16
#define weta_stack_setCodePtr  weta_stack_setUint16
#define weta_stack_getCodePtr  weta_stack_getUint16

#define DEBOUNCE_TICKS 15

#define SUPPORT_32BIT
//#define SUPPORT_DOUBLE
//#define SUPPORT_FLOAT
#define SUPPORT_STRING

#define SUPPORT_JSON

#define FORCE_STACK_BYTE_ALIGNED

#define WETA_WIFI_MODE_AP
//#define WETA_WIFI_MODE_STATION

#include "arch/espmissingincludes.h"
#include <mem.h>
#include <osapi.h>
#include "arch/hw_defs.h"
#define weta_printf os_printf
#define weta_sprintf os_sprintf


#define weta_malloc	os_malloc
#define weta_free	os_free

#ifdef DEBUG
#define DEBUGMSG(...) weta_printf(__VA_ARGS__)
#else
#define DEBUGMSG(...)
#endif

#include "boards/board.h"

#endif //__WETA_PLATFORM_H__
