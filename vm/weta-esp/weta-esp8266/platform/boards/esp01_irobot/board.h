#ifndef __ESP01_IROBOT_NONOS_BOARD_H__
#define __ESP01_IROBOT_NONOS_BOARD_H__

#include <eagle_soc.h>
//#include <gpio.h>

#define FLASH_USER_START       0x47000
#define FLASH_USER_LENGTH      4096
#define FLASH_ERASE_BLOCK_SIZE 4096

#define USER_LED_PIN        1
#define RUN_PIN             2

#define WETA_SPORT         0
#define WETA_DEBUG_SPORT         0

#define SUPPORT_QUERY

//#define SUPPORT_GPIO
//#define SUPPORT_PWM
//#define SUPPORT_MOTORS
//#define SUPPORT_H_BRIDGE
//#define SUPPORT_SERVOS
//#define SUPPORT_STEPPERS
//#define SUPPORT_ADC
//#define SUPPORT_DAC
#define SUPPORT_IROBOT


#endif // __ESP01_IROBOT_NONOS_BOARD_H__
