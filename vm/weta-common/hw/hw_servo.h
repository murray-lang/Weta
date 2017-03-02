#ifndef __HW_SERVO_H__
#define  __HW_SERVO_H__

#include <weta_platform.h>
#include <stdbool.h>
#include "hw_pwm.h"

struct _Hardware;

typedef struct
{
	uint16_t dutyMin;
	uint16_t duty0;	
	uint16_t dutyMax;
	int16_t  dutyPer10Degrees; // Signed!(+ve duty means -ve degrees (CCW)) 
} ServoConfig;	

typedef struct 
{
	PwmChannel* pwm;
    uint8_t     id;
	bool		selected;
    int16_t	    position;
    ServoConfig config;
} ServoState;

typedef struct
{
	ServoState* servos;
	uint8_t		n_servos;
} Servos;

extern void hw_servo_init(struct _Hardware* hw, uint16_t flags);
extern void hw_servo_select(Servos* servos, uint8_t select);
extern void hw_servo_update(Servos* servos);
extern void hw_servo_set_position(Servos* servos, int16_t position);

#endif // __HW_SERVO_H__