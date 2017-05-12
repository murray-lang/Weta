//
// Created by murray on 13/01/17.
//
#include <weta_platform.h>
#include <weta.h>
#include "mynano32_board.h"
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/uart.h>
#include <driver/adc.h>

#ifdef SUPPORT_QUERY

#include <stdio.h>  // for sprintf()
#include <string.h>
#include <hw_defs.h>

#endif

Hardware hardware =
    {
        .flash             = (uint8_t *) FLASH_USER_START,
        .flashLength       = FLASH_USER_LENGTH,
        .configFlash       = (uint8_t *) FLASH_CONFIG_START,
        .configFlashLength = FLASH_CONFIG_LENGTH,
        .sports =
            {
                {
                    .port      = 0,
                    .baud      = 115200,
                    .params.databits = SERIAL_DATABITS_8,
                    .params.parity   = SERIAL_PARITY_NONE,
                    .params.stopbits = SERIAL_STOPBITS_1,
                    .params.flow     = SERIAL_FLOW_RTS
                }//,
                /*
                {
                    .port      = 1,
                    .baud      = 9600,
                    .data_bits = (uint8_t) UART_DATA_8_BITS,
                    .parity    = (uint8_t) UART_PARITY_DISABLE,
                    .stop_bits = (uint8_t) UART_STOP_BITS_1
                }
                 */
            },
        .ledGpio = 0xFF
        //, .gpio = {{0}}
        /*
        {
            {.pin = PIN_USER_LED, .options = GPIO_OPT_OUT | GPIO_OPT_INVERT, .debounce = {0}},
            {.pin = PIN_RUN, .options = GPIO_OPT_PULLUP | GPIO_OPT_DEBOUNCE,
                .debounce = {.state = false, .candidate = false, .debounced = false, .latched = true, .lastCount = 0}},
            {.pin = PIN_SWITCH1, .options = GPIO_OPT_PULLUP, .debounce = {0}},
            {.pin = PIN_SWITCH2, .options = GPIO_OPT_PULLUP, .debounce = {0}},
            {.pin = PIN_SWITCH3, .options = GPIO_OPT_PULLUP, .debounce = {0}},
            {.pin = PIN_SWITCH4, .options = GPIO_OPT_PULLUP, .debounce = {0}},
            {.pin = PIN_MOTOR_0_DIR, .options = GPIO_OPT_OUT, .debounce = {0}},
            {.pin = PIN_MOTOR_1_DIR, .options = GPIO_OPT_OUT, .debounce = {0}},
            {.pin = PIN_STEPPER_CLOCK, .options = GPIO_OPT_OUT, .debounce = {0}},
            {.pin = PIN_STEPPER_DATA, .options = GPIO_OPT_OUT | GPIO_OPT_INVERT, .debounce = {0}},
            {.pin = PIN_STEPPER_STROBE, .options = GPIO_OPT_OUT, .debounce = {0}}
        }
         */
        //, .shifters = {{0}}
        /*
        {
            {
                .clock   = GPIO_INDEX_STEPPER_CLOCK,
                .data    = GPIO_INDEX_STEPPER_DATA,
                .strobe  = GPIO_INDEX_STEPPER_STROBE,
                .shifted = 0
            }
        }
         */
        //, .motors = {{0}}
        /*
        {
            {
                .pins = {.dir = GPIO_INDEX_MOTOR_0_DIR, .pwm = 0},
                .state ={.selected = false, .on = false, .brake = false, .dir = MOTOR_THIS_WAY, .power = 100}
            },
            {
                .pins = {.dir = GPIO_INDEX_MOTOR_1_DIR, .pwm = 1},
                .state = {.selected = false, .on = false, .brake = false, .dir = MOTOR_THIS_WAY, .power = 100}
            }
        }
         */
        //,.steppers = {{0}}
        /*
            {
                {.shifter = 0, .offset = 0, .unused1 = 0xFF, .unused2 = 0xFF,
                    .reverse = false, .index = 0, .cmd = STEP_CMD_OFF, .arg = 0},
                {.shifter = 0, .offset = 4, .unused1 = 0xFF, .unused2 = 0xFF,
                    .reverse = true, .index = 0, .cmd = STEP_CMD_OFF, .arg = 0}
            },
            */
        //,.servos = {{0}}
        /*
        {
            {
                .pwm = 2, .selected = false, .position = 0,
                .config = {.dutyMin = 110, .duty0 = 300, .dutyMax = 490, .dutyPer10Degrees = -20}
            },
            {
                .pwm = 3, .selected = false, .position = 0,
                .config = {.dutyMin = 110, .duty0 = 300, .dutyMax = 490, .dutyPer10Degrees = -20}
            }
        }
         */
        //, .pwms = {{0}}
        /*
        {
            {.channel = LEDC_CHANNEL_0, .pin = PIN_MOTOR_0_PWM},
            {.channel = LEDC_CHANNEL_1, .pin = PIN_MOTOR_1_PWM},
            {.channel = LEDC_CHANNEL_3, .pin = PIN_SERVO_0_PWM},
            {.channel = LEDC_CHANNEL_4, .pin = PIN_SERVO_1_PWM},
            {.channel = LEDC_CHANNEL_5, .pin = PIN_BUZZER}
        }
         */
        //,.adc =
        /*
            {
                .channels = {{0}},
                .depth = (WetaAdcDepth) ADC_WIDTH_10Bit
            }
            */
    };

void WETAFUNCATTR
init_board(void)
{
        // Debugging
    //gpio_pad_select_gpio(PIN_STEPPER_CLOCK);
    //gpio_pad_select_gpio(PIN_STEPPER_DATA);
        // I suspect that having a shift register connected to this pin might
        // be causing problems with the ESP ROM software because it is a UART
        // pin. Making it a GPIO explicitly.
    gpio_pad_select_gpio(17);

    hardware.ledGpio = 0xFF;
    hardware.beeperPwm = 0xFF;
    int i;
        // mark the unused UARTS
    //for (i = 0; i < MAX_UARTS; i++) {
    //    hardware.sports[i].port = 0xFF;
    //}
        // mark the unused GPIOs
    for (i = 0; i < MAX_GPIO; i++) {
        hardware.gpio[i].pin = 0xFF;
    }
        // mark the unused shifters
    for (i = 0; i < MAX_SHIFTERS; i++) {
        hardware.shifters[i].data = 0xFF;
        hardware.shifters[i].clock = 0xFF;
        hardware.shifters[i].strobe = 0xFF;
        hardware.shifters[i].shifted = 0;
    }
        // Mark the unused PWM timers
    for (i = 0; i < MAX_PWM_TIMERS; i++) {
        hardware.pwms.timers[i].width = 0;
        hardware.pwms.timers[i].frequency = 0;
    }
        // Mark the unused PWMs
    for (i = 0; i < MAX_PWM; i++) {
        hardware.pwms.channels[i].channel = 0xFF;
        hardware.pwms.channels[i].pin = 0xFF;
    }
    // Mark the unused Servos
    for (i = 0; i < MAX_SERVOS; i++) {
        hardware.servos[i].selected = false;
        hardware.servos[i].pwm = 0xFF;
        hardware.servos[i].position = 0;
        hardware.servos[i].config.dutyMin = 0;
        hardware.servos[i].config.dutyMax = 0;
        hardware.servos[i].config.duty0 = 0;
        hardware.servos[i].config.dutyPer10Degrees = 0;
    }
    // mark the unused motors
    for (i = 0; i < MAX_MOTORS; i++) {
        hardware.motors[i].state.selected = false;
        hardware.motors[i].pins.a = 0xFF;
        hardware.motors[i].pins.b = 0xFF;
        hardware.motors[i].pins.pwm = 0xFF;
    }
    // Mark the unused steppers
    for (i = 0; i < MAX_STEPPERS; i++) {
        hardware.steppers[i].shifter = 0xFF;
        hardware.steppers[i].arg = 0;
        hardware.steppers[i].cmd = STEP_CMD_OFF;
        int j;
        for (j = 0; j < 4; j++) {
            hardware.steppers[i].pins[j] = 0xFF;
        }

    }
        // Mark the unused ADC
    hardware.adc.depth = 0;
    for (i = 0; i < MAX_ADC; i++) {
        hardware.adc.channels[i].channel = 0xFF;
        hardware.adc.channels[i].gain   = 0;
    }
}

#ifdef SUPPORT_QUERY

bool WETAFUNCATTR
weta_query_config(Weta *weta, char *json, uint16_t length)
{
    /*
    uint8_t dataBits[] = {5, 6, 7, 8};
    char *stopBits[] = {"0", "1", "2", "1.5"};
    char *parity[] = {"none", "even", "odd"};
    */
    strcat(json, "{\"profile\": \"Nano32\",\"digital\":[");

    GpioPin* gpio = weta->hal->gpio;
    bool comma = false;
    int i;
    for (i = 0; i < MAX_GPIO; i++) {
        if (gpio[i].pin != 0xFF) {
            if (comma)
                weta_strcat(json, ",");
            comma = true;
            weta_sprintf(json + weta_strlen(json),
                "{\"id\":%d,\"io\":%d,\"in\":",
                i, gpio[i].pin);
            weta_strcat(json, (gpio[i].options & GPIO_OPT_INPUT) ? "1" : "0");
            weta_strcat(json, ",\"out\":");
            weta_strcat(json, (gpio[i].options & GPIO_OPT_OUTPUT) ? "1" : "0");
                // Can't be open drain as well as pullup/pulldown
            if (gpio[i].options & GPIO_OPT_OPEN_DRAIN) {
                weta_strcat(json, ",\"opendrain\":1");
            } else {
                if (gpio[i].options & GPIO_OPT_PULLUP)
                    weta_strcat(json, ",\"pullup\":1");
                if (gpio[i].options & GPIO_OPT_PULLDOWN)
                    weta_strcat(json, ",\"pulldown\":1");
            }
            if (gpio[i].options & GPIO_OPT_INVERT)
                weta_strcat(json, ",\"invert\":1");
            if (gpio[i].options & GPIO_OPT_INPUT && gpio[i].options & GPIO_OPT_DEBOUNCE)
                weta_strcat(json, ",\"debounce\":1");
            weta_strcat(json, "}");
        }
    }
    weta_strcat(json, "],\"pwm\":{\"timers\":[");
    comma = false;
    PwmTimer* timers = weta->hal->pwms.timers;
    for (i = 0; i < MAX_PWM_TIMERS; i++) {
        if (timers[i].width != 0 && timers[i].frequency != 0) {
            if (comma)
                weta_strcat(json, ",");
            comma = true;
            weta_sprintf(
                json + weta_strlen(json),
                "{\"id\":%d,\"frequency\":%d, \"width\":%d}",
                i, timers[i].frequency, timers[i].width
            );
        }
    }
    weta_strcat(json, "],\"channels\":[");
    comma = false;
    PwmChannel *channels = weta->hal->pwms.channels;
    for (i = 0; i < MAX_PWM; i++) {
        if (channels[i].pin != 0xFF) {
            if (comma)
                weta_strcat(json, ",");
            comma = true;
            weta_sprintf(
                json + weta_strlen(json),
                "{\"id\":%d,\"io\":%d,\"timer\":%d}",
                channels[i].channel, channels[i].pin, channels[i].timer
            );
        }
    }
    weta_strcat(json, "]}");
    if (weta->hal->adc.depth) {
        weta_sprintf(
            json + weta_strlen(json),
            ",\"adc\":{\"depth\":[{\"depth\":%d}],\"channels\":[",
            weta->hal->adc.depth
        );
        comma = false;
        AdcChannel *adcChannels = weta->hal->adc.channels;
        for (i = 0; i < MAX_ADC; i++) {
            if (adcChannels[i].channel != 0xFF) {
                if (comma)
                    weta_strcat(json, ",");
                comma = true;
                weta_sprintf(
                    json + weta_strlen(json),
                    "{\"id\":%d,\"io\":%d,\"gain\":%d}",
                    adcChannels[i].channel, weta_pins.adc_order[adcChannels[i].channel], adcChannels[i].gain
                );
            }

        }
        weta_strcat(json, "]}");
    }
    weta_strcat(json, ",\"motor\": [");
    comma = false;
    Motor *motors = weta->hal->motors;
    for (i = 0; i < MAX_MOTORS; i++) {
        if (motors[i].pins.pwm == 0xFF || motors[i].pins.a == 0xFF) {
            continue; // Not a valid configuration
        }
        if (comma)
            weta_strcat(json, ",");
        comma = true;
        weta_sprintf(
            json + weta_strlen(json),
            "{\"id\":%d,\"pwm\":%d,",
            i, motors[i].pins.pwm
        );
        if (motors[i].pins.b == 0xFF) {
            weta_sprintf(
                json + weta_strlen(json),
                "\"dir\":%d}",
                motors[i].pins.dir
            );
        } else {
            weta_sprintf(
                json + weta_strlen(json),
                "\"a\":%d,\"b\":%d}",
                motors[i].pins.a, motors[i].pins.b
            );
        }
    }
    weta_strcat(json, "],\"stepper\":[");
    comma = false;
    StepperMotor *steppers = weta->hal->steppers;
    for (i = 0; i < MAX_STEPPERS; i++) {
        if (steppers[i].pins[0] == 0xFF) {
                // First value is either A or a shifter ID. Must have one or
                // the other.
            continue;
        }
            // Now look at the second value. If missing then the first value
            // above is a shifter ID. If present then the first value is A and
            // the second is B
        if (steppers[i].pins[1] == 0xFF) {
                // The first value is a shifter ID. That means that the third
                // value is an offset and must be present.
            if (steppers[i].pins[2] == 0xFF) {
                continue;
            }
                //We have a valid Stepper-with-shifter config. JSON it!
            if (comma)
                weta_strcat(json, ",");
            weta_sprintf(
                json + weta_strlen(json),
                "{\"id\":%d,\"reverse\":%d,\"shifter\":%d,\"offset\":%d}",
                i,
                steppers[i].reverse ? 1 : 0,
                steppers[i].shifter,
                steppers[i].offset
            );
        } else {
                // We had A and B at least. See if we also have C and D
                // If one is present then they must both be.
            if (   (steppers[i].pins[2] == 0xFF && steppers[i].pins[3] != 0xFF)
                || (steppers[i].pins[2] != 0xFF && steppers[i].pins[3] == 0xFF)) {
                continue; // Invalid config. Go no further with this one.
            }
                // OK, we have A and B for certain. Encode them.
            if (comma)
                weta_strcat(json, ",");
            weta_sprintf(
                json + weta_strlen(json),
                "{\"id\":%d,\"reverse\":%d,\"a\":%d,\"b\":%d",
                i,
                steppers[i].reverse ? 1 : 0,
                steppers[i].pins[0],
                steppers[i].pins[1]
            );
                // if we have C then we also have D (because of the test above)
            if (steppers[i].pins[2] != 0xFF) {
                weta_sprintf(
                    json + weta_strlen(json),
                    ",\"c\":%d,\"d\":%d",
                    steppers[i].pins[0], steppers[i].pins[1]
                );
            }
            weta_strcat(json, "}");
        }
        comma = true;
    }
    weta_strcat(json, "],\"servo\": [");
    comma = false;
    ServoState *servos = weta->hal->servos;
    for (i = 0; i < MAX_SERVOS; i++) {
        if (servos[i].pwm == 0xFF) {
            continue; // Not a valid configuration
        }
        if (comma)
            weta_strcat(json, ",");
        comma = true;
        weta_sprintf(
            json + weta_strlen(json),
            "{\"id\":%d,\"pwm\":%d,\"minduty\":%d,\"maxduty\":%d,\"duty0\":%d,\"dutyper10\":%d}",
            i,
            servos[i].pwm,
            servos[i].config.dutyMin,
            servos[i].config.dutyMax,
            servos[i].config.duty0,
            servos[i].config.dutyPer10Degrees
        );
    }
   weta_strcat(json, "],\"shifter\": [");
    comma = false;
    Shifter *shifters = weta->hal->shifters;
    for (i = 0; i < MAX_SHIFTERS; i++) {
        if (shifters[i].clock == 0xFF ||
            shifters[i].data == 0xFF ||
            shifters[i].strobe == 0xFF) {
            continue; // Not a valid configuration
        }
        if (comma)
            weta_strcat(json, ",");
        comma = true;
        weta_sprintf(
            json + weta_strlen(json),
            "{\"id\":%d,\"width\":%d,\"clock\":%d,\"data\":%d,\"strobe\":%d}",
            i,
            shifters[i].width,
            shifters[i].clock,
            shifters[i].data,
            shifters[i].strobe
        );
    }
    weta_strcat(json, "]}");
    return true;
}

bool WETAFUNCATTR
weta_query_status(Weta *weta, WetaQuery q, char *json, uint16_t length)
{
    uint8_t dataBits[] = {5, 6, 7, 8};
    char *stopBits[] = {"0", "1", "2", "1.5"};
    char *parity[] = {"none", "even", "odd"};
    // TODO: Deal with length parameter
    weta_strcat(json, "{");
    int i;
    bool comma = false;
    if (q & QUERY_DIGITAL) {
        weta_strcat(json, "\"digital\":{\"input\":[");
        GpioPin* pins = weta->hal->gpio;
        bool value = false;

        for (i = 0; i < MAX_GPIO; i++) {

            if (pins[i].pin != 0xFF) {
                if (pins[i].options & GPIO_OPT_INPUT) {
                    if (comma)
                        weta_strcat(json, ",");
                    comma = true;
                    hw_gpio_get_raw(&pins[i], &value);
                    weta_strcat(json, value ? "1" : "0");
                }
            }
        }
        weta_strcat(json, "],\"output\":[");
        comma = false;
        for (i = 0; i < MAX_GPIO; i++) {
            if (pins[i].pin != 0xFF) {
                if (pins[i].options & GPIO_OPT_OUTPUT) {
                    if (comma)
                        weta_strcat(json, ",");
                    comma = true;
                    hw_gpio_get_raw(&pins[i], &value);
                    weta_strcat(json, value ? "1" : "0");
                }
            }
        }
        weta_strcat(json, "]}");
    }
    if (q & QUERY_ANALOG) {
        if (q & QUERY_DIGITAL) {
            weta_strcat(json, ",");
        }
        int16_t value0 = 0;
        int16_t value1 = 0;
        hw_adc_get(weta->hal, 0, &value0);
        hw_adc_get(weta->hal, 1, &value1);
        weta_sprintf(json + weta_strlen(json), "\"analog\":{\"input\":[%d,%d]}", value0, value1
        );
    }
    if (q & QUERY_SERIAL) {
        if (q & (QUERY_DIGITAL | QUERY_ANALOG)) {
            weta_strcat(json, ",");
        }
        weta_strcat(json, "\"serial\":[");
        int i;
        for (i = 0; i < MAX_UARTS; i++) {
            if (weta->hal->sports[i].port != 0xFF) {
                sprintf(json + weta_strlen(json)
                        , "{\"port\":%d, \"baud\":%d, \"databits\":%d, \"parity\":\"%s\", \"stopbits\":%s}%s"
                        , weta->hal->sports[i].port, weta->hal->sports[i].baud
                        , dataBits[weta->hal->sports[i].params.databits], parity[weta->hal->sports[i].params.parity]
                        , stopBits[weta->hal->sports[i].params.stopbits], i == MAX_UARTS - 1 ? "" : ","
                );
            }
        }
        weta_strcat(json, "]");
    }

    weta_strcat(json, "}");
    return true;
}

#endif
