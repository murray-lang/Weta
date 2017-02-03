#include <weta_platform.h>

extern "C" {
void WETAFUNCATTR
hw_buzzer_init(uint16_t flags)
{
	flags = flags;
	//stamp_buzzer_init();
}

void WETAFUNCATTR
hw_buzzer_beep(void)
{
	//stamp_buzzer_beep();
}
}
