#ifndef __HW_BUZZER_H__
#define __HW_BUZZER_H__

#include <weta_platform.h>
#include "hw.h"
#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_buzzer_init(struct _Hardware* hw, uint16_t flags);
extern void hw_buzzer_beep(struct _Hardware* hw);

#ifdef __cplusplus
}
#endif
#endif // __HW_BUZZER_H__