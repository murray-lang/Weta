//
// Created by murray on 11/03/17.
//

#ifndef __HW_PINS_H__
#define __HW_PINS_H__
#include <weta_platform.h>

/**
 * @brief CPU pin capabilities. Hopefully 32 bits is enough
 */

typedef uint32_t WetaPinCaps;

typedef enum
{
    PINCAP_NONE             = 0x00000000,
    PINCAP_DIGITAL_IN       = 0x00000001,
    PINCAP_DIGITAL_OUT      = 0x00000002,
    PINCAP_ADC              = 0x00000004,
    PINCAP_DAC              = 0x00000008,
    PINCAP_PWM              = 0x00000010,
    PINCAP_COUNTER          = 0x00000020,
    PINCAP_TOUCH            = 0x00000040,
    PINCAP_I2C_SCL          = 0x00000080,
    PINCAP_I2C_SDA          = 0x00000100,
    PINCAP_SPI_MISO         = 0x00000200,
    PINCAP_SPI_MOSI         = 0x00000400,
    PINCAP_SPI_SCLK         = 0x00000800,
    PINCAP_SPI_SS           = 0x00001000,
    PINCAP_UART_RX          = 0x00002000,
    PINCAP_UART_TX          = 0x00004000,
    PINCAP_UART_RTS         = 0x00008000,
    PINCAP_UART_CTS         = 0x00010000,
    PINCAP_AVOID            = 0x80000000
} eWetaPinCaps;

typedef struct
{
    uint8_t tx;
    uint8_t rx;
    uint8_t cts;
    uint8_t rts;
}UartPinGroup;

typedef struct
{
    uint8_t scl;
    uint8_t sda;
}I2cPinGroup;

typedef struct
{
    uint8_t miso;
    uint8_t mosi;
    uint8_t sclk;
    uint8_t ss;
}SpiPinGroup;


typedef struct
{
    WetaPinCaps*    pins;
    uint8_t         n_pins;
    uint8_t*        adc_order;
    uint8_t         n_adc;
    uint8_t*        dac_order;
    uint8_t         n_dac;
    uint8_t*        touch_order;
    uint8_t         n_touch;
    UartPinGroup*   uart_groups;
    uint8_t         n_uart_groups;
    SpiPinGroup*    spi_groups;
    uint8_t         n_spi_groups;
    I2cPinGroup*    i2c_groups;
    uint8_t         n_i2c_groups;
} WetaPins;

extern const WetaPins weta_pins;

#ifdef SUPPORT_JSON
extern bool weta_pins_to_json(char * buf, size_t buflen);
#endif


#endif //__HW_PINS_H__
