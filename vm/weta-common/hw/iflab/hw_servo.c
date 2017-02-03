#include "../hw_servo.h"
#include "hw_board.h"

//#include <ch.h>				// For debugging
//#include <hal.h>			// For debugging
//#include "./hw/hw_serial.h" // For debugging
//#include <string.h>
//#include <stdio.h>          // For debugging (sprintf())

void 
hw_servo_init(uint16_t flags)
{
	flags = flags;
	stamp_servomotor_init();
}

void hw_servo_select(Servos* servos, uint8_t select)
{
	int8_t i;
	for (i = 0; i < servos->n_servos; i++)
		servos->servos[i].selected = !!(select & (1 << i));
}

uint16_t 
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

void 
hw_servo_update(Servos* servos)
{
	uint8_t i;
	for (i = 0; i < servos->n_servos; i++)
	{
		if (servos->servos[i].selected)
		{
			//char szMsg[32];
			//sprintf(szMsg,"hw_servo_update(): Servo %d to %d\r\n", i, servos->servos[i].position);
			//hw_serial_write(Serial2, (uint8_t*)szMsg, strlen(szMsg));
			stamp_servomotor_control(
				servos->servos[i].id,
				calc_duty(&servos->servos[i])
			);
		}
	}
}

void 
hw_servo_set_position(Servos* servos, int16_t position)
{
	uint8_t i;
	for (i = 0; i < servos->n_servos; i++)
	{
		
		if (servos->servos[i].selected)
		{
			servos->servos[i].position = position;
			//char szMsg[64];
			//sprintf(szMsg,"hw_servo_set_position(): Servo %d position to %d\r\n", i, position);
			//hw_serial_write(Serial2, (uint8_t*)szMsg, strlen(szMsg));
		}
	}
		
	hw_servo_update(servos);
}
