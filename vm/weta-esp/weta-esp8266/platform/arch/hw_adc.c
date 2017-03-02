#include <weta_platform.h>
#include <hw.h>

void WETAFUNCATTR
hw_adc_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;
	//stamp_init_adc();
}

bool WETAFUNCATTR
hw_adc_get(Adc *adc, uint8_t i, int16_t* value)
{
	return false; //stamp_adc(channel, (uint16_t*)value);
}
