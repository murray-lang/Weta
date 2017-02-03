#include "../hw_buzzer.h"
#include "hw_board.h"

void 
hw_buzzer_init(uint16_t flags)
{
	flags = flags;
	stamp_buzzer_init();
}

void 
hw_buzzer_beep(void)
{
	stamp_buzzer_beep();
}