#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

#include <weta_platform.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    GPIO_OPT_NONE       = 0x00,
    GPIO_OPT_INPUT      = 0x01,
    GPIO_OPT_OUTPUT     = 0x02,
    GPIO_OPT_PULLUP     = 0x04,
    GPIO_OPT_PULLDOWN   = 0x08,
    GPIO_OPT_OPEN_DRAIN = 0x10,
    GPIO_OPT_INVERT     = 0x20,
    GPIO_OPT_DEBOUNCE   = 0x40
}eGpioOptions;

typedef struct
{
    bool                state     :1;
    bool                candidate :1;
    bool                debounced :1;
    bool                latched   :1;
    WetaDebounceCounter lastCount : sizeof(WetaDebounceCounter)*8;
} DebounceState;

typedef struct
{
    WetaPin			pin;
    eGpioOptions    options;
    DebounceState   debounce;
} GpioPin;

typedef GpioPin  Gpio[MAX_GPIO];

struct _Hardware;

extern void hw_gpio_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_gpio_config(struct _Hardware* hw, uint8_t index, uint8_t gpio_num, eGpioOptions options);
extern bool hw_gpio_get(struct _Hardware* hw, uint8_t i, bool *value);
extern bool hw_gpio_get_raw(GpioPin* gpio, bool *value);
extern bool hw_gpio_get_debounced(GpioPin* gpio, bool *value);
extern bool hw_gpio_set(struct _Hardware* hw, uint8_t i, bool value);
extern bool hw_gpio_pulse(struct _Hardware* hw, uint8_t i);
#ifdef SUPPORT_DEBOUNCE
extern void hw_gpio_debounce(struct _Hardware* hw);
extern void hw_gpio_clear_debounce(struct _Hardware* hw, uint8_t gpio_num);
#endif

#ifdef __cplusplus
}
#endif

#endif // __HW_GPIO_H__
