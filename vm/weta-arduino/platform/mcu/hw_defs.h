#ifndef __HW_STRUCTS_H__
#define __HW_STRUCTS_H__

/**
 * @brief Depending on the platform, a pin might have to be identified with a port
 */
typedef uint8_t  WetaPin;
typedef uint32_t WetaGpioMode;
typedef uint8_t  WetaPwmChannel;
typedef uint16_t WetaPwmDuty;
typedef uint8_t  WetaAdcChannel;

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
    WetaPin			pin;
    WetaGpioMode	dir;
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
    WetaAdcChannel*     channels;
    uint8_t         n_channels;
    //WetaAdcDepth    depth;
} Adc;

typedef WetaPin PwmChannel;

typedef struct
{
    PwmChannel*		channels;
    uint8_t			n_channels;
} PwmChannels;

typedef struct
{
    uint8_t  port;
    uint32_t baud;
    uint8_t  config;
} SerialPort;

typedef struct
{
    SerialPort* ports;
    uint8_t     n_ports;
} SerialPorts;

#endif  //__HW_STRUCTS_H__