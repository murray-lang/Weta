#include <weta_platform.h>
#include <hw_pwm.h>

void WETAFUNCATTR
hw_pwm_init(PwmChannels* channels, uint16_t flags)
{
    pwm_init(
            channels->frequency,
            channels->initial_duty,
            channels->n_channels,
            channels->gpio_info
    );
}

void WETAFUNCATTR
hw_pwm_set_duty_i(PwmChannels* channels, WetaPwmChannel channel, WetaPwmDuty duty)
{
    if (channel >= channels->n_channels)
        return;

    hw_pwm_set_duty(&channel, duty);
}

void WETAFUNCATTR
hw_pwm_set_duty(PwmChannel* channel, WetaPwmDuty duty)
{

    pwm_set_duty(duty, *channel);
}
