#include "hw_motor.h"
#include "hw_gpio.h"
#include "hw_pwm.h"


void WETAFUNCATTR 
hw_motor_init(uint16_t flags)
{
	flags = flags;
	
}

void WETAFUNCATTR
hw_motor_select(Motors* motors, uint8_t select)
{
	int8_t i;
	for (i = 0; i < motors->n_motors; i++)
		motors->motors[i].state.selected = !!(select & (1 << i));
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
#ifdef MOTORS_ARE_H_BRIDGE
                hw_gpio_set(pins->a, false);
                hw_gpio_set(pins->b, false);
#else
                hw_pwm_set_duty(pins->pwm, (WetaPwmDuty)0);
#endif
			}
            else if(state->brake)
            {
#ifdef MOTORS_ARE_H_BRIDGE
                hw_gpio_set(pins->a, true);
                hw_gpio_set(pins->b, true);
#else
                hw_pwm_set_duty(pins->pwm, (WetaPwmDuty)0);
#endif
            }
            else 
			{
				if (state->dir == MOTOR_THIS_WAY)
				{
#ifdef MOTORS_ARE_H_BRIDGE
                    hw_gpio_set(pins->a, true);
                    hw_gpio_set(pins->b, false);
#else
                    hw_gpio_set(pins->dir, true);
#endif
				}
				else
				{
#ifdef MOTORS_ARE_H_BRIDGE
                    hw_gpio_set(pins->a, false);
                    hw_gpio_set(pins->b, true);
#else
                    hw_gpio_set(pins->dir, true);
#endif
				}
                hw_pwm_set_duty(pins->pwm, (WetaPwmDuty)(state->power*PWM_DUTY_RESOLUTION)/255);
            }
        }
    }
}

void WETAFUNCATTR
hw_motor_on(Motors* motors, bool on)
{
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
hw_motor_power(Motors* motors, uint8_t power)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
		if (motors->motors[i].state.selected)
			motors->motors[i].state.power = power;

	hw_motor_update(motors);

}
