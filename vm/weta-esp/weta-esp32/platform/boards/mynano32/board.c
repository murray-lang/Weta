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
        {
            .port      = 0,
            .baud      = 115200,
            .data_bits = (uint8_t)UART_DATA_8_BITS,
            .parity    = (uint8_t)UART_PARITY_DISABLE,
            .stop_bits = (uint8_t)UART_STOP_BITS_1
        },
        {
            .port      = 1,
            .baud      = 9600,
            .data_bits = (uint8_t)UART_DATA_8_BITS,
            .parity    = (uint8_t)UART_PARITY_DISABLE,
            .stop_bits = (uint8_t)UART_STOP_BITS_1
        }
    };

DebounceState debounce[1] =
    {
        {.state = false, .candidate = false, .debounced = false, .latched = true, .lastTime = 0}
    };

GpioPin gpio[] =
    {
        { .pin = PIN_USER_LED,     .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_RUN,          .dir = GPIO_MODE_INPUT,  .pull = GPIO_PULLUP_ONLY, .debounce = &debounce[0] },
        { .pin = PIN_SWITCH1,      .dir = GPIO_MODE_INPUT,  .pull = GPIO_PULLUP_ONLY, .debounce = NULL },
        { .pin = PIN_SWITCH2,      .dir = GPIO_MODE_INPUT,  .pull = GPIO_PULLUP_ONLY, .debounce = NULL },
        { .pin = PIN_SWITCH3,      .dir = GPIO_MODE_INPUT,  .pull = GPIO_PULLUP_ONLY, .debounce = NULL },
        { .pin = PIN_SWITCH4,      .dir = GPIO_MODE_INPUT,  .pull = GPIO_PULLUP_ONLY, .debounce = NULL },
        { .pin = PIN_MOTOR_0_DIR,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_MOTOR_1_DIR,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_0_A,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_0_B,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_0_C,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_0_D,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_1_A,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_1_B,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_1_C,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
        { .pin = PIN_STEPPER_1_D,  .dir = GPIO_MODE_OUTPUT, .pull = GPIO_FLOATING,    .debounce = NULL },
    };

GpioPin* debounced_gpios[] = { &gpio[GPIO_INDEX_RUN] };

PwmChannel pwm[] =
    {
        { .channel = LEDC_CHANNEL_1, .pin = PIN_MOTOR_0_PWM },
        { .channel = LEDC_CHANNEL_2, .pin = PIN_MOTOR_1_PWM },
        { .channel = LEDC_CHANNEL_3, .pin = PIN_SERVO_0_PWM },
        { .channel = LEDC_CHANNEL_4, .pin = PIN_SERVO_1_PWM }
    };

Motor motors[] =
    {
        {
            { .dir = &gpio[GPIO_INDEX_MOTOR_0_DIR],  .pwm = &pwm[0] },
            { .id = 0, .selected = false, .on = false, .brake = false, .dir = MOTOR_THIS_WAY, .power = 100 }
        },
        {
            { .dir = &gpio[GPIO_INDEX_MOTOR_1_DIR],  .pwm = &pwm[1] },
            { .id = 0, .selected = false, .on = false, .brake = false, .dir = MOTOR_THIS_WAY, .power = 100 }
        }
    };

StepperMotor steppers[] =
    {
        {
            .pins =
                {
                &gpio[GPIO_INDEX_STEPPER_0_A],
                &gpio[GPIO_INDEX_STEPPER_0_B],
                &gpio[GPIO_INDEX_STEPPER_0_C],
                &gpio[GPIO_INDEX_STEPPER_0_D]
                },
            .index = 0, .cmd = STEP_CMD_OFF, .arg = 0
        },
        {
            .pins =
                {
                &gpio[GPIO_INDEX_STEPPER_1_A],
                &gpio[GPIO_INDEX_STEPPER_1_B],
                &gpio[GPIO_INDEX_STEPPER_1_C],
                &gpio[GPIO_INDEX_STEPPER_1_D]
                },
            .index = 0, .cmd = STEP_CMD_OFF, .arg = 0
        }
    };
#ifdef SUPPORT_SERVOS
ServoState servos[] =
    {
        {
            .pwm = &pwm[2], .id = 0, .selected = false, .position = 0,
            .config = { .dutyMin = 110, .duty0 = 300, .dutyMax = 490, .dutyPer10Degrees = -20 }
        },
        {
            .pwm = &pwm[3], .id = 1, .selected = false, .position = 0,
            .config = { .dutyMin = 110, .duty0 = 300, .dutyMax = 490, .dutyPer10Degrees = -20 }
        }
    };
#endif
AdcChannel adc_channels[] =
    {
        { .channel = (WetaAdcChannel)ADC1_CHANNEL_0, .atten = (WetaAdcAtten)ADC_ATTEN_0db },
        { .channel = (WetaAdcChannel)ADC1_CHANNEL_3, .atten = (WetaAdcAtten)ADC_ATTEN_6db }
    };

Hardware hardware =
    {
        .flash = (uint8_t*)FLASH_USER_START,
        .flashLength = FLASH_USER_LENGTH,
        .sports =
            {
            .ports = serialPorts,
            .n_ports = sizeof(serialPorts) / sizeof(SerialPort)
            },
        .motors =
            {
                .motors = motors,
                .n_motors = sizeof(motors) / sizeof(Motor)
            },
        .steppers =
            {
                .steppers = steppers,
                .n_steppers = sizeof(steppers) / sizeof(StepperMotor),
                .steps =
                    {
                        { 1, 0, 0, 1 },
                        { 1, 1, 0, 0 },
                        { 0, 1, 1, 0 },
                        { 0, 0, 1, 1 }
                    }
            }
#ifdef SUPPORT_SERVOS
        ,.servos =
            {
                .servos = servos,
                .n_servos = sizeof(servos) / sizeof(ServoState)
            }
#endif
        ,.gpio =
            {
                .pins = gpio,
                .n_pins = sizeof(gpio) / sizeof(GpioPin),
                .debounced_pins = debounced_gpios,
                .n_debounced_pins = sizeof(debounced_gpios) / sizeof(GpioPin*)
            },
        .pwms =
            {
                .channels = pwm,
                .n_channels = sizeof(pwm) / sizeof(PwmChannel)
            },
        .adc =
            {
                .channels = adc_channels,
                .n_channels = sizeof(adc_channels) / sizeof(AdcChannel),
                .depth = (WetaAdcDepth)ADC_WIDTH_10Bit
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
