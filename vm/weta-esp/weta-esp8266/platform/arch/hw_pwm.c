#include <weta_platform.h>
#include <hw.h>

void WETAFUNCATTR
hw_pwm_init(struct _Hardware* hw, uint16_t flags)
{
    DEBUGMSG("hw_pwm_init()\r\n");

    pwm_init(
            hw->pwms.period,
            hw->pwms.initial_duty,
            hw->pwms.n_channels,
            hw->pwms.gpio_info
    );
    pwm_start();

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
    DEBUGMSG("hw_pwm_set_duty(%d, %d) ", *channel, duty);
    uint32_t converted = (duty*MAX_PWM_DUTY)/255;
    DEBUGMSG("%d\r\n", converted);
    pwm_set_duty(converted, *channel);
    pwm_start();
}
