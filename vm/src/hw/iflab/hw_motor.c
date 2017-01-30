#include "../hw_motor.h"
#include "hw_board.h"

void 
hw_motor_init(uint16_t flags)
{
	flags = flags;
	stamp_dcmotor_init();
}

void hw_motor_select(Motors* motors, uint8_t select)
{
	int8_t i;
	for (i = 0; i < motors->n_motors; i++)
		motors->motors[i].selected = !!(select & (1 << i));
}

void hw_motor_update(Motors* motors)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
	{
        if (motors->motors[i].selected) 
		{
            if (!motors->motors[i].on)
			{
                stamp_control_motor(motors->motors[i].id, MOTOR_STOP, 0);
			}
            else if(motors->motors[i].brake)
            {
                stamp_control_motor(motors->motors[i].id,MOTOR_SPEED,0);
            }
            else 
			{
				stamp_control_motor(
					motors->motors[i].id,
					(motors->motors[i].dir == MOTOR_THIS_WAY) ? MOTOR_RIGHT:MOTOR_LEFT, 
					0
				);
				stamp_control_motor(motors->motors[i].id, MOTOR_SPEED, motors->motors[i].power);
            }
        }
    }
}

void hw_motor_on(Motors* motors, bool on)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
	{
		if (motors->motors[i].selected)
		{
			motors->motors[i].on = on;
			if (on)
				motors->motors[i].brake = false;
		}
	}
	hw_motor_update(motors);
}

void hw_motor_brake(Motors* motors, bool brake)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
	{
		if (motors->motors[i].selected)
		{
			motors->motors[i].brake = brake;
			if (brake)
				motors->motors[i].on = false;
		}
	}
	hw_motor_update(motors);
}

void hw_motor_direction(Motors* motors, MotorDirection dir)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
		if (motors->motors[i].selected)
			motors->motors[i].dir = dir;

	hw_motor_update(motors);
}

void hw_motor_reverse(Motors* motors)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
	{
		if (motors->motors[i].selected)
		{
			if (motors->motors[i].dir == MOTOR_THIS_WAY)
				motors->motors[i].dir = MOTOR_THAT_WAY;
			else
				motors->motors[i].dir = MOTOR_THIS_WAY;
		}
	}
	hw_motor_update(motors);
}

void hw_motor_power(Motors* motors, uint8_t power)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
		if (motors->motors[i].selected)
			motors->motors[i].power = power;

	hw_motor_update(motors);

}