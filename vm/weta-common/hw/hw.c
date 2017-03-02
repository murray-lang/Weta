#include "hw.h"
//#include <driver/ledc.h>    // for debugging only

void WETAFUNCATTR 
hw_init(void)
{
}

//ledc_timer_config_t blahdy;

void WETAFUNCATTR 
hw_init_specific(Hardware* pHardware, uint16_t flags)
{

	DEBUGMSG("hw_init_specific()\r\n");
	hw_serial_init(pHardware, flags);

#ifdef SUPPORT_GPIO
	DEBUGMSG("hw_gpio_init()\r\n");
    //uint8_t blah = pHardware->gpio.pins[0].pin;
    //DEBUGMSG("First GPIO pin is %d\r\n", blah);
	hw_gpio_init(pHardware, flags);
#endif

#ifdef SUPPORT_PWM

    //ledc_timer_config_t config;
    //config.speed_mode = LEDC_HIGH_SPEED_MODE;
    //blahdy.speed_mode = config.speed_mode;
    DEBUGMSG("hw_pwm_init()\r\n");
	hw_pwm_init(pHardware, flags);
#endif

	hw_buzzer_init(pHardware,flags);

#ifdef SUPPORT_ADC
	DEBUGMSG("hw_adc_init()\r\n");
	hw_adc_init(pHardware, flags);
#endif
#ifdef SUPPORT_DAC
	hw_dac_init(pHardware, flags);
#endif
#ifdef SUPPORT_MOTORS
	DEBUGMSG("hw_motor_init()\r\n");
	hw_motor_init(pHardware, flags);
#endif
#ifdef	SUPPORT_STEPPERS
	DEBUGMSG("hw_stepper_init()\r\n");
	hw_stepper_init(pHardware,flags);
#endif
#ifdef SUPPORT_SERVOS
	DEBUGMSG("hw_servo_init()\r\n");
	hw_servo_init(pHardware,flags);
#endif
#ifdef SUPPORT_I2C
	hw_i2c_init(pHardware, flags);
#endif

}
