//
// Created by murray on 13/01/17.
//
#include <weta_platform.h>
#include "board.h"
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/uart.h>
#include <driver/adc.h>
#include <weta.h>
#ifdef SUPPORT_QUERY
#include <stdio.h>  // for sprintf()
#include <string.h>
#endif

SerialPort serialPorts[2] =
    {
        { 0, 115200, (uint8_t)UART_DATA_8_BITS, (uint8_t)UART_PARITY_DISABLE, (uint8_t)UART_STOP_BITS_1 },
        { 1, 9600, (uint8_t)UART_DATA_8_BITS, (uint8_t)UART_PARITY_DISABLE, (uint8_t)UART_STOP_BITS_1 }
    };

DebounceState debounce[1] =
    {
        {false, false, false, true, 0}
    };

GpioPin gpio[] =
    {
        { PIN_USER_LED,     GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_RUN,          GPIO_MODE_INPUT,  GPIO_PULLUP_ONLY, &debounce[0] },
        { PIN_SWITCH1,      GPIO_MODE_INPUT,  GPIO_PULLUP_ONLY, NULL },
        { PIN_SWITCH2,      GPIO_MODE_INPUT,  GPIO_PULLUP_ONLY, NULL },
        { PIN_SWITCH3,      GPIO_MODE_INPUT,  GPIO_PULLUP_ONLY, NULL },
        { PIN_SWITCH4,      GPIO_MODE_INPUT,  GPIO_PULLUP_ONLY, NULL },
        { PIN_MOTOR_0_DIR,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_MOTOR_1_DIR,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_0_A,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_0_B,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_0_C,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_0_D,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_1_A,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_1_B,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_1_C,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
        { PIN_STEPPER_1_D,  GPIO_MODE_OUTPUT, GPIO_FLOATING,    NULL },
    };

GpioPin* debounced_gpios[] = { &gpio[GPIO_INDEX_RUN] };

PwmChannel pwm[] =
    {
        { LEDC_CHANNEL_1, PIN_MOTOR_0_PWM },
        { LEDC_CHANNEL_2, PIN_MOTOR_1_PWM },
        { LEDC_CHANNEL_3, PIN_SERVO_0_PWM },
        { LEDC_CHANNEL_4, PIN_SERVO_1_PWM }
    };

Motor motors[] =
    {
        {
            { &gpio[GPIO_INDEX_MOTOR_0_DIR],  &pwm[0] },
            { 0, false, false, false, MOTOR_THIS_WAY, 100 }
        },
        {
            { &gpio[GPIO_INDEX_MOTOR_1_DIR],  &pwm[1] },
            { 1, false, false, false, MOTOR_THIS_WAY, 100 }
        }
    };

StepperMotor steppers[] =
    {
        {
            {
                &gpio[GPIO_INDEX_STEPPER_0_A],
                &gpio[GPIO_INDEX_STEPPER_0_B],
                &gpio[GPIO_INDEX_STEPPER_0_C],
                &gpio[GPIO_INDEX_STEPPER_0_D]
            },
            0, STEP_CMD_OFF, 0
        },
        {
            {
                &gpio[GPIO_INDEX_STEPPER_1_A],
                &gpio[GPIO_INDEX_STEPPER_1_B],
                &gpio[GPIO_INDEX_STEPPER_1_C],
                &gpio[GPIO_INDEX_STEPPER_1_D]
            },
            0, STEP_CMD_OFF, 0
        }
    };

ServoState servos[] =
    {
        { &pwm[2], 0, false, 0, { 110, 300, 490, -20 } },
        { &pwm[3], 1, false, 0, { 110, 300, 490, -20 } }
    };

AdcChannel adc_channels[] =
    {
        { (WetaAdcChannel)ADC1_CHANNEL_0, (WetaAdcAtten)ADC_ATTEN_0db },
        { (WetaAdcChannel)ADC1_CHANNEL_3, (WetaAdcAtten)ADC_ATTEN_6db }
    };

// Use RAM as program storage for now
//uint8_t fakeFlash[1024];

Hardware hardware =
    {
        (uint8_t*)FLASH_USER_START,
        FLASH_USER_LENGTH,
        {
            serialPorts,
            sizeof(serialPorts) / sizeof(SerialPort)
        },
        {
            motors,
            sizeof(motors) / sizeof(Motor)
        },
        {
            steppers,
            sizeof(steppers) / sizeof(StepperMotor)
        },
        {
            servos,
            sizeof(servos) / sizeof(ServoState)
        },
        {
            gpio,
            sizeof(gpio) / sizeof(GpioPin),
            debounced_gpios,
            sizeof(debounced_gpios) / sizeof(GpioPin*)
        },
        {
            pwm,
            sizeof(pwm) / sizeof(PwmChannel)
        },
        {
            adc_channels,
            sizeof(adc_channels) / sizeof(AdcChannel),
            (WetaAdcDepth)ADC_WIDTH_10Bit
        }
    };

#ifdef SUPPORT_QUERY
bool WETAFUNCATTR
weta_query(Weta* weta, WetaQuery q, char * json, uint16_t length)
{
    uint8_t dataBits[] = {5, 6, 7, 8};
    char* stopBits[] = {"1", "1.5", "2"};
    char* parity[]   = {"none", "even", "odd"};
        // TODO: Deal with length parameter
    strcat(json, "{");

    if (q & QUERY_DIGITAL)
    {
        strcat(json, "\"digital\":{\"input\":[");
        bool value = false;
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_RUN, &value);
        strcat(json, value ? "1," : "0,");
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_SWITCH1, &value);
        strcat(json, value ? "1," : "0,");
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_SWITCH2, &value);
        strcat(json, value ? "1," : "0,");
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_SWITCH3, &value);
        strcat(json, value ? "1," : "0,");
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_SWITCH4, &value);
        strcat(json, value ? "1]" : "0]");

        strcat(json, ",\"output\":[");
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_LED, &value);
        strcat(json, value ? "1]" : "0]");
        strcat(json, "}");
    }
    if (q & QUERY_ANALOG)
    {
        if (q & QUERY_DIGITAL)
            strcat(json, ",");
        int16_t value0 = 0;
        int16_t value1 = 0;
        hw_adc_get(&weta->hal->adc, 0, &value0);
        hw_adc_get(&weta->hal->adc, 1, &value1);
        sprintf(json + strlen(json), "\"analog\":{\"input\":[%d,%d]}",
            value0, value1
        );
    }
    if (q & QUERY_SERIAL)
    {
        if (q & (QUERY_DIGITAL | QUERY_ANALOG))
            strcat(json, ",");
        strcat(json, "\"serial\":[");
        int i;
        for (i = 0; i < weta->hal->sports.n_ports; i++)
        {
            sprintf(json + strlen(json),
                    "{\"port\":%d, \"baud\":%d, \"databits\":%d, \"parity\":\"%s\", \"stopbits\":%s}%s",
                    weta->hal->sports.ports[i].port,
                    weta->hal->sports.ports[i].baud,
                    dataBits[weta->hal->sports.ports[i].data_bits],
                    parity[weta->hal->sports.ports[i].parity],
                    stopBits[weta->hal->sports.ports[i].stop_bits],
                    i == weta->hal->sports.n_ports - 1 ? "" : ","
            );
        }
        strcat(json, "]");
    }

    strcat(json, "}");
    return true;
}
#endif