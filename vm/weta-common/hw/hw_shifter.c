#include "hw_shifter.h"


bool WETAFUNCATTR
hw_shift_strobe(Shifter* shifter)
{
    return hw_gpio_pulse(shifter->strobe);
}

bool WETAFUNCATTR
hw_shift_out_byte(Shifter* shifter, uint8_t bits, uint8_t n, bool strobe)
{
    int i;
    for (i = 0; i < n; i++)
    {
        hw_gpio_set(shifter->data, (bits & (1<<i)) != 0);
        hw_gpio_pulse(shifter->clock);
    }
    if (strobe)
        hw_gpio_pulse(shifter->strobe);

    return true;
}

bool WETAFUNCATTR
hw_shift_in_byte(Shifter* shifter, uint8_t* bits, uint8_t n, bool strobe)
{
    if (strobe)
        hw_gpio_pulse(shifter->strobe);
    int i;
    for (i = 0; i < n; i++)
    {
        hw_gpio_pulse(shifter->clock);
        bool next;
        hw_gpio_get(shifter->data, &next);
        *bits &= ~(1<<n);
        if (next)
            *bits |= 1 << n;
    }
    return true;
}