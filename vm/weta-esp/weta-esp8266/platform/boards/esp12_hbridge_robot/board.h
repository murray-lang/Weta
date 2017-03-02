#ifndef __HBRIDGE_BOARD_H__
#define __HBRIDGE_BOARD_H__

#include <eagle_soc.h>
#include <gpio.h>


// hw_motors.h looks for this
//#define MOTORS_A_B_PWM
#define MOTORS_DIR_PWM

#define NUM_PWM_CHANNELS    2
//PWM period 10000us = 100Hz
#define PWM_PERIOD          10000

#define RUN_PIN              4
#define USER_LED_PIN         2
#define MOTOR_0_DIR_PIN     12
#define MOTOR_0_PWM_PIN     13
#define MOTOR_0_PWM_FUNC    FUNC_GPIO13
#define MOTOR_0_PWM_MUX     PERIPHS_IO_MUX_MTCK_U
#define MOTOR_1_DIR_PIN      5
#define MOTOR_1_PWM_PIN     14
#define MOTOR_1_PWM_FUNC    FUNC_GPIO14
#define MOTOR_1_PWM_MUX     PERIPHS_IO_MUX_MTMS_U

#define GPIO_INDEX_RUN 0
#define GPIO_INDEX_LED 1

#define WETA_SPORT         0
#define WETA_DEBUG_SPORT         1

#define STEPPER_WIDTH 2
#define STEPPER_STEPS 4

#define SUPPORT_GPIO
#define SUPPORT_DEBOUNCE
#define SUPPORT_PWM
#define SUPPORT_MOTORS
#define SUPPORT_H_BRIDGE
//#define SUPPORT_CRICKET
//#define SUPPORT_SERVOS
//#define SUPPORT_ADC
//#define SUPPORT_DAC


#define SUPPORT_QUERY
//#define SUPPORT_STEPPERS

#endif // __HBRIDGE_BOARD_H__