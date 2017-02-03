#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

#include <weta_platform.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void hw_gpio_init(Gpio *gpio, uint16_t flags);
extern bool hw_gpio_get_i(Gpio *gpio, uint8_t i, bool *value);
extern bool hw_gpio_get(GpioPin *pin, bool *value);
extern bool hw_gpio_set_i(Gpio *gpio, uint8_t i, bool value);
extern bool hw_gpio_set(GpioPin *pin, bool value);
extern void hw_gpio_debounce(Gpio *gpio);

#ifdef __cplusplus
}
#endif

#endif // __HW_GPIO_H__
