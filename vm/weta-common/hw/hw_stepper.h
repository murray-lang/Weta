#ifndef __HW_STEPPER_H__
#define  __HW_STEPPER_H__

#ifdef SUPPORT_STEPPERS
#include <weta_platform.h>
#include <stdbool.h>
#ifdef STEPPERS_USE_SHIFTER
#include "hw_shifter.h"
#else
#include "hw_gpio.h"
#endif

typedef enum
{
	STEP_CMD_NONE,
	STEP_CMD_OFF,
	STEP_CMD_FORWARD,
	STEP_CMD_BACKWARD,
	STEP_CMD_RESET
} eStepCommand;

typedef uint32_t StepperArg;

typedef struct
{
    eStepCommand    cmd;
    StepperArg      arg;
} StepperCommand;

typedef struct
{
#ifndef STEPPERS_USE_SHIFTER
	GpioPin*		pins[STEPPER_WIDTH];
#endif
    bool            reverse;
	uint8_t			index;
	eStepCommand	cmd;
	StepperArg		arg;
} StepperMotor;

typedef struct 
{
	StepperMotor*	steppers;
	uint8_t			n_steppers;
	const uint8_t   steps[STEPPER_STEPS][STEPPER_WIDTH];
#ifdef STEPPERS_USE_SHIFTER
    Shifter         shifter;
#endif
} Steppers;


typedef uint32_t StepperArg;
//typedef uint8_t  StepperStep[STEPPER_WIDTH];

struct _Hardware;

extern void hw_stepper_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_stepper_tick(Steppers* steppers);
extern void hw_stepper_control(Steppers* steppers, StepperCommand* commands);
#endif // SUPPORT_STEPPERS
#endif // __HW_STEPPER_H__
