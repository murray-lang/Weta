//
// Created by murray on 13/01/17.
//

#ifndef VM_ARDUINO_ADK_BOARD_H
#define VM_ARDUINO_ADK_BOARD_H

#include "Arduino.h"

#define FLASH_USER_START       0x110000
#define FLASH_USER_LENGTH      0x1000
#define FLASH_ERASE_BLOCK_SIZE SPM_PAGESIZE
#define FLASH_BLOCK_SIZE       SPM_PAGESIZE
#define FLASH_ALIGN			   4

#define LED_ON_VALUE                 false
#define LED_OFF_VALUE                true
#define GPIO_INDEX_LED               8
#define GPIO_INDEX_RUN               5
#define GPIO_INDEX_SWITCH1           2
#define GPIO_INDEX_SWITCH2           3
#define GPIO_INDEX_SWITCH3           4
#define GPIO_INDEX_SWITCH4           5
#define GPIO_INDEX_MOTOR_0_DIR       6
#define GPIO_INDEX_MOTOR_1_DIR       7
#define GPIO_INDEX_STEPPER_0_A		 8
#define GPIO_INDEX_STEPPER_0_B		 9
#define GPIO_INDEX_STEPPER_0_C		10
#define GPIO_INDEX_STEPPER_0_D		11
#define GPIO_INDEX_STEPPER_1_A		12
#define GPIO_INDEX_STEPPER_1_B		13
#define GPIO_INDEX_STEPPER_1_C		14
#define GPIO_INDEX_STEPPER_1_D		15

#define PIN_SERVO_0_PWM     9
#define PIN_SERVO_1_PWM     10

#define PIN_RUN 		14

#define PIN_UART1_TX    10
#define PIN_UART1_RX     9

#define PIN_SWITCH1		32
#define PIN_SWITCH2		33
#define PIN_SWITCH3		34
#define PIN_SWITCH4		35

/*
#define PIN_USER_LED		16

#define PIN_MOTOR_0_DIR		3
#define PIN_MOTOR_0_PWM		27
#define PIN_MOTOR_1_DIR		22
#define PIN_MOTOR_1_PWM		23

#define PIN_STEPPER_0_A		 2
#define PIN_STEPPER_0_B		19
#define PIN_STEPPER_0_C		21
#define PIN_STEPPER_0_D		18
#define PIN_STEPPER_1_A		17
#define PIN_STEPPER_1_B		15
#define PIN_STEPPER_1_C		5
#define PIN_STEPPER_1_D		4
*/
#define STEPPER_STEPS 4
#define STEPPER_WIDTH 4


#endif //VM_ARDUINO_ADK_BOARD_H
