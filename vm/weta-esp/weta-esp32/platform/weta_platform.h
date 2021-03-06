#ifndef __WETA_PLATFORM_H__
#define __WETA_PLATFORM_H__

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include  <sdkconfig.h>

#define WETA_STACK_SIZE 256
#define WETA_MAX_STACKS 1		// ie only 1 VM at a time

#define DEFAULT_FORMAT_BYTE  "%02x"
#define DEFAULT_FORMAT_INT   "%d"
#define DEFAULT_FORMAT_FLOAT "%g"

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
#define weta_printf printf

#define weta_strcat strcat
#define weta_strchr strchr
#define weta_strcmp strcmp
#define weta_strcpy strcpy
#define weta_strlen strlen
#define weta_strncmp strncmp
#define weta_strncpy strncpy
#define weta_strstr strstr

#define DEBUG
#ifdef DEBUG
#define DEBUGMSG(...) weta_printf(__VA_ARGS__)/*,fflush(stdout)*/
#else
#define DEBUGMSG(...)
#endif

typedef uint32_t WetaTimestamp;
typedef uint32_t WetaFlashPtr;
typedef uint32_t WetaFlashFlags;
typedef uint32_t WetaDebounceCounter;

typedef uint8_t  WetaPin;
typedef uint32_t WetaGpioMode;
typedef uint8_t  WetaPwmChannel;
typedef uint8_t  WetaPwmDuty;
typedef uint8_t  WetaAdcChannel;
typedef uint8_t  WetaAdcDepth;
    // Although ESP32 only has attenuation, the generic configuration uses
    // gain. So use gain hear so that it matches the expectation for
    // setting and querying configurations. Note signed (-ve for attenuation).
typedef int8_t   WetaAdcGain;

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

#define SUPPORT_DEBOUNCE
#define DEBOUNCE_TICKS 14000000
#define MILLIS_PER_TICK 1000

#define SUPPORT_32BIT
#define SUPPORT_DOUBLE
#define SUPPORT_FLOAT
#define SUPPORT_STRING
#define SUPPORT_JSON

#define SUPPORT_QUERY
//#define SUPPORT_CRICKET

#define FORCE_STACK_BYTE_ALIGNED

#define weta_beep(hw)                               hw_buzzer_beep(hw)
#define weta_motor_select(hw, sel)                  hw_motor_select(hw, sel)
#define weta_motor_dir(hw, dir)                     hw_motor_direction(hw, dir)
#define weta_motor_rd(hw)                           hw_motor_reverse(hw)
#define weta_motor_power(hw, pow)                   hw_motor_power(hw, pow)
#define weta_motor_brake(hw, yep)                   hw_motor_brake(hw, yep)
#define weta_motor_on(hw, on)                       hw_motor_on(hw, on)
#define weta_servo_select(hw, sel)                  hw_servo_select(hw, sel)
#define weta_servo_pos(hw, dir)                     hw_servo_set_position(hw, dir)
#define weta_servo_left(hw, amt)                    hw_servo_left(hw, amt)
#define weta_servo_right(hw, amt)                   hw_servo_right(hw, amt)
#define weta_adc_get(hw, i, val)                    hw_adc_get(hw, i, val)
#define weta_dac_set(hw, i, val)                    hw_dac_set(hw, i, val)
#define weta_digital_in(hw, i, val)                 hw_gpio_get(hw, i, val)
#define weta_digital_out(hw, i, val)                hw_gpio_set(hw, i, val)
#define weta_serial_available(hw, i)                hw_serial_available_i((hw)->sports, i)
#define weta_serial_read(hw, i, buf, len, timeout)  hw_serial_read_i((hw)->sports, i, buf, len, timeout)
#define weta_serial_write(hw, i, buf, len)          hw_serial_write_i((hw)->sports, i, buf, len)
#define weta_i2c_start(hw)                          hw_i2c_start(hw)
#define weta_i2c_stop(hw)                           hw_i2c_stop(hw)
#define weta_i2c_write(hw, s, r, w, buf, n)         hw_i2c_write(hw, s, r, w, buf, n)
#define weta_i2c_read(hw, s, r, w, buf, n)          hw_i2c_read(hw, s, r, w, buf, n)

#include "boards/board.h"

#endif //__WETA_PLATFORM_H__
