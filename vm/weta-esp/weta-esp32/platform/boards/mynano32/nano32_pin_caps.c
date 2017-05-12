#include <hw_pins.h>


static WetaPinCaps pincaps[] =
    {
/*GPIO00*/  PINCAP_AVOID | PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO01*/  PINCAP_AVOID | PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_UART_TX,
/*GPIO02*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO03*/  PINCAP_AVOID | PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_UART_RX,
/*GPIO04*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO05*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_SS,
/*GPIO06*/  PINCAP_AVOID,
/*GPIO07*/  PINCAP_AVOID,
/*GPIO08*/  PINCAP_AVOID,
/*GPIO09*/  PINCAP_AVOID,
/*GPIO10*/  PINCAP_AVOID,
/*GPIO11*/  PINCAP_AVOID,
/*GPIO12*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_MISO | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO13*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_MOSI | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO14*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_SCLK | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO15*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_SS   | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO16*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_UART_RX,
/*GPIO17*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_UART_TX,
/*GPIO18*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_SCLK,
/*GPIO19*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_MISO | PINCAP_UART_CTS,
/*GPIO20*/  PINCAP_AVOID,
/*GPIO21*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_I2C_SDA,
/*GPIO22*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_I2C_SCL | PINCAP_UART_RTS,
/*GPIO23*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_SPI_MOSI,
/*GPIO24*/  PINCAP_AVOID,
/*GPIO25*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_DAC,
/*GPIO26*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_DAC,
/*GPIO27*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO28*/  PINCAP_AVOID,
/*GPIO29*/  PINCAP_AVOID,
/*GPIO30*/  PINCAP_AVOID,
/*GPIO31*/  PINCAP_AVOID,
/*GPIO32*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO33*/  PINCAP_DIGITAL_IN | PINCAP_DIGITAL_OUT | PINCAP_PWM | PINCAP_ADC | PINCAP_TOUCH,
/*GPIO34*/  PINCAP_DIGITAL_IN | PINCAP_ADC,
/*GPIO35*/  PINCAP_DIGITAL_IN | PINCAP_ADC,
/*GPIO36*/  PINCAP_DIGITAL_IN | PINCAP_ADC,
/*GPIO37*/  PINCAP_AVOID | PINCAP_DIGITAL_IN | PINCAP_ADC,
/*GPIO38*/  PINCAP_AVOID | PINCAP_DIGITAL_IN | PINCAP_ADC,
/*GPIO39*/  PINCAP_AVOID | PINCAP_DIGITAL_IN | PINCAP_ADC
    };

static UartPinGroup uart_pin_groups[] =
    {
        { .tx = 1, .rx = 3, .cts = 19, .rts = 22 },
        { .tx = 17, .rx = 16, .cts = 0xFF, .rts = 0xFF }
    };

static SpiPinGroup spi_pin_groups[] =
    {
        { .miso = 12, .mosi = 13, .sclk = 14, .ss = 15 },
        { .miso = 19, .mosi = 23, .sclk = 18, .ss = 5 }
    };

static I2cPinGroup i2c_pin_groups[] =
    {
        { .scl = 22, .sda = 21 }
    };

static uint8_t adc_pin_order[] =
    {
        36, 0xFF, 0xFF, 39, 32, 33, 34, 35, 0xFF, 0xFF, 4, 0, 2, 15, 13, 12, 14, 27, 25, 26
    };

static uint8_t touch_pin_order[] =
    {
        4, 0, 2, 15, 13, 12, 14, 27, 33, 32
    };

const WetaPins weta_pins =
    {
        .pins = pincaps,
        .n_pins = sizeof (pincaps) / sizeof (WetaPinCaps),
        .uart_groups = uart_pin_groups,
        .n_uart_groups = sizeof (uart_pin_groups) / sizeof (UartPinGroup),
        .spi_groups = spi_pin_groups,
        .n_spi_groups = sizeof (spi_pin_groups) / sizeof (SpiPinGroup),
        .i2c_groups = i2c_pin_groups,
        .n_i2c_groups = sizeof (i2c_pin_groups) / sizeof (I2cPinGroup),
        .adc_order = adc_pin_order,
        .n_adc = sizeof (adc_pin_order) / sizeof (uint8_t),
        .touch_order = touch_pin_order,
        .n_touch = sizeof (touch_pin_order) / sizeof (uint8_t)
    };
