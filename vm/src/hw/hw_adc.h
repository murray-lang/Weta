#ifndef __HW_ADC_H__
#define __HW_ADC_H__

#include <weta_platform.h>
#include <stdbool.h>

typedef struct
{
    WetaAdcChannel  channel;
    WetaAdcAtten    atten;
} AdcChannel;

typedef struct
{
    AdcChannel*     channels;
    uint8_t         n_channels;
    WetaAdcDepth    depth;
} Adc;

extern void hw_adc_init(Adc* adc, uint16_t flags);
extern bool hw_adc_get(Adc* adc, uint8_t i, int16_t* value);

#endif // __HW_ADC_H__