#include "hw_servo.h"
#include "hw.h"

//#include <ch.h>				// For debugging
//#include <hal.h>			// For debugging
//#include "./hw/hw_serial.h" // For debugging
//#include <string.h>
//#include <stdio.h>          // For debugging (sprintf())

static void hw_servo_update(struct _Hardware* hw);

void WETAFUNCATTR
hw_servo_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;
}

bool WETAFUNCATTR
hw_servo_config(
    struct _Hardware* hw,
    uint8_t servoid,
    uint8_t pwm,
    uint16_t minduty,
    uint16_t maxduty,
    uint16_t duty0,
    int16_t dutyper10   // Note signed
)
{
    if (servoid >= MAX_SERVOS)
        return false;

    ServoState* servo = &hw->servos[servoid];
    servo->selected   = false;
    servo->position   = 0;
    servo->pwm        = pwm;
    servo->config.dutyMin          = minduty;
    servo->config.dutyMax          = maxduty;
    servo->config.duty0            = duty0;
    servo->config.dutyPer10Degrees = dutyper10;
    return true;
}

void WETAFUNCATTR 
hw_servo_select(struct _Hardware* hw, uint8_t select)
{
	int8_t i;
	for (i = 0; i < MAX_SERVOS; i++) {
		if (hw->servos[i].pwm != 0xFF) {
			hw->servos[i].selected = (select & (1 << i)) != 0;
		}
	}
}

uint16_t WETAFUNCATTR 
calc_duty(ServoState* servo)
{
	uint16_t duty =   servo->config.duty0 
					+ (servo->config.dutyPer10Degrees * servo->position) / 10;
			
	if (duty < servo->config.dutyMin)
		return servo->config.dutyMin;
	if (duty > servo->config.dutyMax)
		return servo->config.dutyMax;

	return duty;
}

static void WETAFUNCATTR
hw_servo_update(struct _Hardware* hw)
{
	uint8_t i;
	for (i = 0; i < MAX_SERVOS; i++)
	{
		if (hw->servos[i].pwm == 0xFF) {
			continue;
		}
		if (hw->servos[i].selected)
		{
			//char szMsg[32];
			//sprintf(szMsg,"hw_servo_update(): Servo %d to %d\r\n", i, servos->servos[i].position);
			//hw_serial_write(Serial2, (uint8_t*)szMsg, strlen(szMsg));
			hw_pwm_set_duty(hw->servos[i].pwm, (WetaPwmDuty)calc_duty(&hw->servos[i]));
		}
	}
}

void WETAFUNCATTR 
hw_servo_set_position(struct _Hardware* hw, int16_t position)
{
	uint8_t i;
	for (i = 0; i < MAX_SERVOS; i++)
	{
		if (hw->servos[i].pwm == 0xFF) {
			continue;
		}
		if (hw->servos[i].selected)
		{
            hw->servos[i].position = position;
			//char szMsg[64];
			//sprintf(szMsg,"hw_servo_set_position(): Servo %d position to %d\r\n", i, position);
			//hw_serial_write(Serial2, (uint8_t*)szMsg, strlen(szMsg));
		}
	}
		
	hw_servo_update(hw);
}

void WETAFUNCATTR
hw_servo_left(struct _Hardware* hw, int16_t amount)
{
    // TODO: implement hw_servo_left
}

void WETAFUNCATTR
hw_servo_right(struct _Hardware* hw, int16_t amount)
{
    // TODO: implement hw_servo_left
}

