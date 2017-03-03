#include <weta_platform.h>
#ifdef SUPPORT_STEPPERS
#include <hw.h>
#include "hw_time.h"


#define INTERSTEP_WAIT 2
#define INCREMENT_STEP(n) ((n) < STEPPER_STEPS-1 ? (n) + 1 : 0);
#define DECREMENT_STEP(n) ((n) == 0 ? STEPPER_STEPS-1 : (n) - 1); 

void hw_stepper_reset(Steppers* steppers);
void hw_stepper_apply(Steppers* steppers);
//uint8_t hw_increment_step(uint8_t val);


void WETAFUNCATTR
hw_stepper_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;

    hw_stepper_all_off(&hw->steppers);
}

void WETAFUNCATTR
hw_stepper_all_off(Steppers* steppers)
{
    int i;
    for (i = 0; i < steppers->n_steppers; i++)
    {
        steppers->steppers[i].index = 0;
        steppers->steppers[i].cmd = STEP_CMD_OFF;
        steppers->steppers[i].arg = 0;
    }
    hw_stepper_apply(steppers);
}

bool WETAFUNCATTR
hw_stepper_tick(Steppers* steppers)
{
	bool apply = false;
    bool done = true;
    int i;
    for (i = 0; i < steppers->n_steppers; i++)
    {
        if (steppers->steppers[i].arg)
        {
            if (steppers->steppers[i].cmd == STEP_CMD_FORWARD)
            {
                if (steppers->steppers[i].reverse)
                {
                    steppers->steppers[i].index = DECREMENT_STEP(steppers->steppers[i].index);
                }
                else
                {
                    steppers->steppers[i].index = INCREMENT_STEP(steppers->steppers[i].index);
                }
            }
            else if(steppers->steppers[i].cmd == STEP_CMD_BACKWARD)
            {
                if (steppers->steppers[i].reverse)
                {
                    steppers->steppers[i].index = INCREMENT_STEP(steppers->steppers[i].index);
                }
                else
                {
                    steppers->steppers[i].index = DECREMENT_STEP(steppers->steppers[i].index);
                }
            }
            apply = true;
            steppers->steppers[i].arg--;
            if (steppers->steppers[i].arg)
                done = false;

        }
    }
	if (apply)
	{
		hw_stepper_apply(steppers);
		hw_time_waitms(INTERSTEP_WAIT);
            // Turn off any motors that are finished with their last command
            // to save battery life.
        /*
        apply = false;
        for (i = 0; i < steppers->n_steppers; i++)
        {
                // Also check that they weren't just turned off
                // (to avoid an infinite loop)
            if (!steppers->steppers[i].arg && steppers->steppers[i].cmd != STEP_CMD_OFF)
            {
                steppers->steppers[i].cmd = STEP_CMD_OFF;
                apply = true;
            }
        }
        if (apply)
            hw_stepper_apply(steppers);
            */
	}
	return done;
}

void WETAFUNCATTR
hw_stepper_control(Steppers* steppers, StepperCommand* commands)
{
    int i;
    for (i = 0; i < steppers->n_steppers; i++)
    {
            // Leave stepper alone if command is STEP_CMD_NONE
        if (commands[i].cmd == STEP_CMD_NONE)
            continue;

        steppers->steppers[i].cmd = commands[i].cmd;
        if (commands[i].cmd == STEP_CMD_OFF)
        {
            steppers->steppers[i].arg = 1;
        }
        else if (commands[i].cmd == STEP_CMD_RESET)
        {
            steppers->steppers[i].index = 0;
            steppers->steppers[i].cmd = STEP_CMD_NONE;
            steppers->steppers[i].arg = 0;
        }
        else
        {
            steppers->steppers[i].arg = commands[i].arg;
        }
    }
}

void WETAFUNCATTR
hw_stepper_reset(Steppers* steppers)
{
    int i;
    for (i = 0; i < steppers->n_steppers; i++)
    {
        steppers->steppers[i].cmd = STEP_CMD_NONE;
        steppers->steppers[i].arg = 0;
    }
}

#ifdef STEPPERS_USE_SHIFTER
void WETAFUNCATTR
hw_stepper_apply(Steppers* steppers)
{
    int i;
    uint8_t bits;
    for (i = 0; i < steppers->n_steppers; i++)
    {
        bits = 0;
        int j;
        if (   steppers->steppers[i].cmd != STEP_CMD_OFF
            && steppers->steppers[i].cmd != STEP_CMD_NONE)
        {
            for (j = STEPPER_WIDTH - 1; j >= 0; j--)
                bits |= steppers->steps[steppers->steppers[i].index][j] << j;
        }
        hw_shift_out_byte(&steppers->shifter, bits, STEPPER_WIDTH, false);
    }
    hw_shift_strobe(&steppers->shifter);

}
#else
void WETAFUNCATTR
hw_stepper_apply(Steppers* steppers)
{
	int i;
    if (steppers->steppers[0].cmd == STEP_CMD_OFF)
    {
		for (i = 0; i < STEPPER_WIDTH; i++)
        	hw_gpio_set((steppers->steppers[0].pins[i]), false);
    }
    else
    {
        for (i = 0; i < STEPPER_WIDTH; i++)
            hw_gpio_set((steppers->steppers[0].pins[i]), (steppers->steps[steppers->steppers[0].index][i]));
    }

    if (steppers->steppers[1].cmd == STEP_CMD_OFF)
    {
        for (i = 0; i < STEPPER_WIDTH; i++)
            hw_gpio_set((steppers->steppers[1].pins[i]), false);
    }
    else
    {
        for (i = 0; i < STEPPER_WIDTH; i++)
            hw_gpio_set((steppers->steppers[1].pins[i]), (steppers->steps[steppers->steppers[1].index][i]));
    }
}
#endif // STEPPERS_USE_SHIFTER
#endif // SUPPORT_STEPPERS
