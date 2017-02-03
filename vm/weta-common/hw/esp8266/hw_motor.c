#include "../hw_motor.h"
#include "hw_board.h"
#include <driver/pwm.h>
#include "gpio.h"
#include "eagle_soc.h"

#define MOTOR_0_DIRA_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define MOTOR_0_DIRA_IO_NUM 14
#define MOTOR_0_DIRA_IO_FUNC  FUNC_GPIO14

#define MOTOR_0_DIRB_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define MOTOR_0_DIRB_IO_NUM 13
#define MOTOR_0_DIRB_IO_FUNC  FUNC_GPIO13

#define MOTOR_1_DIRA_IO_MUX PERIPHS_IO_MUX_GPIO4_U
#define MOTOR_1_DIRA_IO_NUM 4
#define MOTOR_1_DIRA_IO_FUNC  FUNC_GPIO4

#define MOTOR_1_DIRB_IO_MUX PERIPHS_IO_MUX_GPIO5_U
#define MOTOR_1_DIRB_IO_NUM 5
#define MOTOR_1_DIRB_IO_FUNC  FUNC_GPIO5

struct motor_pin_info 
{
	uint8 dir_a;
	uint8 dir_b;
};

static struct motor_pin_info pin_info[] =
{
	{ MOTOR_0_DIRA_IO_NUM, MOTOR_0_DIRB_IO_NUM },
	{ MOTOR_1_DIRA_IO_NUM, MOTOR_1_DIRB_IO_NUM }
};

void WETAFUNCATTR 
hw_motor_init(uint16_t flags)
{
	flags = flags;
	
	PIN_FUNC_SELECT(MOTOR_0_DIRA_IO_MUX, MOTOR_0_DIRA_IO_FUNC);
    PIN_FUNC_SELECT(MOTOR_0_DIRB_IO_MUX, MOTOR_0_DIRB_IO_FUNC);

	PIN_FUNC_SELECT(MOTOR_1_DIRA_IO_MUX, MOTOR_1_DIRA_IO_FUNC);
    PIN_FUNC_SELECT(MOTOR_1_DIRB_IO_MUX, MOTOR_1_DIRB_IO_FUNC);

	uint8 init_duty[] = { 128, 128 };
    pwm_init(100, init_duty);
}

void WETAFUNCATTR
hw_motor_select(Motors* motors, uint8_t select)
{
	int8_t i;
	for (i = 0; i < motors->n_motors; i++)
		motors->motors[i].selected = !!(select & (1 << i));
}

void WETAFUNCATTR
hw_motor_update(Motors* motors)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
	{
        if (motors->motors[i].selected) 
		{
            if (!motors->motors[i].on)
			{
                GPIO_OUTPUT_SET(pin_info[i].dir_a, false);
				GPIO_OUTPUT_SET(pin_info[i].dir_b, false);
			}
            else if(motors->motors[i].brake)
            {
                GPIO_OUTPUT_SET(pin_info[i].dir_a, true);
				GPIO_OUTPUT_SET(pin_info[i].dir_b, true);
            }
            else 
			{
				if (motors->motors[i].dir == MOTOR_THIS_WAY)
				{
					GPIO_OUTPUT_SET(pin_info[i].dir_a, true);
					GPIO_OUTPUT_SET(pin_info[i].dir_b, false);
				}
				else
				{
					GPIO_OUTPUT_SET(pin_info[i].dir_a, false);
					GPIO_OUTPUT_SET(pin_info[i].dir_b, true);
				}
				pwm_set_duty((motors->motors[i].power * 255)/100, i);
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
		if (motors->motors[i].selected)
		{
			motors->motors[i].on = on;
			if (on)
				motors->motors[i].brake = false;
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
		if (motors->motors[i].selected)
		{
			motors->motors[i].brake = brake;
			if (brake)
				motors->motors[i].on = false;
		}
	}
	hw_motor_update(motors);
}

void WETAFUNCATTR 
hw_motor_direction(Motors* motors, MotorDirection dir)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
		if (motors->motors[i].selected)
			motors->motors[i].dir = dir;

	hw_motor_update(motors);
}

void WETAFUNCATTR 
hw_motor_reverse(Motors* motors)
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

void WETAFUNCATTR 
hw_motor_power(Motors* motors, uint8_t power)
{
	int8_t i;
    for (i = 0; i < motors->n_motors; ++i) 
		if (motors->motors[i].selected)
			motors->motors[i].power = power;

	hw_motor_update(motors);

}
