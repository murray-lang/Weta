#include <weta_platform.h>
#include "Arduino.h"
#include <hw_pwm.h>

extern "C" {

void
hw_pwm_init(PwmChannels *channels, uint16_t flags)
{
    // Set pin as output
    int i;
    for (i = 0; i < channels->n_channels; i++)
    {
        pinMode(channels->channels[i], OUTPUT);
    }

}

void
hw_pwm_set_duty_i(PwmChannels *channels, WetaPwmChannel channel, WetaPwmDuty duty)
{
    if (channel >= channels->n_channels)
        return;
    hw_pwm_set_duty(&channels->channels[channel], duty);
}

void
hw_pwm_set_duty(PwmChannel *channel, WetaPwmDuty duty)
{
    analogWrite(*channel, duty);
}

} //extern "C"
