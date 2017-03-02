#include <weta_platform.h>
#include <hw.h>
#include <driver/adc.h>


void WETAFUNCATTR
hw_adc_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;
    adc1_config_width((adc_bits_width_t)hw->adc.depth);
	int i = 0;
	for (i = 0; i < hw->adc.n_channels; i++)
    {
        adc1_config_channel_atten(
                (adc1_channel_t)hw->adc.channels[i].channel,
                (adc_atten_t)hw->adc.channels[i].atten);
    }
}

bool WETAFUNCATTR
hw_adc_get(Adc* adc, uint8_t i, int16_t* value)
{
    if (i < adc->n_channels)
        *value = (int16_t) adc1_get_voltage(adc->channels[i].channel);
    else
        *value = 0;
    return true;
}
