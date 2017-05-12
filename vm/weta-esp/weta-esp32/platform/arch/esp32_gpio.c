/**
 * @file hw_gpio.c
 */
#include <hw.h>
#include <driver/gpio.h>
//#include <stdio.h>

#define isPinInverted(p) !!((p).options & GPIO_OPT_INVERT)

static WETAFUNCATTR
void hw_gpio_config_pin(GpioPin *pin)
{
    // Firstly see if the gpio is unused
    if (pin->pin == 0xFF) {
        return;
    } // Unused

    gpio_pad_select_gpio(pin->pin);
    eGpioOptions options = pin->options;

    gpio_mode_t mode = 0;

    if (options & GPIO_OPT_OUTPUT) {
        mode |= GPIO_MODE_OUTPUT;
    }
    if (options & GPIO_OPT_INPUT) {
        mode |= GPIO_MODE_INPUT;
    }
    if (options & GPIO_OPT_OPEN_DRAIN) {
        mode |= GPIO_MODE_DEF_OD;
    }
    gpio_set_direction(pin->pin, mode);

    gpio_pull_mode_t pullmode = GPIO_FLOATING;  // default
    if ((options & GPIO_OPT_PULLUP) && (options & GPIO_OPT_PULLDOWN)) {
        pullmode = GPIO_PULLUP_PULLDOWN;
    } else if (options & GPIO_OPT_PULLUP) {
        pullmode = GPIO_PULLUP_ONLY;
    } else if (options & GPIO_OPT_PULLDOWN) {
        pullmode = GPIO_PULLDOWN_ONLY;
    }
    //gpio_set_pull_mode((gpio_num_t) pin->pin, pullmode);
    //if (gpio->pins[i].dir == GPIO_MODE_INPUT)
    //    gpio_pullup_en((gpio_num_t)gpio->pins[i].pin);

    pin->debounce.state     = false;
    pin->debounce.candidate = false;
    pin->debounce.debounced = false;
    pin->debounce.lastCount = 0;
    pin->debounce.latched   = false;
}

void WETAFUNCATTR
hw_gpio_init(struct _Hardware *hw, uint16_t flags)
{
    flags = flags;

    uint8_t i;
    for (i = 0; i < MAX_GPIO; i++) {
        if (hw->gpio[i].pin != 0xFF) {
            //DEBUGMSG("hw_gpio_init() pins[%d] = %d\r\n", i, gpio->pins[i].pin);
            hw_gpio_config_pin(&hw->gpio[i]);
        }
    }
    //DEBUGMSG("About to leave hw_gpio_init()\r\n");
}

bool WETAFUNCATTR
hw_gpio_config(struct _Hardware *hw, uint8_t index, uint8_t gpio_num, eGpioOptions options)
{
    if (index > MAX_GPIO) {
        return false;
    }
    hw->gpio[index].pin = gpio_num;
    hw->gpio[index].options = options;
    hw_gpio_config_pin(&hw->gpio[index]);
    return true;
}

bool WETAFUNCATTR
hw_gpio_get_raw(GpioPin* gpio, bool *value)
{

    if (gpio->pin == 0xFF) {
        return false;
    }
    *value = (gpio_get_level((gpio_num_t) gpio->pin) != 0) != isPinInverted(*gpio);
    // ESP32 API has no error option!
    return true;
}

bool WETAFUNCATTR
hw_gpio_set(struct _Hardware *hw, uint8_t i, bool value)
{
    if (i >= MAX_GPIO) {
        return false;
    }

    if (hw->gpio[i].pin == 0xFF) {
        return false;
    }
    //bool out = value != isPinInverted(hw->gpio[i]) ? 1 : 0;
    //DEBUGMSG("%d(%d)",hw->gpio[i].pin, out ? 1 : 0);
    esp_err_t rc = gpio_set_level(
        (gpio_num_t) hw->gpio[i].pin, value != isPinInverted(hw->gpio[i]) ? 1 : 0
    );
    return rc == ESP_OK;
}

bool WETAFUNCATTR
hw_gpio_pulse(struct _Hardware *hw, uint8_t i)
{
    if (i >= MAX_GPIO) {
        return false;
    }
    //return hw_gpio_pulse(&hw->gpio->pins[i]);
    if (hw->gpio[i].pin == 0xFF) {
        return false;
    }
    esp_err_t rc1 = gpio_set_level(
        (gpio_num_t) hw->gpio[i].pin, isPinInverted(hw->gpio[i]) ? 0 : 1
    );
    esp_err_t rc2 = gpio_set_level(
        (gpio_num_t) hw->gpio[i].pin, isPinInverted(hw->gpio[i]) ? 1 : 0
    );
    return rc1 + rc2 == ESP_OK;
}




