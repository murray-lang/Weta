#include "hw.h"

void WETAFUNCATTR 
hw_motor_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;
	hw_motor_all_off(&hw->motors);
}

void WETAFUNCATTR
hw_motor_select(Motors* motors, uint8_t select)
{
	int8_t i;
	for (i = 0; i < motors->n_motors; i++)
	{
		//DEBUGMSG("Motor %d...", i);
		bool selected = !!(select & (1 << i));
		//DEBUGMSG(selected ? "selected" : "not selected");
		motors->motors[i].state.selected = selected;
		//DEBUGMSG("...done\r\n");
	}
}

void WETAFUNCATTR
hw_motor_update(Motors* motors)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i)
	{
        MotorState* state = &motors->motors[i].state;
        if (state->selected)
		{
            MotorPins* pins = &motors->motors[i].pins;
            if (!state->on)
			{
#if defined (MOTORS_A_B_PWM)
                hw_gpio_set(pins->a, false);
                hw_gpio_set(pins->b, false);
#elif defined (MOTORS_DIR_PWM)
                hw_pwm_set_duty(pins->pwm, (WetaPwmDuty)0);
#endif
			}
            else if(state->brake)
            {
#if defined (MOTORS_A_B_PWM)
                hw_gpio_set(pins->a, true);
                hw_gpio_set(pins->b, true);
#elif defined (MOTORS_DIR_PWM)
                hw_pwm_set_duty(pins->pwm, (WetaPwmDuty)0);
#endif
            }
            else 
			{
				if (state->dir == MOTOR_THIS_WAY)
				{
#if defined (MOTORS_A_B_PWM)
                    hw_gpio_set(pins->a, true);
                    hw_gpio_set(pins->b, false);
#elif defined (MOTORS_DIR_PWM)
                    hw_gpio_set(pins->dir, true);
#endif
				}
				else
				{
#if defined (MOTORS_A_B_PWM)
                    hw_gpio_set(pins->a, false);
                    hw_gpio_set(pins->b, true);
#elif defined (MOTORS_DIR_PWM)
                    hw_gpio_set(pins->dir, false);
#endif
				}
                //DEBUGMSG("hw_motor_update(): Motor %d duty = %d\r\n", i, state->power);
                hw_pwm_set_duty(pins->pwm, (WetaPwmDuty)state->power);
            }
        }
    }
}

void WETAFUNCATTR
hw_motor_all_off(Motors* motors)
{
	hw_motor_select(motors, 0xFF);
	hw_motor_on(motors, false);
}

void WETAFUNCATTR
hw_motor_on(Motors* motors, bool on)
{
	//DEBUGMSG("hw_motor_on(%d)\r\n", on);
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i)
	{
		if (motors->motors[i].state.selected)
		{
			motors->motors[i].state.on = on;
			if (on)
				motors->motors[i].state.brake = false;
		}
	}
	hw_motor_update(motors);
}

void WETAFUNCATTR
hw_motor_brake(Motors* motors, bool brake)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i)
	{
		if (motors->motors[i].state.selected)
		{
			motors->motors[i].state.brake = brake;
			if (brake)
				motors->motors[i].state.on = false;
		}
	}
	hw_motor_update(motors);
}

void WETAFUNCATTR 
hw_motor_direction(Motors* motors, MotorDirection dir)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
		if (motors->motors[i].state.selected)
			motors->motors[i].state.dir = dir;

	hw_motor_update(motors);
}

void WETAFUNCATTR 
hw_motor_reverse(Motors* motors)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
	{
		if (motors->motors[i].state.selected)
		{
			if (motors->motors[i].state.dir == MOTOR_THIS_WAY)
				motors->motors[i].state.dir = MOTOR_THAT_WAY;
			else
				motors->motors[i].state.dir = MOTOR_THIS_WAY;
		}
	}
	hw_motor_update(motors);
}

void WETAFUNCATTR 
hw_motor_power(Motors* motors, MotorPower power)
{
	//DEBUGMSG("hw_motor_power(%d)\r\n", power);
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
		if (motors->motors[i].state.selected)
			motors->motors[i].state.power = power;

	hw_motor_update(motors);
}
