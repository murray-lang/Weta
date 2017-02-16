#include <weta_platform.h>
#include <hw_gpio.h>
#include <hw_time.h>
#include "hw_defs.h"
#include <gpio16.h>

void WETAFUNCATTR
hw_gpio_init(Gpio* gpio, uint16_t flags)
{
	flags = flags;
	uint8_t i;
	for (i = 0; i < gpio->n_pins; i++)
	{
			// Special configuration of GPIO16
		if (gpio->pins[i].pin == 16)
        {
            if (gpio->gpio16dir == GPIO16_IN)
                gpio16_input_conf();
            else
                gpio16_output_conf();
        }
		if (gpio->pins[i].debounce)
		{
			gpio->pins[i].debounce->state     = false;
			gpio->pins[i].debounce->candidate = false;
			gpio->pins[i].debounce->debounced = false;
			gpio->pins[i].debounce->lastTime  = 0;
		}
	}
}

bool WETAFUNCATTR 
hw_gpio_get_i(Gpio* gpio, uint8_t i, bool* value)
{
	if (i >= gpio->n_pins)
		return false;
    hw_gpio_get(&gpio->pins[i], value);
	return true;
}

bool WETAFUNCATTR
hw_gpio_get(GpioPin* pin, bool* value)
{
    if (pin->pin == 16)
        *value = !!gpio16_input_get() != pin->invert;
    else
        *value = !!(gpio_input_get() & pin->pin) != pin->invert;
    // no error option!
    return true;
}

bool WETAFUNCATTR 
hw_gpio_set_i(Gpio* gpio, uint8_t i, bool value)
{
	if (i >= gpio->n_pins)
		return false;
    hw_gpio_set(&gpio->pins[i], value);
	return true;
}

bool WETAFUNCATTR
hw_gpio_set(GpioPin* pin, bool value)
{

    if (pin->pin == 16)
        gpio16_output_set((uint8_t)(value != pin->invert));
    else
        GPIO_OUTPUT_SET(pin->pin, value != pin->invert);

    return true;
}

void WETAFUNCATTR
hw_gpio_debounce(Gpio* gpio)
{
	uint8_t i;
	for (i = 0; i < gpio->n_debounced_pins; i++)
	{
		GpioPin *pin = gpio->debounced_pins[i];
		DebounceState *debounce = pin->debounce;
		// If latched and debounced then do nothing (until cleared)
		if (debounce->latched)
		if (debounce->debounced)
		if (debounce->state)
			continue;
		// Get the current state at the current time
		uint32_t thisTime = hw_time_ticks();
		bool rawState;
		hw_gpio_get(pin, &rawState);
		rawState = !rawState;       // Reverse logic (low = pressed)
		if (debounce->debounced) {
			// It was debounced. A state change means that it is no longer
			// debounced.
			if (rawState != debounce->state)
			{
				debounce->debounced = false;
				debounce->lastTime = 0;
			}
		}
		// If the state has changed, or we're starting afresh...
		if (rawState != debounce->candidate
			|| debounce->lastTime == 0)
		{
			//...record the latest details
			debounce->lastTime = thisTime;
			debounce->candidate = rawState;
		}
		// If lastTime hasn't changed for a while then neither has the
		// candidate state
		if ((thisTime - debounce->lastTime) > DEBOUNCE_TICKS)
		{
			debounce->state = debounce->candidate;
			debounce->debounced = true;
			debounce->lastTime = 0;
		}
	}
}
