#include "weta_turtle.h"
#include "../hw/hw.h"


// TO DO: Calibrate scale factor for distance travelled
// The scale is defined as numerator and denominator instead of floating point
// because tools for smaller microcontrollers might not support floating point.
#define DISTANCE_SCALE_NUMERATOR 321
#define DISTANCE_SCALE_DENOMINATOR 101

#define TURN_SCALE_NUMERATOR 300
#define TURN_SCALE_DENOMINATOR 100

#define SCALE_DISTANCE(mm) (((mm)*DISTANCE_SCALE_NUMERATOR)/DISTANCE_SCALE_DENOMINATOR)
#define SCALE_TURN(degrees) (((degrees)*TURN_SCALE_NUMERATOR)/TURN_SCALE_DENOMINATOR)

void WETAFUNCATTR
weta_turtle_forward(Hardware* hw, int16_t mm)
{
    DEBUGMSG("weta_turtle_forward %d\r\n", mm);
    if (mm < 0)
    {
        weta_turtle_backward(hw, -mm);
        return;
    }
#ifdef	SUPPORT_STEPPERS
    StepperCommand cmds[] =
        {
            { .stepper = 0, .cmd = STEP_CMD_FORWARD, .arg = SCALE_DISTANCE(mm) },
            { .stepper = 1, .cmd = STEP_CMD_FORWARD, .arg = SCALE_DISTANCE(mm) }
        };
    hw_stepper_control(hw, cmds, 2);
#endif
}

void WETAFUNCATTR
weta_turtle_backward(Hardware* hw, int16_t mm)
{
    DEBUGMSG("weta_turtle_backward %d\r\n", mm);
    if (mm < 0)
    {
        weta_turtle_forward(hw, -mm);
        return;
    }
#ifdef	SUPPORT_STEPPERS
    StepperCommand cmds[] =
        {
            { .stepper = 0, .cmd = STEP_CMD_BACKWARD, .arg = SCALE_DISTANCE(mm) },
            { .stepper = 1, .cmd = STEP_CMD_BACKWARD, .arg = SCALE_DISTANCE(mm) }
        };
    hw_stepper_control(hw, cmds, 2);
#endif
}

void WETAFUNCATTR
weta_turtle_left(Hardware* hw, int16_t degrees)
{
    if (degrees < 0)
    {
        weta_turtle_right(hw, -degrees);
        return;
    }
    // TO DO
    // Simple test for now
#ifdef	SUPPORT_STEPPERS
    StepperCommand cmds[] =
        {
            { .stepper = 0, .cmd = STEP_CMD_FORWARD, .arg = SCALE_TURN(degrees) },
            { .stepper = 1, .cmd = STEP_CMD_BACKWARD, .arg = SCALE_TURN(degrees) }
        };
    hw_stepper_control(hw, cmds, 2);
#endif
}

void WETAFUNCATTR
weta_turtle_right(Hardware* hw, int16_t degrees)
{
    if (degrees < 0)
    {
        weta_turtle_left(hw, -degrees);
        return;
    }
    // TO DO
#ifdef	SUPPORT_STEPPERS
    StepperCommand cmds[] =
        {
            { .stepper = 0, .cmd = STEP_CMD_BACKWARD, .arg = SCALE_TURN(degrees) },
            { .stepper = 1, .cmd = STEP_CMD_FORWARD, .arg = SCALE_TURN(degrees) }
        };
    hw_stepper_control(hw, cmds, 2);
#endif
}

void WETAFUNCATTR
weta_turtle_penup(void)
{
    // TO DO
}

void WETAFUNCATTR
weta_turtle_pendown(void)
{
    // TO DO
}