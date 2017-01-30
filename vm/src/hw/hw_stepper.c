#include "hw_stepper.h"
#include "hw_time.h"
#include "hw_gpio.h"


#define INTERSTEP_WAIT 3
#define INCREMENT_STEP(n) ((n) < STEPPER_STEPS-1 ? (n) + 1 : 0);
#define DECREMENT_STEP(n) ((n) == 0 ? STEPPER_STEPS-1 : (n) - 1); 

static const uint8_t steps[STEPPER_STEPS][STEPPER_WIDTH] =
{ 
	{ 1, 0, 0, 1 },
	{ 1, 1, 0, 0 },
	{ 0, 1, 1, 0 },
	{ 0, 0, 1, 1 }
};

/*
static eStepCommand	stepperCmd1 = STEP_CMD_NONE;
static eStepCommand	stepperCmd2 = STEP_CMD_NONE;	
static StepperArg	stepperArg1 = 0;
static StepperArg	stepperArg2 = 0;
*/
void hw_stepper_reset(Steppers* steppers);
void hw_stepper_apply(Steppers* steppers);
//uint8_t hw_increment_step(uint8_t val);


void WETAFUNCATTR
hw_stepper_init(Steppers* steppers, uint16_t flags)
{
	flags = flags;

	hw_stepper_apply(steppers);
}

bool WETAFUNCATTR
hw_stepper_tick(Steppers* steppers)
{
	bool apply = false;

	if (steppers->steppers[0].arg)
	{
		if (steppers->steppers[0].cmd == STEP_CMD_FORWARD)
		{
			steppers->steppers[0].index = INCREMENT_STEP(steppers->steppers[0].index);
		}
		else if(steppers->steppers[0].cmd == STEP_CMD_BACKWARD)
		{
			steppers->steppers[0].index = DECREMENT_STEP(steppers->steppers[0].index);
		}
		apply = true;
		steppers->steppers[0].arg--;
	}

	if (steppers->steppers[1].arg)
	{
			// Note that the direction is reversed for one of the motors
			// because it is physically mounted in the opposite direction
		if (steppers->steppers[1].cmd == STEP_CMD_FORWARD)
		{
			steppers->steppers[1].index = DECREMENT_STEP(steppers->steppers[1].index);
		}
		else if(steppers->steppers[1].cmd == STEP_CMD_BACKWARD)
		{
			steppers->steppers[1].index = INCREMENT_STEP(steppers->steppers[1].index);
		}
		apply = true;
		steppers->steppers[1].arg--;
	}
	if (apply)
	{
		hw_stepper_apply(steppers);
		hw_time_waitms(INTERSTEP_WAIT);
            // If both motors are finished then turn both off to save battery
            // power. (Don't turn one off if the other is still going because
            // we might need the braking on the stopped wheel)
            // Note that the steppers might already have been turned off above
        if(   (!steppers->steppers[0].arg && !steppers->steppers[1].arg)
           && (steppers->steppers[0].cmd != STEP_CMD_OFF || steppers->steppers[1].cmd != STEP_CMD_OFF))
        {
            steppers->steppers[0].cmd = STEP_CMD_OFF;
            steppers->steppers[1].cmd = STEP_CMD_OFF;
            hw_stepper_apply(steppers);
        }
	}
	return steppers->steppers[0].arg + steppers->steppers[1].arg == 0;
}

void WETAFUNCATTR
hw_stepper_control(Steppers* steppers, eStepCommand cmd1, StepperArg arg1, eStepCommand cmd2, StepperArg arg2)
{
    steppers->steppers[0].cmd = cmd1;
    steppers->steppers[1].cmd = cmd2;
    steppers->steppers[0].arg = arg1;
    steppers->steppers[1].arg = arg2;
	
	if (cmd1 == STEP_CMD_NONE)
	{
        steppers->steppers[0].arg = 0;
	}
	else if (cmd1 == STEP_CMD_OFF)
	{
		steppers->steppers[0].arg = 1;
	}
	else if (cmd1 == STEP_CMD_RESET)
	{
		steppers->steppers[0].index = 0;
        steppers->steppers[0].cmd = STEP_CMD_NONE;
        steppers->steppers[0].arg = 0;
	}
	
	if (cmd2 == STEP_CMD_NONE || cmd2 == STEP_CMD_OFF)
	{
        steppers->steppers[1].arg = 0;
	}
	else if (cmd2 == STEP_CMD_OFF)
	{
		steppers->steppers[1].arg = 1;
	}
	else if (cmd2 == STEP_CMD_RESET)
	{
		steppers->steppers[1].index = 0;
        steppers->steppers[1].cmd = STEP_CMD_NONE;
        steppers->steppers[1].arg = 0;
	}
}

void WETAFUNCATTR
hw_stepper_reset(Steppers* steppers)
{
	steppers->steppers[0].cmd = STEP_CMD_NONE;
    steppers->steppers[1].cmd = STEP_CMD_NONE;
    steppers->steppers[0].arg = 0;
    steppers->steppers[1].arg = 0;
}

void WETAFUNCATTR
hw_stepper_apply(Steppers* steppers)
{
    if (steppers->steppers[0].cmd == STEP_CMD_OFF)
    {
        hw_gpio_set((steppers->steppers[0].pins[0]), false);
        hw_gpio_set((steppers->steppers[0].pins[1]), false);
        hw_gpio_set((steppers->steppers[0].pins[2]), false);
        hw_gpio_set((steppers->steppers[0].pins[3]), false);
    }
    else
    {
        hw_gpio_set((steppers->steppers[0].pins[0]), (steps[steppers->steppers[0].index][0]));
        hw_gpio_set((steppers->steppers[0].pins[1]), (steps[steppers->steppers[0].index][1]));
        hw_gpio_set((steppers->steppers[0].pins[2]), (steps[steppers->steppers[0].index][2]));
        hw_gpio_set((steppers->steppers[0].pins[3]), (steps[steppers->steppers[0].index][3]));
    }

    if (steppers->steppers[1].cmd == STEP_CMD_OFF)
    {
        hw_gpio_set((steppers->steppers[1].pins[0]), false);
        hw_gpio_set((steppers->steppers[1].pins[1]), false);
        hw_gpio_set((steppers->steppers[1].pins[2]), false);
        hw_gpio_set((steppers->steppers[1].pins[3]), false);
    }
    else
    {
        hw_gpio_set((steppers->steppers[1].pins[0]), (steps[steppers->steppers[1].index][0]));
        hw_gpio_set((steppers->steppers[1].pins[1]), (steps[steppers->steppers[1].index][1]));
        hw_gpio_set((steppers->steppers[1].pins[2]), (steps[steppers->steppers[1].index][2]));
        hw_gpio_set((steppers->steppers[1].pins[3]), (steps[steppers->steppers[1].index][3]));
    }
}

