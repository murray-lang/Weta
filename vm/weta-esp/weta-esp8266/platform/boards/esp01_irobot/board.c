#include <weta_platform.h>
#include <weta.h>
#include <os_type.h>
#include <osapi.h>


SerialPort serialPorts[] =
    {
        {
            .port      = UART0,
            .baud      = BIT_RATE_57600,
            .data_bits = EIGHT_BITS,
            .parity    = NONE_BITS,
            .stop_bits = ONE_STOP_BIT
         }
    };
#ifdef SUPPORT_GPIO
GpioPin gpio[] =
    {
        {
            .pin = RUN_PIN,
            .invert = false
        },
        {
            .pin = USER_LED_PIN,
            .invert = false
        },
    };
#endif

Hardware hardware =
{
	.code = (uint8_t*)FLASH_USER_START,
	.codeLength = FLASH_USER_LENGTH,
	.sports =
        {
            .ports   = serialPorts,
            .n_ports = sizeof(serialPorts) / sizeof(SerialPort),
	    }
#ifdef SUPPORT_MOTORS
	,.motors =
        {
            .motors = motors,
            .n_motors = sizeof(motors) / sizeof(Motor),
	    }
#endif
#ifdef SUPPORT_GPIO
	,.gpio =
        {
            .pins = gpio,
            .n_pins = sizeof(gpio) / sizeof(GpioPin)
	    }
#endif
#ifdef SUPPORT_PWM
    ,.pwms =
        {
            .frequency    = 100,
            .initial_duty = initialDuty,
            .gpio_info    = pwm_gpio,
            .n_channels   = NUM_PWM_CHANNELS
        }
#endif
#ifdef SUPPORT_ADC
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
#ifdef SUPPORT_GPIO
    if (q & QUERY_DIGITAL)
    {
        strcat(json, "\"digital\":{\"input\":[");
        bool value = false;
        hw_gpio_get_i(&weta->hal->gpio, GPIO_INDEX_RUN, &value);
        strcat(json, value ? "1]" : "0]");
        strcat(json, "}");
    }
#endif
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

