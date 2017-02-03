#ifndef __HW_PWM_H__
#define  __HW_PWM_H__

#include <weta_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void hw_pwm_init(PwmChannels *channels, uint16_t flags);
extern void hw_pwm_set_duty_i(PwmChannels *channels, WetaPwmChannel channel, WetaPwmDuty duty);
extern void hw_pwm_set_duty(PwmChannel *channel, WetaPwmDuty duty);

#ifdef __cplusplus
}
#endif

#endif //__HW_PWM_H__
