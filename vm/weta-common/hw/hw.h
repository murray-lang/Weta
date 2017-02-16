#ifndef __HW_H__
#define  __HW_H__

#include "hw_endian.h"
#include "hw_serial.h"
#ifdef SUPPORT_MOTORS
#include "hw_motor.h"
#endif
#ifdef	SUPPORT_STEPPERS
#include "hw_stepper.h"
#endif
#ifdef	SUPPORT_SERVOS
#include "hw_servo.h"
#endif
#ifdef	SUPPORT_GPIO
#include "hw_gpio.h"
#endif
#ifdef	SUPPORT_PWM
#include "hw_pwm.h"
#endif
#ifdef	SUPPORT_ADC
#include "hw_adc.h"
#endif
#ifdef	SUPPORT_DAC
#include "hw_dac.h"
#endif
#include "hw_i2c.h"
#include "hw_buzzer.h"
#include "hw_time.h"

typedef struct _Hardware
{
	uint8_t*    flash;
	WetaCodePtr flashLength;
	SerialPorts sports;
#ifdef SUPPORT_MOTORS
	Motors      motors;
#endif
#ifdef	SUPPORT_STEPPERS
	Steppers    steppers;
#endif
#ifdef	SUPPORT_SERVOS
	Servos      servos;
#endif
#ifdef	SUPPORT_GPIO
	Gpio        gpio;
#endif
#ifdef	SUPPORT_PWM
	PwmChannels pwms;
#endif
#ifdef	SUPPORT_ADC
	Adc			adc;
#endif
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
