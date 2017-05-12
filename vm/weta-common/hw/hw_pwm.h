#ifndef __HW_PWM_H__
#define  __HW_PWM_H__

#include <weta_platform.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_pwm_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_pwm_config_timer(struct _Hardware* hw, uint8_t timer, uint16_t frequency, uint8_t width);
extern bool hw_pwm_config_channel(struct _Hardware* hw, WetaPwmChannel index, uint8_t gpio_num, uint8_t timer);
//extern void hw_pwm_set_duty_i(WetaPwmChannel channel, WetaPwmDuty duty);
extern void hw_pwm_set_duty(WetaPwmChannel channel, WetaPwmDuty duty);

#ifdef __cplusplus
}
#endif

#endif //__HW_PWM_H__
