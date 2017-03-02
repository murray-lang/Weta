#include <weta_platform.h>
#include <hw.h>

void WETAFUNCATTR
hw_dac_init(struct _Hardware* hw,uint16_t flags)
{
	flags = flags;
}


bool WETAFUNCATTR
hw_dac_set(struct _Hardware* hw, uint8_t channel, uint16_t value)
{
	channel = channel;
	value = value;
	return false; // No DAC!
}
