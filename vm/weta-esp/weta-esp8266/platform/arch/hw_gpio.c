#include <weta_platform.h>
#include <hw.h>
#include <gpio16.h>

void WETAFUNCATTR
hw_gpio_init(struct _Hardware* hw, uint16_t flags)
{

	DEBUGMSG("hw_gpio_init() %d pins\r\n", hw->gpio.n_pins);
	flags = flags;

    gpio_init();

	uint8_t i;
	for (i = 0; i < hw->gpio.n_pins; i++)
	{
        PIN_FUNC_SELECT(GPIO_PIN_REG(hw->gpio.pins[i].pin), GET_GPIO_FUNC(hw->gpio.pins[i].pin));
        if (hw->gpio.pins[i].debounce)
		{
			hw->gpio.pins[i].debounce->state     = false;
			hw->gpio.pins[i].debounce->candidate = false;
			hw->gpio.pins[i].debounce->debounced = false;
			hw->gpio.pins[i].debounce->lastTime  = 0;
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
    *value = GPIO_INPUT_GET(GPIO_ID_PIN(pin->pin)) != pin->invert;
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
	DEBUGMSG("hw_gpio_set(%d, %d)\r\n", pin->pin, value);
    GPIO_OUTPUT_SET(GPIO_ID_PIN(pin->pin), value != pin->invert);

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

void WETAFUNCATTR
hw_gpio_clear_debounce(GpioPin *pin)
{
	if (!pin->debounce)
		return;

	pin->debounce->candidate = false;
	pin->debounce->debounced = false;
	pin->debounce->state     = false;
	pin->debounce->lastTime  = 0;
}
