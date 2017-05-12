//
// Created by murray on 18/03/17.
//
#include "hw.h"


bool WETAFUNCATTR
hw_gpio_get(struct _Hardware* hw, uint8_t i, bool *value)
{
    if (i >= MAX_GPIO)
        return false;

    if (hw->gpio[i].options & GPIO_OPT_DEBOUNCE) {
        return hw_gpio_get_debounced(&hw->gpio[i], value);
    } else {
        return hw_gpio_get_raw(&hw->gpio[i], value);
    }

}

bool WETAFUNCATTR
hw_gpio_get_debounced(GpioPin* gpio, bool *value)
{
    if (gpio->debounce.debounced) {
        *value = gpio->debounce.state;
        return true;
    }
    return false;
}

void WETAFUNCATTR
hw_gpio_debounce(struct _Hardware *hw)
{
    uint8_t i;
    for (i = 0; i < MAX_GPIO; i++) {
        if (hw->gpio[i].pin == 0xFF) {
            continue;
        }
        if (!(hw->gpio[i].options & GPIO_OPT_DEBOUNCE)) {
            continue;
        }
        GpioPin *pin = &hw->gpio[i];
        DebounceState *debounce = &pin->debounce;
        // If latched and debounced then do nothing (until cleared)
        if (debounce->latched) {
            if (debounce->debounced) {
                if (debounce->state) {
                    continue;
                }
            }
        }
        // Get the current state at the current time
        uint32_t thisTime = hw_time_ticks();
        bool rawState;
        hw_gpio_get_raw(pin, &rawState);

        if (debounce->debounced) {
            // It was debounced. A state change means that it is no longer
            // debounced.
            if (rawState != debounce->state) {
                debounce->debounced = false;
                debounce->lastCount = 0;
            }
        }
        // If the state has changed, or we're starting afresh...
        if (rawState != debounce->candidate || debounce->lastCount == 0) {
            //...record the latest details
            debounce->lastCount = thisTime;
            debounce->candidate = rawState;
        }
        // If lastTime hasn't changed for a while then neither has the
        // candidate state
        if ((thisTime - debounce->lastCount) > DEBOUNCE_TICKS) {
            debounce->state = debounce->candidate;
            debounce->debounced = true;
            debounce->lastCount = 0;
        }
    }
}

void WETAFUNCATTR
    hw_gpio_clear_debounce(struct _Hardware* hw, uint8_t gpio_num)
{
    if (gpio_num >= MAX_GPIO)
        return;
    hw->gpio[gpio_num].debounce.candidate = false;
    hw->gpio[gpio_num].debounce.debounced = false;
    hw->gpio[gpio_num].debounce.state = false;
    hw->gpio[gpio_num].debounce.lastCount = 0;
}
