#include "../hw_adc.h"
#include "hw_board.h"

void 
hw_adc_init(uint16_t flags)
{
	flags = flags;
	stamp_init_adc();
}

bool 
hw_adc_get(uint8_t channel, int16_t* value)
{
	return stamp_adc(channel, (uint16_t*)value);
}