#include "hw.h"

void WETAFUNCATTR 
hw_init(void)
{
}

void WETAFUNCATTR 
hw_init_specific(Hardware* hardware, uint16_t flags)
{
	hw_serial_init(&hardware->sports, flags);
#ifdef SUPPORT_GPIO
	hw_gpio_init(&hardware->gpio, flags);
#endif
#ifdef SUPPORT_PWM
	hw_pwm_init(&hardware->pwms, flags);
#endif
#ifdef SUPPORT_ADC
	hw_adc_init(&hardware->adc, flags);
#endif
	//hw_dac_init(flags);
#ifdef SUPPORT_MOTORS
	hw_motor_init(flags);
#endif
#ifdef	SUPPORT_STEPPERS
	hw_stepper_init(&hardware->steppers,flags);
#endif
#ifdef SUPPORT_SERVOS
	hw_servo_init(flags);
#endif
	//hw_i2c_init(flags);
	//hw_buzzer_init(flags);

}
