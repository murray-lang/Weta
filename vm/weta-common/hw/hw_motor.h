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
	bool           selected : 1;
    bool           on	    : 1;
    bool           brake	: 1;
    MotorDirection dir		: 1;
    uint8_t        unused   : 4;
	MotorPower     power    : sizeof(MotorPower)*8;
} MotorState;

typedef struct
{
    union
    {
        uint8_t a;      // Use this if full h-bridge control
        uint8_t dir;    // Use this if single direction pin
    };
    uint8_t         b;  // If this is 0xFF the use dir pin only
    WetaPwmChannel  pwm;
} MotorPins;

typedef struct
{
    MotorPins   pins;
    MotorState  state;
} Motor;

typedef Motor Motors[MAX_MOTORS];

extern void hw_motor_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_motor_config(
	struct _Hardware* hw,
	uint8_t motor,
	uint8_t pwm,
	uint8_t a_or_dir,
	uint8_t b
);
extern void hw_motor_select(struct _Hardware* hw, uint8_t select);
extern void hw_motor_update(struct _Hardware* hw);
extern void hw_motor_on(struct _Hardware* hw, bool on);
extern void hw_motor_all_off(struct _Hardware* hw);
extern void hw_motor_brake(struct _Hardware* hw, bool brake);
extern void hw_motor_direction(struct _Hardware* hw, MotorDirection dir);
extern void hw_motor_reverse(struct _Hardware* hw);
extern void hw_motor_power(struct _Hardware* hw, MotorPower power);

#endif // __HW_MOTOR_H__