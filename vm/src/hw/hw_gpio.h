#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

#include <weta_platform.h>
#include <stdbool.h>


typedef struct
{
    bool        state;
    bool        candidate;
    bool        debounced;
    bool        latched;
    uint32_t    lastTime;
} DebounceState;

typedef struct
{
	WetaPin			pin;
	WetaGpioMode	dir;
	WetaGpioMode	pull;
    DebounceState*  debounce;
} GpioPin;

typedef struct
{
	GpioPin* pins;
	uint8_t  n_pins;
    GpioPin** debounced_pins;
    uint8_t  n_debounced_pins;
} Gpio;

extern void hw_gpio_init(Gpio* gpio, uint16_t flags);
extern bool hw_gpio_get_i(Gpio* gpio, uint8_t i, bool* value);
extern bool hw_gpio_get(GpioPin* pin, bool* value);
extern bool hw_gpio_set_i(Gpio* gpio, uint8_t i, bool value);
extern bool hw_gpio_set(GpioPin* pin, bool value);
extern void hw_gpio_debounce(Gpio* gpio);

#endif // __HW_GPIO_H__
