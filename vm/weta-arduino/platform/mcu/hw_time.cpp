#include <weta_platform.h>
#include "Arduino.h"

extern "C" {

void WETAFUNCATTR
hw_time_waitms(uint16_t ms)
{
	delay(ms);
}

void WETAFUNCATTR
hw_time_waitus(uint16_t us)
{
	delayMicroseconds(us);
}

uint32_t WETAFUNCATTR
hw_time_ticks(void)
{
	return micros();
}

uint32_t WETAFUNCATTR
hw_time_now(void)
{
	return (uint32_t) millis(); // For now. No rtc.
}

} //extern "C"
