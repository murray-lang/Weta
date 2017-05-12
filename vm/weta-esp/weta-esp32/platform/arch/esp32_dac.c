#include <hw.h>
#include <driver/dac.h>

void WETAFUNCATTR
hw_dac_init(struct _Hardware* hw,uint16_t flags)
{
	flags = flags;
}


bool WETAFUNCATTR
hw_dac_set(struct _Hardware* hw, uint8_t channel, uint16_t value)
{
	return dac_out_voltage(
		(dac_channel_t) channel,
		(uint8_t)value
	) == ESP_OK;
}
