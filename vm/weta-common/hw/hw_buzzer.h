#ifndef __HW_BUZZER_H__
#define __HW_BUZZER_H__

#include <weta_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void hw_buzzer_init(uint16_t flags);
extern void hw_buzzer_beep(void);

#ifdef __cplusplus
}
#endif
#endif // __HW_BUZZER_H__