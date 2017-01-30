#include "../hw_time.h"
#include "ch.h"

void 
hw_time_waitms(uint16_t ms)
{
	chThdSleepMilliseconds((uint32_t)ms);
}

uint32_t 
hw_time_now(void)
{
	return chTimeNow();
}