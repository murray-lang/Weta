#ifndef __WETA_PLATFORM_H__
#define __WETA_PLATFORM_H__

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include  <sdkconfig.h>

// ESP32 compiler puts code in flash by default, so WETAFUNCATTR empty
#define WETAFUNCATTR

#define PWM_FREQ 100
// PWM resolution set to 15 bits in hw_pwm.c
#define PWM_DUTY_RESOLUTION 32767

// Leave the following undefined if motors have a single direction pin
//#define MOTORS_ARE_H_BRIDGE

#define weta_malloc	malloc
#define weta_free	free
#define weta_sprintf sprintf

typedef uint32_t WetaFlashPtr;
typedef uint32_t WetaFlashFlags;

/**
 * @brief The type of a byte code address.
 */

typedef uint16_t	WetaCodePtr;
typedef int16_t		WetaStackPtr;
/**
 * @brief Depending on the platform, a pin might have to be identified with a port
 */
typedef uint8_t		WetaPin;

typedef uint32_t WetaGpioMode;

typedef uint8_t  WetaPwmChannel;
typedef uint16_t  WetaPwmDuty;

typedef uint8_t     WetaAdcChannel;
typedef uint8_t     WetaAdcDepth;
typedef uint8_t     WetaAdcAtten;

typedef uint8_t WetaDebounceFlags;


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

#define DEBOUNCE_TICKS 25

#define SUPPORT_32BIT
#define SUPPORT_DOUBLE
#define SUPPORT_FLOAT
#define SUPPORT_STRING
#define SUPPORT_JSON
#define SUPPORT_QUERY

#define FORCE_STACK_BYTE_ALIGNED

#include <../boards/board.h>
/*
#if defined(CONFIG_TARGET_BOARD_MYNANO32)
#include <../boards/mynano32/board.h>
#elif defined(CONFIG_TARGET_BOARD_CARLOS)
#include <../boards/carlos/board.h>
#endif
*/
 
#endif //__WETA_PLATFORM_H__
