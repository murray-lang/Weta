#ifndef __HW_TIME_H__
#define __HW_TIME_H__

#include <weta_platform.h>

extern void     hw_time_waitms(uint16_t ms);
extern void     hw_time_waitus(uint16_t us);
extern uint32_t hw_time_ticks(void);
extern uint32_t hw_time_now(void);

#endif // __HW_TIME_H__