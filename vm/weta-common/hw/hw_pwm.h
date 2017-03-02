#ifndef __HW_PWM_H__
#define  __HW_PWM_H__

#include <weta_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_pwm_init(struct _Hardware* hw, uint16_t flags);
extern void hw_pwm_set_duty_i(PwmChannels *channels, WetaPwmChannel channel, WetaPwmDuty duty);
extern void hw_pwm_set_duty(PwmChannel *channel, WetaPwmDuty duty);

#ifdef __cplusplus
}
#endif

#endif //__HW_PWM_H__
