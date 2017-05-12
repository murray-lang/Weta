#include <weta_platform.h>

#ifdef SUPPORT_STEPPERS

#include <hw.h>
#include "hw_time.h"

#define stepper_uses_shifter(s) ((s)->pins[1] == 0xFF)
#define stepper_uses_2_pins(s) ((s)->pins[2] == 0xFF)

static const uint8_t steps4wide[STEPPER_STEPS][4] =
    {
        {1, 0, 0, 1},
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1}
    };

static const uint8_t steps2wide[STEPPER_STEPS][2] =
    {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };

#define INTERSTEP_WAIT 2
#define INCREMENT_STEP(n) ((n) < STEPPER_STEPS-1 ? (n) + 1 : 0);
#define DECREMENT_STEP(n) ((n) == 0 ? STEPPER_STEPS-1 : (n) - 1);

void hw_stepper_reset(struct _Hardware *hw);

void hw_stepper_apply(struct _Hardware *hw);

void hw_stepper_apply_shifter(struct _Hardware *hw, uint8_t i);

void hw_stepper_apply_2_pins(struct _Hardware *hw, uint8_t i);
void hw_stepper_apply_4_pins(struct _Hardware *hw, uint8_t i);


void WETAFUNCATTR
hw_stepper_init(struct _Hardware *hw, uint16_t flags)
{
    flags = flags;

    hw_stepper_all_off(hw);
}

bool WETAFUNCATTR
hw_stepper_config(
    struct _Hardware* hw,
    uint8_t stepperid,
    bool    reverse,        // Is motor mounted backwards
    uint8_t a_or_shifter,   // Shifter if b is 0xFF, otherwise a
    uint8_t b,              // b of flag that above is shifter
    uint8_t c_or_offset,    // Offset into shifter if b is 0xFF. 0xFF if only 2 pins
    uint8_t d               // 0xFF if only 2 pins used
)
{
    if (stepperid >= MAX_STEPPERS)
        return false;
    StepperMotor* stepper = &hw->steppers[stepperid];

    stepper->reverse = reverse;
    stepper->pins[1] = b;
    if (b == 0xFF)
    {
            // We've been given a shifter definition
        stepper->shifter = a_or_shifter;
        stepper->unused1 = 0xFF;
        stepper->offset  = c_or_offset;
        stepper->unused2 = 0xFF;
    }
    else
    {
            // Either 2 or 4 pin stepper control
            // 2 pin control if c_or_offset == 0xFF
        stepper->pins[0] = a_or_shifter;
        stepper->pins[2] = c_or_offset;
        stepper->pins[3] = d;
    }
    stepper->cmd = STEP_CMD_OFF;
    stepper->arg = 0;
    stepper->index = 0;
    return true;
}

void WETAFUNCATTR
hw_stepper_all_off(struct _Hardware *hw)
{
    int i;
    for (i = 0; i < MAX_STEPPERS; i++) {
        if (hw->steppers[i].pins[0] != 0xFF) {
            hw->steppers[i].index = 0;
            hw->steppers[i].cmd = STEP_CMD_OFF;
            hw->steppers[i].arg = 0;
        }
    }
    hw_stepper_apply(hw);
}

bool WETAFUNCATTR
hw_stepper_tick(struct _Hardware *hw)
{
    bool apply = false;
    bool done = true;
    int i;
    for (i = 0; i < MAX_STEPPERS; i++) {

        StepperMotor *stepper = &hw->steppers[i];
        if (stepper->pins[0] == 0xFF) {
            continue;
        }
        if (stepper->arg) {
            if (stepper->cmd == STEP_CMD_FORWARD) {
                if (stepper->reverse) {
                    stepper->index = DECREMENT_STEP(stepper->index);
                } else {
                    stepper->index = INCREMENT_STEP(stepper->index);
                }
            } else if (stepper->cmd == STEP_CMD_BACKWARD) {
                if (stepper->reverse) {
                    stepper->index = INCREMENT_STEP(stepper->index);
                } else {
                    stepper->index = DECREMENT_STEP(stepper->index);
                }
            }
            apply = true;
            stepper->arg--;
            if (stepper->arg) {
                done = false;
            }

        }
    }
    if (apply) {
        hw_stepper_apply(hw);
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
    if (done)
        hw_stepper_all_off(hw);
    return done;
}

void WETAFUNCATTR
hw_stepper_control(
    struct _Hardware *hw,
    StepperCommand   *commands,
    uint8_t          numCommands
)
{
    DEBUGMSG("hw_stepper_control()\r\n");
    int i;
    for (i = 0; i < numCommands; i++) {
        // Leave stepper alone if command is STEP_CMD_NONE
        if (commands[i].cmd == STEP_CMD_NONE) {
            continue;
        }
        uint8_t j = commands[i].stepper;
        if (j >= MAX_STEPPERS) {
            continue;
        }
        StepperMotor *stepper = &hw->steppers[j];
        if (stepper->pins[0] == 0xFF) {
            continue;
        }
        stepper->cmd = commands[i].cmd;
        if (commands[i].cmd == STEP_CMD_OFF) {
            stepper->arg = 1;
        } else if (commands[i].cmd == STEP_CMD_RESET) {
            stepper->index = 0;
            stepper->cmd = STEP_CMD_NONE;
            stepper->arg = 0;
        } else {
            stepper->arg = commands[i].arg;
        }
    }
}

void WETAFUNCATTR
hw_stepper_reset(struct _Hardware *hw)
{
    int i;
    for (i = 0; i < MAX_STEPPERS; i++) {
        hw->steppers[i].cmd = STEP_CMD_NONE;
        hw->steppers[i].arg = 0;
    }
}

void WETAFUNCATTR
hw_stepper_apply(struct _Hardware *hw)
{
    uint8_t i;
    bool anyShifters = false;
    for (i = 0; i < MAX_STEPPERS; i++) {
        StepperMotor *stepper = &hw->steppers[i];
        if (stepper_uses_shifter(stepper)) {
            hw_stepper_apply_shifter(hw, i);
            anyShifters = true;
        } else if (stepper_uses_2_pins(stepper)) {
            hw_stepper_apply_2_pins(hw, i);
        } else {
            hw_stepper_apply_4_pins(hw, i);
        }
    }
        // We need to go through again and shift out any data put into shifters
    if (anyShifters) {
        for (i = 0; i < MAX_STEPPERS; i++) {
            StepperMotor *stepper = &hw->steppers[i];
            if (stepper->pins[0] != 0xFF && stepper_uses_shifter(stepper)) {
                    // 0xFF means shift everything out (shifter will limit it)
                hw_shift_out(hw, stepper->shifter, 0xFF, true);
            }
        }
    }
}

void WETAFUNCATTR
hw_stepper_apply_shifter(struct _Hardware *hw, uint8_t i)
{
    StepperMotor *stepper = &hw->steppers[i];
    if (stepper->pins[0] == 0xFF) {
        //DEBUGMSG("hw_stepper_apply_shifter(%d) - pins not configured\r\n", i);
        return;
    }
    uint8_t bits = 0;
    if (stepper->cmd != STEP_CMD_OFF && stepper->cmd != STEP_CMD_NONE) {
        int j;
        for (j = 3; j >= 0; j--) {
            bits |= steps4wide[stepper->index][j] << j;
        }
    } // else bits stays 0
    hw_shift_set_buffer(hw, stepper->shifter, bits, stepper->offset, 4);
}

void WETAFUNCATTR
hw_stepper_apply_2_pins(struct _Hardware *hw, uint8_t i)
{
    StepperMotor *stepper = &hw->steppers[i];
    if (stepper->pins[0] == 0xFF) {
        return;
    }
    int j;
    if (stepper->cmd == STEP_CMD_OFF) {
        for (j = 0; j < 2; j++) {
            hw_gpio_set(hw, stepper->pins[j], false);
        }
    } else {
        for (j = 0; j < 2; j++) {
            hw_gpio_set(hw, stepper->pins[j], steps2wide[stepper->index][j]);
        }
    }
}

void WETAFUNCATTR
hw_stepper_apply_4_pins(struct _Hardware *hw, uint8_t i)
{
    StepperMotor *stepper = &hw->steppers[i];
    if (stepper->pins[0] == 0xFF) {
        return;
    }
    int j;
    if (stepper->cmd == STEP_CMD_OFF) {
        for (j = 0; j < 4; j++) {
            hw_gpio_set(hw, stepper->pins[j], false);
        }
    } else {
        for (j = 0; j < 4; j++) {
            hw_gpio_set(hw, stepper->pins[j], steps4wide[stepper->index][j]);
        }
    }
}

#endif // SUPPORT_STEPPERS
