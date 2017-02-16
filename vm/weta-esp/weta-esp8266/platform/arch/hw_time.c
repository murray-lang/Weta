#include <weta_platform.h>
#include <os_type.h>
#include <osapi.h>

void WETAFUNCATTR 
hw_time_waitms(uint16_t ms)
{
	os_delay_us(ms * 1000);
}

void WETAFUNCATTR 
hw_time_waitus(uint16_t us)
{
	os_delay_us(us);
}

uint32_t WETAFUNCATTR
hw_time_ticks(void)
{
	return system_get_time();
}

uint32_t WETAFUNCATTR 
hw_time_now(void)
{
	return system_get_time();
}
