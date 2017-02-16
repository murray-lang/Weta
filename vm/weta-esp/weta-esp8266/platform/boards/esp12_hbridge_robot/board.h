#ifndef __HBRIDGE_BOARD_H__
#define __HBRIDGE_BOARD_H__

#include <eagle_soc.h>
#include <gpio.h>

#define FLASH_USER_START       0x48000
#define FLASH_USER_LENGTH      4096
#define FLASH_ERASE_BLOCK_SIZE 4096

// hw_motors.h looks for this
#define MOTORS_ARE_H_BRIDGE

#define NUM_PWM_CHANNELS 2

#define RUN_PIN             2
#define MOTOR_0_DIRA_PIN    14
#define MOTOR_0_DIRB_PIN    13
#define MOTOR_0_PWM_PIN     12
#define MOTOR_0_PWM_FUNC    FUNC_GPIO12
#define MOTOR_1_DIRA_PIN    4
#define MOTOR_1_DIRB_PIN    5
#define MOTOR_1_PWM_PIN     0
#define MOTOR_1_PWM_FUNC    FUNC_GPIO0

#define GPIO_INDEX_RUN 0

#define WETA_SPORT         0
#define WETA_DEBUG_SPORT         1

#define STEPPER_WIDTH 2
#define STEPPER_STEPS 4

#define SUPPORT_GPIO
#define SUPPORT_PWM
#define SUPPORT_MOTORS
#define SUPPORT_H_BRIDGE
//#define SUPPORT_SERVOS
//#define SUPPORT_ADC
//#define SUPPORT_DAC


#define SUPPORT_QUERY
//#define SUPPORT_STEPPERS

#endif // __HBRIDGE_BOARD_H__