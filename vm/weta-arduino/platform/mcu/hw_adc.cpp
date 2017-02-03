#include <weta_platform.h>
#include "Arduino.h"

extern "C"
{
void WETAFUNCATTR
hw_adc_init(Adc *adc, uint16_t flags)
{
    flags = flags;
    // Nothing to do
}

bool WETAFUNCATTR
hw_adc_get(Adc *adc, uint8_t i, int16_t *value)
{
    if (i < adc->n_channels)
        *value = (int16_t) analogRead(adc->channels[i]);
    else
        *value = 0;
    return true;
}
}
