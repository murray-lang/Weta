#include <hw_gpio.h>
#include <hw_time.h>
#include <driver/gpio.h>
#include <stdio.h>

void WETAFUNCATTR
hw_gpio_init(Gpio* gpio, uint16_t flags)
{
	flags = flags;

	uint8_t i;
	for (i = 0; i < gpio->n_pins; i++)
	{
		gpio_pad_select_gpio(gpio->pins[i].pin);
		gpio_set_direction((gpio_num_t)gpio->pins[i].pin, (gpio_mode_t)gpio->pins[i].dir);
        gpio_set_pull_mode((gpio_num_t)gpio->pins[i].pin, (gpio_mode_t)gpio->pins[i].pull);
        //if (gpio->pins[i].dir == GPIO_MODE_INPUT)
        //    gpio_pullup_en((gpio_num_t)gpio->pins[i].pin);

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
		
	return hw_gpio_get(&gpio->pins[i], value);
}

bool WETAFUNCATTR
hw_gpio_get(GpioPin* pin, bool* value)
{
	*value = gpio_get_level((gpio_num_t)pin->pin) != 0;
		// ESP32 API has no error option!
	return true;
}

bool WETAFUNCATTR 
hw_gpio_set_i(Gpio* gpio, uint8_t i, bool value)
{
	if (i >= gpio->n_pins)
		return false;
	return hw_gpio_set(&gpio->pins[i], value);
}

bool WETAFUNCATTR
hw_gpio_set(GpioPin* pin, bool value)
{
	esp_err_t rc = gpio_set_level((gpio_num_t)pin->pin, value?1:0);
	return rc == ESP_OK;
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
