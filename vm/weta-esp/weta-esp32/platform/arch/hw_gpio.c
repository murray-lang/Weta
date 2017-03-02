/**
 * @file hw_gpio.c
 */
#include <hw.h>
#include <hw_time.h>
#include <driver/gpio.h>
#include <stdio.h>

void WETAFUNCATTR
hw_gpio_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;

	uint8_t i;
	for (i = 0; i < hw->gpio.n_pins; i++)
	{
        //DEBUGMSG("hw_gpio_init() pins[%d] = %d\r\n", i, gpio->pins[i].pin);

		gpio_pad_select_gpio(hw->gpio.pins[i].pin);

		gpio_set_direction((gpio_num_t)hw->gpio.pins[i].pin, (gpio_mode_t)hw->gpio.pins[i].dir);
        gpio_set_pull_mode((gpio_num_t)hw->gpio.pins[i].pin, (gpio_mode_t)hw->gpio.pins[i].pull);
        //if (gpio->pins[i].dir == GPIO_MODE_INPUT)
        //    gpio_pullup_en((gpio_num_t)gpio->pins[i].pin);

        if (hw->gpio.pins[i].debounce)
        {
            hw->gpio.pins[i].debounce->state     = false;
            hw->gpio.pins[i].debounce->candidate = false;
            hw->gpio.pins[i].debounce->debounced = false;
            hw->gpio.pins[i].debounce->lastTime  = 0;
        }
    }
    //DEBUGMSG("About to leave hw_gpio_init()\r\n");
}

/**
 * @brief Get the value of the given digital input.
 * @param [in] gpio Pointer to Gpio structure containing information on all gpios
 * @param [in] i    Index into the array of gpio information
 * @param [out] value   Boolean value of the digital input
 * @return true if a value was successfully obtained. Note that this is often
 * not based on any real checking.
 */
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
	*value = (gpio_get_level((gpio_num_t)pin->pin) != 0) != pin->invert;
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
	esp_err_t rc = gpio_set_level((gpio_num_t)pin->pin, value != pin->invert ? 1 : 0);
	return rc == ESP_OK;
}

bool WETAFUNCATTR
hw_gpio_pulse_i(Gpio* gpio, uint8_t i)
{
    if (i >= gpio->n_pins)
        return false;
    return hw_gpio_pulse(&gpio->pins[i]);
}

bool WETAFUNCATTR
hw_gpio_pulse(GpioPin* pin)
{
    esp_err_t rc1 = gpio_set_level((gpio_num_t)pin->pin, pin->invert ? 0 : 1);
    esp_err_t rc2 = gpio_set_level((gpio_num_t)pin->pin, pin->invert ? 1 : 0);
    return rc1 + rc2 == ESP_OK;
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
