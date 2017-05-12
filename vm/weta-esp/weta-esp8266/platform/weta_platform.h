#ifndef __WETA_PLATFORM_H__
#define __WETA_PLATFORM_H__

#include <c_types.h>
#include <stdbool.h>

#define COOPERATIVE_RTOS
#define MILLIS_PER_TICK 1000

#define FLASH_USER_START       0x4C000
#define FLASH_USER_LENGTH      4096
#define FLASH_ERASE_BLOCK_SIZE 4096
#define FLASH_BLOCK_SIZE       128
#define FLASH_ALIGN			   4

#define WETA_STACK_SIZE 256
#define WETA_MAX_STACKS 1		// ie only 1 VM at a time

#define DEFAULT_FORMAT_BYTE  "%02x"
#define DEFAULT_FORMAT_INT   "%d"
#define DEFAULT_FORMAT_FLOAT "%g"

#define WETAFUNCATTR __attribute__((section(".irom0.text")))

typedef uint32_t    WetaTimestamp;

typedef uint32_t	WetaFlashPtr;
typedef uint32_t	WetaFlashFlags;

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

#define BOOT_STORAGE STORAGE_FLASH

#define DEBOUNCE_TICKS 70000

#define MAX_PWM_DUTY        ((PWM_PERIOD*1000)/45)

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

#define weta_strcat os_strcat
#define weta_strchr os_strchr
#define weta_strcmp os_strcmp
#define weta_strcpy os_strcpy
#define weta_strlen os_strlen
#define weta_strncmp os_strncmp
#define weta_strncpy os_strncpy
#define weta_strstr os_strstr

#define weta_malloc	os_malloc
#define weta_free	os_free

#ifdef DEBUG
#define DEBUGMSG(...) weta_printf(__VA_ARGS__)
#else
#define DEBUGMSG(...)
#endif

#define weta_beep(hw)                               hw_buzzer_beep(hw)
#define weta_led(hw, on)                            hw_gpio_set_i(&(hw)->gpio, GPIO_INDEX_LED, on)
#define weta_motor_select(hw, sel)                  hw_motor_select(&(hw)->motors, sel)
#define weta_motor_dir(hw, dir)                     hw_motor_direction(&(hw)->motors, dir)
#define weta_motor_rd(hw)                           hw_motor_reverse(&(hw)->motors)
#define weta_motor_power(hw, pow)                   hw_motor_power(&(hw)->motors, pow)
#define weta_motor_brake(hw, yep)                   hw_motor_brake(&(hw)->motors, yep)
#define weta_motor_on(hw, on)                       hw_motor_on(&(hw)->motors, on)
#define weta_servo_select(hw, sel)                  hw_servo_select(&(hw)->servos, sel)
#define weta_servo_pos(hw, dir)                     hw_servo_set_position(&(hw)->servos, dir)
#define weta_adc_get(hw, i, val)                    hw_adc_get(&(hw)->adc, i, val)
#define weta_dac_set(hw, i, val)                    hw_dac_set(&(hw)->adc, i, val)
#define weta_digital_in(hw, i, val)                 hw_gpio_get_i(&(hw)->gpio, i, val)
#define weta_digital_out(hw, i, val)                hw_gpio_set_i(&(hw)->gpio, i, val)
#define weta_serial_available(hw, i)                hw_serial_available_i(&(hw)->sports, i)
#define weta_serial_read(hw, i, buf, len, timeout)  hw_serial_read_i(&(hw)->sports, i, buf, len, timeout)
#define weta_serial_write(hw, i, buf, len)          hw_serial_write_i(&(hw)->sports, i, buf, len)
#define weta_i2c_start(hw)                          hw_i2c_start(hw)
#define weta_i2c_stop(hw)                           hw_i2c_stop(hw)
#define weta_i2c_write(hw, s, r, w, buf, n)         hw_i2c_write(hw, s, r, w, buf, n)
#define weta_i2c_read(hw, s, r, w, buf, n)          hw_i2c_read(hw, s, r, w, buf, n)


#include "boards/board.h"

#endif //__WETA_PLATFORM_H__
