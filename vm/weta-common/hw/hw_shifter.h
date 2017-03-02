#ifndef __HW_SHIFTER_H__
#define __HW_SHIFTER_H__

#include <weta_platform.h>
#include <stdbool.h>
#include "hw_gpio.h"

typedef struct
{
    GpioPin* clock;
    GpioPin* data;
    GpioPin* strobe;
} Shifter;

extern bool hw_shift_out_byte(Shifter* shifter, uint8_t bits, uint8_t n, bool strobe);
extern bool hw_shift_in_byte(Shifter* shifter, uint8_t* bits, uint8_t n, bool strobe);
extern bool hw_shift_strobe(Shifter* shifter);

#endif  //__HW_SHIFTER_H__
