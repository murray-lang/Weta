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
	WetaPwmChannel 	pwm;
	bool			selected;
    int16_t	    	position;
    ServoConfig 	config;
} ServoState;

typedef ServoState Servos[MAX_SERVOS];

extern void hw_servo_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_servo_config(
	struct _Hardware* hw,
	uint8_t servo,
	uint8_t pwm,
	uint16_t minduty,
	uint16_t maxduty,
	uint16_t duty0,
	int16_t dutyper10	// Note signed
);
extern void hw_servo_select(struct _Hardware* hw, uint8_t select);
extern void hw_servo_set_position(struct _Hardware* hw, int16_t position);
extern void hw_servo_left(struct _Hardware* hw, int16_t amount);
extern void hw_servo_right(struct _Hardware* hw, int16_t amount);

#endif // __HW_SERVO_H__