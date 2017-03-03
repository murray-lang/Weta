#ifndef __HW_MOTOR_H__
#define  __HW_MOTOR_H__

#include <weta_platform.h>
#include <stdbool.h>
#include "hw_gpio.h"
#include "hw_pwm.h"

struct _Hardware;

typedef enum 
{
	MOTOR_THIS_WAY,
	MOTOR_THAT_WAY
} MotorDirection;

typedef uint8_t MotorPower;

typedef struct 
{
	uint8_t        id		: 4;
    bool           selected : 1;
    bool           on	    : 1;
    bool           brake	: 1;
    MotorDirection dir		: 1;
	MotorPower     power    : sizeof(MotorPower)*8;
} MotorState;

typedef struct
{
#if defined(MOTORS_A_B_PWM)
    GpioPin*    a;
    GpioPin*    b;
#elif defined(MOTORS_DIR_PWM)
    GpioPin*    dir;
#endif
    PwmChannel* pwm;
} MotorPins;

typedef struct
{
    MotorPins   pins;
    MotorState  state;
} Motor;

typedef struct 
{
	Motor*      motors;
	uint8_t     n_motors;
} Motors;

extern void hw_motor_init(struct _Hardware* hw, uint16_t flags);
extern void hw_motor_select(Motors* motors, uint8_t select);
extern void hw_motor_update(Motors* motors);
extern void hw_motor_on(Motors* motors, bool on);
extern void hw_motor_all_off(Motors* motors);
extern void hw_motor_brake(Motors* motors, bool brake);
extern void hw_motor_direction(Motors* motors, MotorDirection dir);
extern void hw_motor_reverse(Motors* motors);
extern void hw_motor_power(Motors* motors, MotorPower power);

#endif // __HW_MOTOR_H__