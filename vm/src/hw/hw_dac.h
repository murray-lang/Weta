#ifndef __HW_DAC_H__
#define __HW_DAC_H__

#include <weta_platform.h>
#include <stdbool.h>

extern void hw_dac_init(uint16_t flags);
extern bool hw_dac_set(uint8_t channel, uint16_t value);

#endif // __HW_DAC_H__