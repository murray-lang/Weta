//
// Created by murray on 13/01/17.
//

#ifndef VM_MYNANO32_BOARD_H
#define VM_MYNANO32_BOARD_H

//#define FLASH_USER_START       0x110000
#define FLASH_USER_START       0x0B0000
#define FLASH_USER_LENGTH        0x1000
#define FLASH_CONFIG_START     (FLASH_USER_START + FLASH_USER_LENGTH)
#define FLASH_CONFIG_LENGTH     FLASH_USER_LENGTH

#define FLASH_ERASE_BLOCK_SIZE 4096
#define FLASH_BLOCK_SIZE       128
#define FLASH_ALIGN			   4

#define MAX_UARTS        1
#define MAX_GPIO        20
#define MAX_PWM          8
#define MAX_PWM_TIMERS   4
#define MAX_MOTORS      MAX_PWM
#define MAX_SERVOS      MAX_PWM
#define MAX_ADC         8
#define MAX_DAC         2

#define MAX_SHIFTERS 4
#define MAX_SHIFTER_WIDTH 32
#define MAX_STEPPERS (MAX_SHIFTERS*2)

#define GPIO_INDEX_LED               0
#define GPIO_INDEX_RUN               1   // Index into the GPIO array of the run button
#define GPIO_INDEX_SWITCH1           2
#define GPIO_INDEX_SWITCH2           3
#define GPIO_INDEX_SWITCH3           4
#define GPIO_INDEX_SWITCH4           5
#define GPIO_INDEX_MOTOR_0_DIR       6
#define GPIO_INDEX_MOTOR_1_DIR       7
#define GPIO_INDEX_STEPPER_CLOCK	 8
#define GPIO_INDEX_STEPPER_DATA		 9
#define GPIO_INDEX_STEPPER_STROBE	10


#define PIN_RUN 		14

#define PIN_UART1_TX    12
#define PIN_UART1_RX    13

#define PIN_SWITCH1		32
#define PIN_SWITCH2		33
#define PIN_SWITCH3		34
#define PIN_SWITCH4		35


//#define PIN_LED1		12
//#define PIN_LED2		13
//#define PIN_LED3		14
//#define PIN_LED4		27

#define PIN_USER_LED		16

#define PIN_MOTOR_0_DIR		23
#define PIN_MOTOR_0_PWM		19
#define PIN_MOTOR_1_DIR		05
#define PIN_MOTOR_1_PWM		18

#define PIN_STEPPER_CLOCK   02
#define PIN_STEPPER_DATA    04
#define PIN_STEPPER_STROBE  17

#define PIN_BUZZER          10

#define PIN_SERVO_0_PWM     25
#define PIN_SERVO_1_PWM     26

#define STEPPER_STEPS 4
//#define STEPPER_WIDTH 2
#define STEPPER_WIDTH 4

#define WETA_SPORT         0
#define WETA_DEBUG_SPORT         0

#define SUPPORT_GPIO
#define SUPPORT_PWM
#define SUPPORT_MOTORS
#define SUPPORT_H_BRIDGE
#define SUPPORT_STEPPERS
#define SUPPORT_SERVOS
#define SUPPORT_ADC
#define SUPPORT_DAC
#define SUPPORT_SHIFTERS

// hw_motors.h looks for this
//#define MOTORS_A_B_PWM
//#define MOTORS_DIR_PWM

// hw_stepper.h looks for this
//#define STEPPERS_USE_SHIFTER



#endif //VM_MYNANO32_BOARD_H
