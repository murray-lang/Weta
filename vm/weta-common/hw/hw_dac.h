#ifndef __HW_DAC_H__
#define __HW_DAC_H__

#include <weta_platform.h>
#include "hw.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_dac_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_dac_set(struct _Hardware* hw, uint8_t channel, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif // __HW_DAC_H__