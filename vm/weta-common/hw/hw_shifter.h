#ifndef __HW_SHIFTER_H__
#define __HW_SHIFTER_H__

#include <weta_platform.h>
#include <stdbool.h>
#include "hw_gpio.h"

typedef struct
{
    uint8_t clock;
    uint8_t data;
    uint8_t strobe;
    uint8_t width;
        // Although a shifter can shift any amount of data in or out, and
        // it's tempting to keep this code minimal, it's also very handy to
        // have a buffer associated with the shifter. Putting it here saves
        // mucking around with collections of associated buffers elsewhere.
        // Fixed with to deal with small systems without dynamic memory.
    uint8_t buffer[MAX_SHIFTER_WIDTH/8];
        // Count of how much has shifted in/out since last touched.
    uint8_t shifted;
} Shifter;

typedef Shifter Shifters[MAX_SHIFTERS];

extern void hw_shift_init(struct _Hardware* hw, uint16_t flags);

extern bool hw_shift_config(
    struct _Hardware* hw,
    uint8_t shifter,
    uint8_t width,
    uint8_t data,
    uint8_t clock,
    uint8_t strobe
);

extern bool hw_shift_set_buffer(
    struct _Hardware* hw,
    uint8_t shifter,
    uint8_t bits,
    uint8_t offset,
    uint8_t count
);
extern bool hw_shift_get_buffer(
    struct _Hardware* hw,
    uint8_t shifter,
    uint8_t* bits,
    uint8_t offset,
    uint8_t count
);

extern uint8_t hw_shift_out(
    struct _Hardware* hw,
    uint8_t shifter,
    uint8_t  count,
    bool     strobe
);
extern uint8_t hw_shift_in(
    struct _Hardware* hw,
    uint8_t shifter,
    uint8_t  count,
    bool     strobe
);
extern bool hw_shift_strobe(struct _Hardware* hw, uint8_t shifter);

#endif  //__HW_SHIFTER_H__
