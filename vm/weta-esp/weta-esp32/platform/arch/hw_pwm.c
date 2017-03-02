#include <weta_platform.h>
#include <hw.h>
#include <driver/ledc.h>

void WETAFUNCATTR
hw_pwm_init(struct _Hardware* hw, uint16_t flags)
{

        // One-time PWM timer configuration
    ledc_timer_config_t config;
    config.speed_mode = LEDC_HIGH_SPEED_MODE;   // Only high speed mode available at the moment
    config.bit_num    = LEDC_TIMER_10_BIT;
    config.timer_num  = LEDC_TIMER_0;           // Just use one timer at this stage
    config.freq_hz    = PWM_FREQ;               // From weta_platform.h
    
    ledc_timer_config(&config);

        // Now configure each channel
    ledc_channel_config_t channel_config;
    channel_config.speed_mode   = LEDC_HIGH_SPEED_MODE; // Only high speed mode available at the moment
    channel_config.intr_type    = LEDC_INTR_DISABLE;    // No fade interrupt required
    channel_config.timer_sel    = LEDC_TIMER_0;         // Select the timer source of channel (0 - 3)
    channel_config.duty         = 0;                    // Leave off to start with

    int i;
    for (i = 0; i < hw->pwms.n_channels; i++)
    {
        channel_config.gpio_num     = hw->pwms.channels[i].pin;      //the LEDC output gpio_num
        channel_config.channel      = hw->pwms.channels[i].channel;  // LEDC channel(0 - 7)
        ledc_channel_config(&channel_config);
    }
}

void 
hw_pwm_set_duty_i(PwmChannels* channels, WetaPwmChannel channel, WetaPwmDuty duty)
{
    if (channel >= channels->n_channels)
        return;
    hw_pwm_set_duty(&channels->channels[channel], duty);
}

void
hw_pwm_set_duty(PwmChannel* channel, WetaPwmDuty duty)
{

    ledc_set_duty(
            LEDC_HIGH_SPEED_MODE,
            (ledc_channel_t) channel->channel,
            (uint32_t) (duty * 1023) / 255
    );
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel->channel);
}
