#include <hw_pwm.h>
#include <driver/ledc.h>

void 
hw_pwm_init(PwmChannels* channels, uint16_t flags)
{

        // One-time PWM timer configuration
    ledc_timer_config_t config;
    config.speed_mode = LEDC_HIGH_SPEED_MODE;   // Only high speed mode available at the moment
    config.bit_num    = LEDC_TIMER_15_BIT;
    config.timer_num  = LEDC_TIMER_0;           // Just use one timer at this stage
    config.freq_hz    = PWM_FREQ;               // From weta_platform.h
    
    //esp_err_t rc =
    ledc_timer_config(&config);


        // Now configure each channel
    int i;
    //TODO: Find out why GPIO pin 1 is a problem with PWM
    for (i = 0; i < channels->n_channels; i++)
    {
        ledc_channel_config_t channel_config;
        channel_config.gpio_num     = channels->channels[i].pin;      //the LEDC output gpio_num
        channel_config.speed_mode   = LEDC_HIGH_SPEED_MODE; // Only high speed mode available at the moment
        channel_config.channel      = channels->channels[i].channel;  // LEDC channel(0 - 7)
        channel_config.intr_type    = LEDC_INTR_DISABLE;    // No fade interrupt required
        channel_config.timer_sel    = LEDC_TIMER_0;         // Select the timer source of channel (0 - 3)
        channel_config.duty         = 0;                    // Leave off to start with
        //rc =
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
            (uint32_t) duty
    );
}
