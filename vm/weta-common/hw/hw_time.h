#ifndef __HW_TIME_H__
#define __HW_TIME_H__

#include <weta_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void hw_time_waitms(uint16_t ms);
extern void hw_time_waitus(uint16_t us);
extern WetaTimestamp hw_time_ticks(void);
extern WetaTimestamp hw_time_now(void);

#ifdef __cplusplus
}
#endif

#endif // __HW_TIME_H__