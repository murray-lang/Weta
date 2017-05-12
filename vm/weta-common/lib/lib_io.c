#include <weta_platform.h>
#include <weta.h>
#include "../turtle/weta_turtle.h"
#include "lib_io.h"
#include "../WvmCodes.h"

void lib_io_digital_config(Weta* pWeta);
void lib_io_pwm_timer_config(Weta* pWeta);
void lib_io_pwm_channel_config(Weta* pWeta);
void lib_io_adc_config(Weta* pWeta);
void lib_io_adc_channel_config(Weta* pWeta);
void lib_io_dac_config(Weta* pWeta);
void lib_io_servo_config(Weta* pWeta);
void lib_io_motor_config(Weta* pWeta);
void lib_io_stepper_config(Weta* pWeta);
void lib_io_shifter_config(Weta* pWeta);

void WETAFUNCATTR
lib_io_handler(Weta* pWeta)
{
    if (weta_store_read_byte(pWeta->store,pWeta->regs.pc, &pWeta->regs.opCode) == 0)
    {
        DEBUGMSG("Unable to read byte from code store.\n");
        weta_reset(pWeta);
        return;
    }
    DEBUGMSG("lib_io_handler() - weta_store_read_byte(%d) = %d\r\n", pWeta->regs.pc, pWeta->regs.opCode);
    pWeta->regs.pc++;

    //sprintf(szMsg, "weta_store_read_byte(%d) = %d\r\n", loc, pWeta->regs.opCode);
    //weta_debug(pWeta, szMsg);
    switch (pWeta->regs.opCode)
    {
    case OP_IO_DIGITAL_CONFIG:
        lib_io_digital_config(pWeta);
        break;

    case OP_IO_PWM_TIMER_CONFIG:
        lib_io_pwm_timer_config(pWeta);
        break;

    case OP_IO_PWM_CHANNEL_CONFIG:
        lib_io_pwm_channel_config(pWeta);
        break;

    case OP_IO_ADC_CONFIG:
        lib_io_adc_config(pWeta);
        break;

    case OP_IO_ADC_CHANNEL_CONFIG:
        lib_io_adc_channel_config(pWeta);
        break;

    case OP_IO_DAC_CONFIG:
        lib_io_dac_config(pWeta);
        break;

    case OP_IO_SERVO_CONFIG:
        lib_io_servo_config(pWeta);
        break;

    case OP_IO_MOTOR_CONFIG:
        lib_io_motor_config(pWeta);
        break;

    case OP_IO_STEPPER_CONFIG:
        lib_io_stepper_config(pWeta);
        break;

    case OP_IO_SHIFTER_CONFIG:
        lib_io_shifter_config(pWeta);
        break;
#ifdef SUPPORT_MOTORS
    case OP_IO_MOTOR_SELECT:
        {
            DEBUGMSG( "io.motor.select\r\n");
            uint8_t selected;
            weta_stack_popUint8(pWeta->stack, &selected);
            weta_motor_select(pWeta->hal, selected);
        }
        break;

    case OP_IO_MOTOR_ON:
        DEBUGMSG( "io.motor.on\r\n");
        weta_motor_on(pWeta->hal, true);
        break;

    case OP_IO_MOTOR_ONFOR:
        {
            DEBUGMSG( "io.motor.onfor\r\n");
            uint16_t tenths;
            weta_stack_popUint16(pWeta->stack, &tenths);
            weta_motor_on(pWeta->hal, true);
            hw_time_waitms(tenths * 100);
            weta_motor_on(pWeta->hal, false);
        }
        break;

    case OP_IO_MOTOR_OFF:
        DEBUGMSG( "io.motor.off\r\n");
        weta_motor_on(pWeta->hal, false);
        break;

    case OP_IO_MOTOR_THISWAY:
        DEBUGMSG( "io.motor.thisway\r\n");
        weta_motor_dir(pWeta->hal, MOTOR_THIS_WAY);
        break;

    case OP_IO_MOTOR_THATWAY:
        DEBUGMSG( "io.motor.thatway\r\n");
        weta_motor_dir(pWeta->hal, MOTOR_THAT_WAY);
        break;

    case OP_IO_MOTOR_RD:
        DEBUGMSG( "io.motor.rd\r\n");
        weta_motor_rd(pWeta->hal);
        break;

    case OP_IO_MOTOR_SETPOWER:
        {
            DEBUGMSG( "io.motor.setpower\r\n");
            uint8_t power;
            weta_stack_popUint8(pWeta->stack, &power);
            //sprintf(szMsg, "OP_SETPOWER to %d\r\n", power);
            //weta_debug(pWeta, szMsg);
            weta_motor_power(pWeta->hal, power);
        }
        break;

    case OP_IO_MOTOR_BRAKE:
        DEBUGMSG( "io.motor.brake\r\n");
        weta_motor_brake(pWeta->hal, true);
        break;
#endif // SUPPORT_MOTORS

#ifdef SUPPORT_SERVOS
    case OP_IO_SERVO_SELECT:
        {
            uint8_t selected;
            weta_stack_popUint8(pWeta->stack, &selected);
            weta_servo_select(pWeta->hal, selected);
        }
        break;

    case OP_IO_SERVO_HEADING:
        {
            int16_t heading;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&heading);
            weta_servo_pos(pWeta->hal, heading);
        }
        break;

    case OP_IO_SERVO_RIGHT:
        {
            int16_t amount;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&amount);
            weta_servo_right(pWeta->hal, amount);
        }
        break;
    case OP_IO_SERVO_LEFT:
        {
            int16_t amount;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&amount);
            weta_servo_left(pWeta->hal, amount);
        }
        break;
#endif // SUPPORT_SERVOS

    case OP_IO_TURTLE_FORWARD:
        {
#ifdef	SUPPORT_STEPPERS
            pWeta->states.machineState = STEPPER;
#endif
            int16_t mm;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&mm);
            weta_turtle_forward(pWeta->hal, mm);
        }
        break;

    case OP_IO_TURTLE_BACKWARD:
        {
#ifdef	SUPPORT_STEPPERS
            pWeta->states.machineState = STEPPER;
#endif
            int16_t mm;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&mm);
            weta_turtle_backward(pWeta->hal, mm);
        }
        break;

    case OP_IO_TURTLE_LEFT:
        {
#ifdef	SUPPORT_STEPPERS
            pWeta->states.machineState = STEPPER;
#endif
            int16_t degrees;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&degrees);
            weta_turtle_left(pWeta->hal, degrees);
        }
        break;

    case OP_IO_TURTLE_RIGHT:
        {
#ifdef	SUPPORT_STEPPERS
            pWeta->states.machineState = STEPPER;
#endif
            int16_t degrees;
            weta_stack_popUint16(pWeta->stack, (uint16_t*)&degrees);
            weta_turtle_right(pWeta->hal, degrees);
        }
        break;

    case OP_IO_TURTLE_PENUP:
        weta_turtle_penup();
        break;

    case OP_IO_TURTLE_PENDOWN:
        weta_turtle_penup();
        break;
    }
}

void WETAFUNCATTR
lib_io_digital_config(Weta* pWeta)
{
    uint8_t options;
    uint8_t gpio_num;
    uint8_t index;

    weta_stack_popUint8(pWeta->stack, &options);
    weta_stack_popUint8(pWeta->stack, &gpio_num);
    weta_stack_popUint8(pWeta->stack, &index);

    DEBUGMSG("hw_gpio_config(%d, %d, %0X)\r\n", index, gpio_num, options);
    hw_gpio_config(pWeta->hal, index, gpio_num, options);
}

void WETAFUNCATTR
lib_io_pwm_timer_config(Weta* pWeta)
{
    uint8_t  width;
    uint16_t frequency;
    uint8_t  timer;

    weta_stack_popUint8(pWeta->stack, &width);
    weta_stack_popUint16(pWeta->stack, &frequency);
    weta_stack_popUint8(pWeta->stack, &timer);

    DEBUGMSG("hw_pwm_config_timer(%d, %d, %d)\r\n", timer, frequency, width);
    hw_pwm_config_timer(pWeta->hal, timer, frequency, width);
}

void WETAFUNCATTR
lib_io_pwm_channel_config(Weta* pWeta)
{
    uint8_t  channel;
    uint8_t  gpio_num;
    uint8_t  timer;

    weta_stack_popUint8(pWeta->stack, &timer);
    weta_stack_popUint8(pWeta->stack, &gpio_num);
    weta_stack_popUint8(pWeta->stack, &channel);

    DEBUGMSG("hw_pwm_config_channel(%d, %d, %d)\r\n", channel, gpio_num, timer);
    hw_pwm_config_channel(pWeta->hal, channel, gpio_num, timer);
}

void WETAFUNCATTR
lib_io_adc_config(Weta* pWeta)
{
    uint8_t  depth;
    weta_stack_popUint8(pWeta->stack, &depth);

    hw_adc_config(pWeta->hal, depth);
}

void WETAFUNCATTR
lib_io_adc_channel_config(Weta* pWeta)
{
    uint8_t  channel;
    int8_t   gain;
    weta_stack_popUint8(pWeta->stack, (uint8_t*)&gain);
    weta_stack_popUint8(pWeta->stack, &channel);

    hw_adc_channel_config(pWeta->hal, channel, gain);
}

void WETAFUNCATTR
lib_io_dac_config(Weta* pWeta)
{

}

void WETAFUNCATTR
lib_io_servo_config(Weta* pWeta)
{
    int16_t  dutyper10; // Note signed
    uint16_t  duty0;
    uint16_t  maxduty;
    uint16_t  minduty;
    uint8_t  pwm;
    uint8_t  servo;

        // Although the stack pops unsigned, the result is still signed because
        // there's no interpretation of the value internally
    weta_stack_popUint16(pWeta->stack, (uint16_t*)&dutyper10);
    weta_stack_popUint16(pWeta->stack, &duty0);
    weta_stack_popUint16(pWeta->stack, &maxduty);
    weta_stack_popUint16(pWeta->stack, &minduty);
    weta_stack_popUint8(pWeta->stack, &pwm);
    weta_stack_popUint8(pWeta->stack, &servo);

    hw_servo_config(pWeta->hal, servo, pwm, minduty, maxduty, duty0, dutyper10);
}

void WETAFUNCATTR
lib_io_motor_config(Weta* pWeta)
{
    uint8_t  motor;
    uint8_t  pwm;
    uint8_t  a_or_dir;
    uint8_t  b;

    weta_stack_popUint8(pWeta->stack, &b);
    weta_stack_popUint8(pWeta->stack, &a_or_dir);
    weta_stack_popUint8(pWeta->stack, &pwm);
    weta_stack_popUint8(pWeta->stack, &motor);

    DEBUGMSG("hw_motor_config(%d, %d, %d, %d)\r\n", motor, pwm, a_or_dir, b);
    hw_motor_config(pWeta->hal, motor, pwm, a_or_dir, b);
}

void WETAFUNCATTR
lib_io_stepper_config(Weta* pWeta)
{
    uint8_t  d;
    uint8_t  c_or_offset;
    uint8_t  b;
    uint8_t  a_or_shifter;
    uint8_t  reverse;
    uint8_t  stepper;

    weta_stack_popUint8(pWeta->stack, &d);
    weta_stack_popUint8(pWeta->stack, &c_or_offset);
    weta_stack_popUint8(pWeta->stack, &b);
    weta_stack_popUint8(pWeta->stack, &a_or_shifter);
    weta_stack_popUint8(pWeta->stack, &reverse);
    weta_stack_popUint8(pWeta->stack, &stepper);

    hw_stepper_config(pWeta->hal, stepper, (bool)reverse, a_or_shifter, b, c_or_offset, d);
}
void WETAFUNCATTR
lib_io_shifter_config(Weta* pWeta)
{
    uint8_t  strobe;
    uint8_t  clock;
    uint8_t  data;
    uint8_t  width;
    uint8_t  shifter;

    weta_stack_popUint8(pWeta->stack, &strobe);
    weta_stack_popUint8(pWeta->stack, &clock);
    weta_stack_popUint8(pWeta->stack, &data);
    weta_stack_popUint8(pWeta->stack, &width);
    weta_stack_popUint8(pWeta->stack, &shifter);

    hw_shift_config(pWeta->hal, shifter, width, data, clock, strobe);
}
