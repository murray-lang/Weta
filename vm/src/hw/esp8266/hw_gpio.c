#include "../hw_gpio.h"
#include "hw_board.h"

void WETAFUNCATTR
hw_gpio_init(Gpio* gpio, uint16_t flags)
{
	flags = flags;
	gpio_init();
	//uint8_t i;
	//for (i = 0; i < gpio->n_pins; i++)
	//	stamp_gpio_init_pin(gpio->pins[i].port, gpio->pins[i].pin, gpio->pins[i].mode);
}

bool WETAFUNCATTR 
hw_gpio_get(Gpio* gpio, uint8_t i, bool* value)
{
	if (i >= gpio->n_pins)
		return false;
		
	return false; //stamp_get_gpio(gpio->pins[i].port, gpio->pins[i].pin, value);
}

bool WETAFUNCATTR 
hw_gpio_set(Gpio* gpio, uint8_t i, bool value)
{
	if (i >= gpio->n_pins)
		return false;
		
	return false; //stamp_set_gpio(gpio->pins[i].port, gpio->pins[i].pin, value);
}
