#ifndef __HW_STEPPER_H__
#define  __HW_STEPPER_H__

#ifdef SUPPORT_STEPPERS
#include <weta_platform.h>
#include <stdbool.h>

#include "hw_shifter.h"

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
	uint8_t 		stepper;
    eStepCommand    cmd;
    StepperArg      arg;
} StepperCommand;

typedef struct
{
    union
    {
        uint8_t pins[STEPPER_WIDTH];
        struct
        {
            uint8_t shifter;
            uint8_t unused1;
            uint8_t offset;
            uint8_t unused2;
        };
    };
    bool            reverse;
	uint8_t			index;
	eStepCommand	cmd;
	StepperArg		arg;
} StepperMotor;

typedef StepperMotor Steppers[MAX_STEPPERS];

typedef uint32_t StepperArg;
//typedef uint8_t  StepperStep[STEPPER_WIDTH];

struct _Hardware;

extern void hw_stepper_init(struct _Hardware* hw, uint16_t flags);
extern bool hw_stepper_config(
    struct _Hardware* hw,
    uint8_t stepper,
    bool    reverse,        // Is motor mounted backwards
    uint8_t a_or_shifter,   // Shifter if b is 0xFF, otherwise a
    uint8_t b,              // b of flag that above is shifter
    uint8_t c_or_offset,    // Offset into shifter if b is 0xFF. 0xFF if only 2 pins
    uint8_t d               // 0xFF if only 2 pins used
);
extern bool hw_stepper_tick(struct _Hardware* hw);
extern void hw_stepper_control(
    struct _Hardware* hw,
    StepperCommand* commands,
    uint8_t numCommands
);
extern void hw_stepper_all_off(struct _Hardware* hw);
#endif // SUPPORT_STEPPERS
#endif // __HW_STEPPER_H__
