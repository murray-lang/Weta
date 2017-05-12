#ifndef __HW_ADC_H__
#define __HW_ADC_H__

#include <weta_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_adc_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_adc_config(struct _Hardware* hw, uint8_t depth);
extern bool hw_adc_channel_config(struct _Hardware* hw, uint8_t channel, int8_t gain);
extern bool hw_adc_get(struct _Hardware* hw, uint8_t i, int16_t *value);

#ifdef __cplusplus
}
#endif

#endif // __HW_ADC_H__