#ifndef __HW_H__
#define  __HW_H__

#include "hw_endian.h"
#include "hw_serial.h"
#include "hw_motor.h"
#include "hw_stepper.h"
#include "hw_servo.h"
#include "hw_gpio.h"
#include "hw_pwm.h"
#include "hw_adc.h"
#include "hw_dac.h"
#include "hw_i2c.h"
#include "hw_buzzer.h"
#include "hw_time.h"

typedef struct _Hardware
{
	uint8_t*    code;
	WetaCodePtr codeLength;
	SerialPorts sports;
	Motors      motors;
	Steppers    steppers;
	Servos      servos;
	Gpio        gpio;
	PwmChannels pwms;
	Adc			adc;
} Hardware;

#ifdef __cplusplus
extern "C" {
#endif

extern void hw_init(void);
extern void hw_init_specific(Hardware *hardware, uint16_t flags);

#ifdef __cplusplus
};
#endif

#endif // __HW_H__
