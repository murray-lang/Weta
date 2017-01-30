#include "hw.h"

void WETAFUNCATTR 
hw_init(void)
{
}

void WETAFUNCATTR 
hw_init_specific(Hardware* hardware, uint16_t flags)
{
	hw_serial_init(&hardware->sports, flags);
	hw_gpio_init(&hardware->gpio, flags);
	hw_pwm_init(&hardware->pwms, flags);
	hw_adc_init(&hardware->adc, flags);
	//hw_dac_init(flags);
	hw_motor_init(flags);
	hw_stepper_init(&hardware->steppers,flags);
	hw_servo_init(flags);
	//hw_i2c_init(flags);
	//hw_buzzer_init(flags);

}
