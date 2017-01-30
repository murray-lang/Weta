#include "../hw_stepper.h"
#include "gpio.h"
#include "eagle_soc.h"

#define INTERSTEP_WAIT 3
#define STEPPER_0_A_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define STEPPER_0_A_IO_NUM 12
#define STEPPER_0_A_IO_FUNC  FUNC_GPIO12

#define STEPPER_0_B_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define STEPPER_0_B_IO_NUM 13
#define STEPPER_0_B_IO_FUNC  FUNC_GPIO13

//#define STEPPER_0_B_IO_MUX PERIPHS_IO_MUX_MTMS_U
//#define STEPPER_0_B_IO_NUM 14
//#define STEPPER_0_B_IO_FUNC  FUNC_GPIO14

// The GPIO 4 and 5 were swapped around here because the silk screened labelling
// of these pins on the ESP-07 is reversed. (This is a known problem)
#define STEPPER_1_B_IO_MUX PERIPHS_IO_MUX_GPIO4_U
#define STEPPER_1_B_IO_NUM 4
#define STEPPER_1_B_IO_FUNC  FUNC_GPIO4

#define STEPPER_1_A_IO_MUX PERIPHS_IO_MUX_GPIO5_U
#define STEPPER_1_A_IO_NUM 5
#define STEPPER_1_A_IO_FUNC  FUNC_GPIO5

#define INCREMENT_STEP(n) ((n) < STEPPER_STEPS-1 ? (n) + 1 : 0);
#define DECREMENT_STEP(n) ((n) == 0 ? STEPPER_STEPS-1 : (n) - 1); 

typedef struct
{
	uint8_t		pins[STEPPER_WIDTH];
	uint8_t		index;
} StepperMotor;
/*
static const uint8_t steps[STEPPER_STEPS][STEPPER_WIDTH] = 
{ 
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 1 },
	{ 1, 0 }
};
*/

static const uint8_t steps[STEPPER_STEPS][STEPPER_WIDTH] = 
{ 
	{ 0, 0 },
	{ 1, 0 },
	{ 1, 1 },
	{ 0, 1 }
};

static StepperMotor steppers[] =
	{
		{
			{ STEPPER_0_A_IO_NUM, STEPPER_0_B_IO_NUM },
			0
		},
		{
			{ STEPPER_1_A_IO_NUM, STEPPER_1_B_IO_NUM },
			0
		}
	};

static eStepCommand	stepperCmd1 = STEP_CMD_NONE;
static eStepCommand	stepperCmd2 = STEP_CMD_NONE;	
static StepperArg	stepperArg1 = 0;
static StepperArg	stepperArg2 = 0;

void hw_stepper_reset(void);
void hw_stepper_apply(void);
uint8_t hw_increment_step(uint8_t val);


void WETAFUNCATTR
hw_stepper_init(uint16_t flags)
{
	flags = flags;
	PIN_FUNC_SELECT(STEPPER_0_A_IO_MUX, STEPPER_0_A_IO_FUNC);
    PIN_FUNC_SELECT(STEPPER_0_B_IO_MUX, STEPPER_0_B_IO_FUNC);

	PIN_FUNC_SELECT(STEPPER_1_A_IO_MUX, STEPPER_1_A_IO_FUNC);
    PIN_FUNC_SELECT(STEPPER_1_B_IO_MUX, STEPPER_1_B_IO_FUNC);
	
	//PIN_PULLUP_DIS(STEPPER_0_A_IO_MUX);
	//PIN_PULLUP_DIS(STEPPER_0_B_IO_MUX);
	//PIN_PULLUP_DIS(STEPPER_1_A_IO_MUX);
	//PIN_PULLUP_DIS(STEPPER_1_B_IO_MUX);
	
	//GPIO_OUTPUT_SET(STEPPER_0_A_IO_NUM, true);
	//GPIO_OUTPUT_SET(STEPPER_0_B_IO_NUM, false);
	//GPIO_OUTPUT_SET(STEPPER_1_A_IO_NUM, true);
	//GPIO_OUTPUT_SET(STEPPER_1_B_IO_NUM, false);
	hw_stepper_apply();
}

bool WETAFUNCATTR
hw_stepper_tick(void)
{
	bool apply = false;
	if (stepperArg1)
	{
		if (stepperCmd1 == STEP_CMD_FORWARD)
		{
			steppers[0].index = INCREMENT_STEP(steppers[0].index);
		}
		else if(stepperCmd1 == STEP_CMD_BACKWARD)
		{
			steppers[0].index = DECREMENT_STEP(steppers[0].index);
		}
		apply = true;
		stepperArg1--;
	}
	
	if (stepperArg2)
	{
			// Note that the direction is reversed for one of the motors
			// because it is physically mounted in the opposite direction
		if (stepperCmd2 == STEP_CMD_FORWARD)
		{
			steppers[1].index = DECREMENT_STEP(steppers[1].index);
		}
		else if(stepperCmd2 == STEP_CMD_BACKWARD)
		{
			steppers[1].index = INCREMENT_STEP(steppers[1].index);
		}
		apply = true;
		stepperArg2--;
	}
	if (apply)
	{
		hw_stepper_apply();
		hw_time_waitms(INTERSTEP_WAIT);
	}
	return stepperArg1 + stepperArg2 == 0;
}

void WETAFUNCATTR
hw_stepper_control(eStepCommand cmd1, StepperArg arg1, eStepCommand cmd2, StepperArg arg2)
{
	stepperCmd1 = cmd1;
	stepperCmd2 = cmd2;
	stepperArg1 = arg1;
	stepperArg2 = arg2;
	
	if (cmd1 == STEP_CMD_NONE)
	{
		stepperArg1 = 0;
	}
	else if (cmd1 == STEP_CMD_RESET)
	{
		steppers[0].index = 0;
		stepperCmd1 = STEP_CMD_NONE;
		stepperArg1 = 0; 
	}
	
	if (cmd2 == STEP_CMD_NONE)
	{
		stepperArg2 = 0;
	}
	else if (cmd2 == STEP_CMD_RESET)
	{
		steppers[1].index = 0;
		stepperCmd2 = STEP_CMD_NONE;
		stepperArg2 = 0; 
	}
}

void WETAFUNCATTR
hw_stepper_reset(void)
{
	stepperCmd1 = STEP_CMD_NONE;
	stepperCmd2 = STEP_CMD_NONE;	
	stepperArg1 = 0;
	stepperArg2 = 0;
}

void WETAFUNCATTR
hw_stepper_apply(void)
{
		// Brackets around the arguments because the GPIO_OUTPUT_SET macro is
		// poorly implemented.
	//GPIO_OUTPUT_SET((steppers[0].pins[0]), (steps[steppers[0].index][0]));
	//GPIO_OUTPUT_SET((steppers[0].pins[1]), (steps[steppers[0].index][1]));
	//GPIO_OUTPUT_SET((steppers[1].pins[0]), (steps[steppers[1].index][0]));
	//GPIO_OUTPUT_SET((steppers[1].pins[1]), (steps[steppers[1].index][1]));
	
	GPIO_OUTPUT_SET(STEPPER_0_A_IO_NUM, (steps[steppers[0].index][0]));
	GPIO_OUTPUT_SET(STEPPER_0_B_IO_NUM, (steps[steppers[0].index][1]));
	GPIO_OUTPUT_SET(STEPPER_1_A_IO_NUM, (steps[steppers[1].index][0]));
	GPIO_OUTPUT_SET(STEPPER_1_B_IO_NUM, (steps[steppers[1].index][1]));
}

