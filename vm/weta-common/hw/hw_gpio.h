#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

#include <weta_platform.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_gpio_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_gpio_get_i(Gpio *gpio, uint8_t i, bool *value);
extern bool hw_gpio_get(GpioPin *pin, bool *value);
extern bool hw_gpio_set_i(Gpio *gpio, uint8_t i, bool value);
extern bool hw_gpio_set(GpioPin *pin, bool value);
extern bool hw_gpio_pulse_i(Gpio *gpio, uint8_t i);
extern bool hw_gpio_pulse(GpioPin *pin);
#ifdef SUPPORT_DEBOUNCE
extern void hw_gpio_debounce(Gpio *gpio);
extern void hw_gpio_clear_debounce(GpioPin *pin);
#endif

#ifdef __cplusplus
}
#endif

#endif // __HW_GPIO_H__
