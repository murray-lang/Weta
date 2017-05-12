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
        { .state = false, .candidate = false, .debounced = false, .latched = true, .lastTime = 0}
    };

GpioPin gpio[] =
    {
        {
            .pin = RUN_PIN,
            .invert = true,
            .debounce = &debounce[0]
        },
        {
            .pin = USER_LED_PIN,
            .invert = true,
            .debounce = NULL
        },
        {
            .pin = MOTOR_0_DIR_PIN,
            .invert = false,
            .debounce = NULL
        },
        {
            .pin = MOTOR_1_DIR_PIN,
            .invert = false,
            .debounce = NULL

        },

    };

GpioPin* debounced_gpios[] = { &gpio[GPIO_INDEX_RUN] };

uint32_t pwm_gpio[NUM_PWM_CHANNELS][3] =
    {
        {MOTOR_0_PWM_MUX,MOTOR_0_PWM_FUNC, MOTOR_0_PWM_PIN},
        {MOTOR_1_PWM_MUX,MOTOR_1_PWM_FUNC, MOTOR_1_PWM_PIN}
    };
uint32_t initialDuty[NUM_PWM_CHANNELS] = { 0, 0 };

PwmChannel motorPwms[NUM_PWM_CHANNELS] = { 0, 1 };

Motor motors[] =
{
    {
        .pins  = { .dir = &gpio[2], .pwm = &motorPwms[0] },
        .state = { 0, false, false, false, MOTOR_THAT_WAY, 0 },
    },
    {
        .pins  = { .dir = &gpio[3], .pwm = &motorPwms[1] },
        .state = { 0, false, false, false, MOTOR_THAT_WAY, 0 }
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
            .n_pins = sizeof(gpio) / sizeof(GpioPin),
            .debounced_pins = debounced_gpios,
            .n_debounced_pins = sizeof (debounced_gpios)/sizeof (GpioPin*)
	    },
    .pwms =
        {
            .period       = PWM_PERIOD,
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
    weta_strcat(json, "{");

    if (q & QUERY_DIGITAL)
    {
        weta_strcat(json, "\"digital\":{\"input\":[");
        bool value = false;
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_RUN, &value);
        weta_strcat(json, value ? "1]" : "0]");
        weta_strcat(json, "}");
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
        weta_strcat(json, ",");
        weta_strcat(json, "\"serial\":[");
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
        weta_strcat(json, "]");
    }

    weta_strcat(json, "}");
    return true;
}
#endif

