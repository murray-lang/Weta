#ifndef __HW_ADC_H__
#define __HW_ADC_H__

#include <weta_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void hw_adc_init(Adc *adc, uint16_t flags);
extern bool hw_adc_get(Adc *adc, uint8_t i, int16_t *value);

#ifdef __cplusplus
}
#endif

#endif // __HW_ADC_H__