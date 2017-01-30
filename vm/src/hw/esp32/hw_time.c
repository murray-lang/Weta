#include "../hw_time.h"
#include <rom/ets_sys.h>	// for ets_delay_us()
#include <time.h>
#include <xtensa/core-macros.h>

void WETAFUNCATTR 
hw_time_waitms(uint16_t ms)
{
	ets_delay_us(ms * 1000);
}

void WETAFUNCATTR 
hw_time_waitus(uint16_t us)
{
	ets_delay_us(us);
}

uint32_t WETAFUNCATTR
hw_time_ticks(void)
{
	return XTHAL_GET_CCOUNT();
}

uint32_t WETAFUNCATTR 
hw_time_now(void)
{
	return (uint32_t)time(NULL);
}
