#include "hw.h"

void WETAFUNCATTR
hw_motor_init(struct _Hardware *hw, uint16_t flags)
{
    flags = flags;
    hw_motor_all_off(hw);
}

bool WETAFUNCATTR
hw_motor_config(
    struct _Hardware *hw, uint8_t motorid, uint8_t pwm, uint8_t a_or_dir, uint8_t b
)
{
    if (motorid >= MAX_MOTORS) {
        return false;
    }

    Motor *motor = &hw->motors[motorid];
    motor->pins.a = a_or_dir;
    motor->pins.b = b;
    motor->pins.pwm = pwm;

    motor->state.selected = false;
    motor->state.on = false;
    motor->state.brake = false;
    motor->state.dir = MOTOR_THIS_WAY;
    motor->state.power = 0;
    return true;
}

void WETAFUNCATTR
hw_motor_select(struct _Hardware *hw, uint8_t select)
{
    int8_t i;
    for (i = 0; i < MAX_MOTORS; i++) {
        if (hw->motors[i].pins.a != 0xFF) {
            //DEBUGMSG("Motor %d...", i);
            bool selected = !!(select & (1 << i));
            //DEBUGMSG(selected ? "selected" : "not selected");
            hw->motors[i].state.selected = selected;
            //DEBUGMSG("...done\r\n");
        }
    }
}

void WETAFUNCATTR
hw_motor_update(struct _Hardware *hw)
{
    int8_t i;
    for (i = 0; i < MAX_MOTORS; ++i) {
        if (hw->motors[i].pins.a == 0xFF) {
            continue;
        }
        MotorState *state = &hw->motors[i].state;
        if (state->selected) {
            MotorPins *pins = &hw->motors[i].pins;
                // If b == 0xFF then a is in fact a direction pin
            bool usedir = pins->b == 0xFF;
            if (!state->on) {
                if (usedir) {
                    // Can't turn the motor off with only a dir pin so
                    // turn the PWM off instead
                    hw_pwm_set_duty(pins->pwm, (WetaPwmDuty) 0);
                } else {
                    hw_gpio_set(hw, pins->a, false);
                    hw_gpio_set(hw, pins->b, false);
                }
            } else if (state->brake) {
                if (usedir) {
                    // No braking with only dir pin. Just turn off PWM
                    hw_pwm_set_duty(pins->pwm, (WetaPwmDuty) 0);
                } else {
                    hw_gpio_set(hw, pins->a, true);
                    hw_gpio_set(hw, pins->b, true);
                }
            } else {
                if (state->dir == MOTOR_THIS_WAY) {
                    if (usedir) {
                        hw_gpio_set(hw, pins->dir, true);
                    } else {
                        hw_gpio_set(hw, pins->a, true);
                        hw_gpio_set(hw, pins->b, false);
                    }
                } else {
                    if (usedir) {
                        hw_gpio_set(hw, pins->dir, false);
                    } else {
                        hw_gpio_set(hw, pins->a, false);
                        hw_gpio_set(hw, pins->b, true);
                    }
                }
                //DEBUGMSG("hw_motor_update(): Motor %d duty = %d\r\n", i, state->power);
                hw_pwm_set_duty(pins->pwm, (WetaPwmDuty) state->power);
            }
        }
    }
}

void WETAFUNCATTR
hw_motor_all_off(struct _Hardware *hw)
{
    hw_motor_select(hw, 0xFF);
    hw_motor_on(hw, false);
}

void WETAFUNCATTR
hw_motor_on(struct _Hardware *hw, bool on)
{
    //DEBUGMSG("hw_motor_on(%d)\r\n", on);

    int8_t i;
    for (i = 0; i < MAX_MOTORS; ++i) {
        if (hw->motors[i].pins.a == 0xFF) {
            continue;
        }
        if (hw->motors[i].state.selected) {
            hw->motors[i].state.on = on;
            if (on) {
                hw->motors[i].state.brake = false;
            }
        }
    }
    hw_motor_update(hw);
}

void WETAFUNCATTR
hw_motor_brake(struct _Hardware *hw, bool brake)
{
    int8_t i;
    for (i = 0; i < MAX_MOTORS; ++i) {
        if (hw->motors[i].pins.a == 0xFF) {
            continue;
        }
        if (hw->motors[i].state.selected) {
            hw->motors[i].state.brake = brake;
            if (brake) {
                hw->motors[i].state.on = false;
            }
        }
    }
    hw_motor_update(hw);
}

void WETAFUNCATTR
hw_motor_direction(struct _Hardware *hw, MotorDirection dir)
{
    int8_t i;
    for (i = 0; i < MAX_MOTORS; ++i) {
        if (hw->motors[i].pins.a == 0xFF) {
            continue;
        }
        if (hw->motors[i].state.selected) {
            hw->motors[i].state.dir = dir;
        }
    }

    hw_motor_update(hw);
}

void WETAFUNCATTR
hw_motor_reverse(struct _Hardware *hw)
{
    int8_t i;
    for (i = 0; i < MAX_MOTORS; ++i) {
        if (hw->motors[i].pins.a == 0xFF) {
            continue;
        }
        if (hw->motors[i].state.selected) {
            if (hw->motors[i].state.dir == MOTOR_THIS_WAY) {
                hw->motors[i].state.dir = MOTOR_THAT_WAY;
            } else {
                hw->motors[i].state.dir = MOTOR_THIS_WAY;
            }
        }
    }
    hw_motor_update(hw);
}

void WETAFUNCATTR
hw_motor_power(struct _Hardware *hw, MotorPower power)
{
    //DEBUGMSG("hw_motor_power(%d)\r\n", power);
    int8_t i;
    for (i = 0; i < MAX_MOTORS; ++i) {
        if (hw->motors[i].pins.a == 0xFF) {
            continue;
        }
        if (hw->motors[i].state.selected) {
            hw->motors[i].state.power = power;
        }
    }

    hw_motor_update(hw);
}
