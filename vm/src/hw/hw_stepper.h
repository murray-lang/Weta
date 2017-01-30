#ifndef __HW_STEPPER_H__
#define  __HW_STEPPER_H__

#include <weta_platform.h>
#include <stdbool.h>
#include "hw_gpio.h"

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
	GpioPin*		pins[STEPPER_WIDTH];
	uint8_t			index;
	eStepCommand	cmd;
	StepperArg		arg;
} StepperMotor;

typedef struct 
{
	StepperMotor*	steppers;
	uint8_t			n_steppers;
} Steppers;


typedef uint32_t StepperArg;
//typedef uint8_t  StepperStep[STEPPER_WIDTH];



extern bool hw_stepper_tick(Steppers* steppers);
extern void hw_stepper_init(Steppers* steppers, uint16_t flags);
extern void hw_stepper_control(Steppers* steppers, eStepCommand cmd1, StepperArg arg1, eStepCommand cmd2, StepperArg arg2);

#endif // __HW_STEPPER_H__
