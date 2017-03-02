#ifndef __HW_DEFS_H__
#define __HW_DEFS_H__

#include <ets_sys.h>
#include <uart_register.h>
#include <uart.h>
#include <gpio.h>
#include <pwm.h>

typedef uint8_t  WetaPin;
typedef uint32_t WetaGpioMode;
typedef uint8_t  WetaPwmChannel;
typedef uint8_t  WetaPwmDuty;
typedef uint8_t  WetaAdcChannel;
typedef uint8_t  WetaAdcDepth;
typedef uint8_t  WetaAdcAtten;

typedef struct
{
    bool        state;
    bool        candidate;
    bool        debounced;
    bool        latched;
    uint32_t    lastTime;
} DebounceState;

typedef struct
{
    uint8_t         pin;
    bool            invert;
    DebounceState*  debounce;
} GpioPin;

typedef struct
{
    GpioPin* pins;
    uint8_t  n_pins;
    GpioPin** debounced_pins;
    uint8_t  n_debounced_pins;
} Gpio;

typedef struct
{
    WetaAdcChannel  channel;
    WetaAdcAtten    atten;
} AdcChannel;

typedef struct
{
    AdcChannel*     channels;
    uint8_t         n_channels;
    WetaAdcDepth    depth;
} Adc;

typedef uint8_t PwmChannel;

typedef struct
{
    uint32_t        period;
    uint32_t*       initial_duty;
    uint32(*gpio_info)[3];
    uint8_t			n_channels;
} PwmChannels;


typedef struct
{
    uint8_t             port;
    UartBautRate 	    baud;
    UartBitsNum4Char    data_bits;
    UartParityMode 	    parity;
    UartStopBitsNum     stop_bits;
} SerialPort;

typedef struct
{
    SerialPort* ports;
    uint8_t     n_ports;
    uint8_t     system_print_port;
} SerialPorts;

#define GPIO_PIN_REG_0          PERIPHS_IO_MUX_GPIO0_U
#define GPIO_PIN_REG_1          PERIPHS_IO_MUX_U0TXD_U
#define GPIO_PIN_REG_2          PERIPHS_IO_MUX_GPIO2_U
#define GPIO_PIN_REG_3          PERIPHS_IO_MUX_U0RXD_U
#define GPIO_PIN_REG_4          PERIPHS_IO_MUX_GPIO4_U
#define GPIO_PIN_REG_5          PERIPHS_IO_MUX_GPIO5_U
#define GPIO_PIN_REG_6          PERIPHS_IO_MUX_SD_CLK_U
#define GPIO_PIN_REG_7          PERIPHS_IO_MUX_SD_DATA0_U
#define GPIO_PIN_REG_8          PERIPHS_IO_MUX_SD_DATA1_U
#define GPIO_PIN_REG_9          PERIPHS_IO_MUX_SD_DATA2_U
#define GPIO_PIN_REG_10         PERIPHS_IO_MUX_SD_DATA3_U
#define GPIO_PIN_REG_11         PERIPHS_IO_MUX_SD_CMD_U
#define GPIO_PIN_REG_12         PERIPHS_IO_MUX_MTDI_U
#define GPIO_PIN_REG_13         PERIPHS_IO_MUX_MTCK_U
#define GPIO_PIN_REG_14         PERIPHS_IO_MUX_MTMS_U
#define GPIO_PIN_REG_15         PERIPHS_IO_MUX_MTDO_U

#define GPIO_PIN_REG(i) \
    (i==0) ? GPIO_PIN_REG_0:  \
    (i==1) ? GPIO_PIN_REG_1:  \
    (i==2) ? GPIO_PIN_REG_2:  \
    (i==3) ? GPIO_PIN_REG_3:  \
    (i==4) ? GPIO_PIN_REG_4:  \
    (i==5) ? GPIO_PIN_REG_5:  \
    (i==6) ? GPIO_PIN_REG_6:  \
    (i==7) ? GPIO_PIN_REG_7:  \
    (i==8) ? GPIO_PIN_REG_8:  \
    (i==9) ? GPIO_PIN_REG_9:  \
    (i==10)? GPIO_PIN_REG_10: \
    (i==11)? GPIO_PIN_REG_11: \
    (i==12)? GPIO_PIN_REG_12: \
    (i==13)? GPIO_PIN_REG_13: \
    (i==14)? GPIO_PIN_REG_14: \
    GPIO_PIN_REG_15

#define GET_GPIO_FUNC(i) \
    (i==0) ? FUNC_GPIO0:  \
    (i==1) ? FUNC_GPIO1:  \
    (i==2) ? FUNC_GPIO2:  \
    (i==3) ? FUNC_GPIO3:  \
    (i==4) ? FUNC_GPIO4:  \
    (i==5) ? FUNC_GPIO5:  \
    (i==9) ? FUNC_GPIO9:  \
    (i==10)? FUNC_GPIO10: \
    (i==12)? FUNC_GPIO12: \
    (i==13)? FUNC_GPIO13: \
    (i==14)? FUNC_GPIO14: \
    FUNC_GPIO15

#endif  //__HW_DEFS_H__