#include <weta_platform.h>

void WETAFUNCATTR
hw_dac_init(uint16_t flags)
{
	flags = flags;
}


bool WETAFUNCATTR
hw_dac_set(uint8_t channel, uint16_t value)
{
	channel = channel;
	value = value;
	return false; // No DAC!
}
