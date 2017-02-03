#ifndef __HW_DAC_H__
#define __HW_DAC_H__

#include <weta_platform.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void hw_dac_init(uint16_t flags);
extern bool hw_dac_set(uint8_t channel, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif // __HW_DAC_H__