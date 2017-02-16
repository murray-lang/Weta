#include <weta_platform.h>
#include <weta.h>
#include <os_type.h>
#include <osapi.h>


SerialPort serialPorts[] =
    {
        {
            .port      = UART0,
            .baud      = BIT_RATE_115200,
            .data_bits = EIGHT_BITS,
            .parity    = NONE_BITS,
            .stop_bits = ONE_STOP_BIT
         },
        {
            .port = UART1,
            .baud      = BIT_RATE_115200,
            .data_bits = EIGHT_BITS,
            .parity    = NONE_BITS,
            .stop_bits = ONE_STOP_BIT
        }
    };

DebounceState debounce[1] =
    {
        {false, false, false, true, 0}
    };

GpioPin gpio[] =
    {
        {
            .pin = RUN_PIN,
            .invert = false,
            .debounce = &debounce[0]
        },
        {
            .pin = MOTOR_0_DIRA_PIN,
            .invert = false,
            .debounce = NULL
        },
        {
            .pin = MOTOR_0_DIRB_PIN,
            .invert = false,
            .debounce = NULL

        },
        {
            .pin = MOTOR_1_DIRA_PIN,
            .invert = false,
            .debounce = NULL

        },
        {
            .pin = MOTOR_1_DIRB_PIN,
            .invert = false,
            .debounce = NULL

        }
    };

uint32_t pwm_gpio[NUM_PWM_CHANNELS][3] =
    {
        {GPIO_PIN_REG(MOTOR_0_PWM_PIN),MOTOR_0_PWM_FUNC, BIT(MOTOR_0_PWM_PIN)},
        {GPIO_PIN_REG(MOTOR_1_PWM_PIN),MOTOR_1_PWM_FUNC, BIT(MOTOR_1_PWM_PIN)}
    };

uint32_t initialDuty[NUM_PWM_CHANNELS] = { 0, 0 };
PwmChannel motorPwms[NUM_PWM_CHANNELS] = { 0, 1 };

Motor motors[] =
{
    {
        .pins  = { .a = &gpio[1], .b = &gpio[2], .pwm = &motorPwms[0] },
        .state = { 0, false, false, false, MOTOR_THIS_WAY, 100 },
    },
    {
        .pins  = { .a = &gpio[3], .b = &gpio[4], .pwm = &motorPwms[1] },
        .state = { 0, false, false, false, MOTOR_THIS_WAY, 100 }
    }
};


Hardware hardware =
{
	.flash = (uint8_t*)FLASH_USER_START,
	.flashLength = FLASH_USER_LENGTH,
	.sports =
        {
            .ports   = serialPorts,
            .n_ports = 0, //sizeof(serialPorts) / sizeof(SerialPort),
	    },
	.motors =
        {
            .motors = motors,
            .n_motors = sizeof(motors) / sizeof(Motor),
	    }
#ifdef	SUPPORT_STEPPERS
    ,.steppers =
        {
            .steppers   = NULL,
            .n_steppers = 0,
            .steps =
                {
                    { 0, 0 },
                    { 1, 0 },
                    { 1, 1 },
                    { 0, 1 }
                }
        },
#endif
#ifdef	SUPPORT_SERVOS
	,.servos =
        {
            .servos = NULL,
            .n_servos = 0
	    }
#endif
	    ,.gpio =
        {
            .pins = gpio,
            .n_pins = sizeof(gpio) / sizeof(GpioPin)
	    },
    .pwms =
        {
            .frequency    = 100,
            .initial_duty = initialDuty,
            .gpio_info    = pwm_gpio,
            .n_channels   = NUM_PWM_CHANNELS
        }
#ifdef	SUPPORT_ADC
    ,.adc =
        {
            .channels   = NULL,
            .n_channels = 0,
            .depth      = 0
        }
#endif
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
        strcat(json, value ? "1]" : "0]");
        strcat(json, "}");
    }
#ifdef SUPPORT_ADC
    if (q & QUERY_ANALOG)
    {
        if (json[strlen(json)-1] == '}');
            strcat(json, ",");
        int16_t value0 = 0;
        int16_t value1 = 0;
        hw_adc_get(&weta->hal->adc, 0, &value0);
        hw_adc_get(&weta->hal->adc, 1, &value1);
        weta_sprintf(json + strlen(json), "\"analog\":{\"input\":[%d,%d]}",
                value0, value1
        );
    }
#endif
    if (q & QUERY_SERIAL)
    {
        if (json[strlen(json)-1] == '}');
            strcat(json, ",");
        strcat(json, "\"serial\":[");
        int i;
        for (i = 0; i < weta->hal->sports.n_ports; i++)
        {
            weta_sprintf(json + strlen(json),
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
