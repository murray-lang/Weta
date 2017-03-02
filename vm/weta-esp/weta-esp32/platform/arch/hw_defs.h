#ifndef __HW_DEFS_H__
#define __HW_DEFS_H__


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
    WetaPin			pin;
    WetaGpioMode	dir;
    WetaGpioMode	pull;
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

typedef struct
{
    WetaPwmChannel	channel;
    WetaPin		    pin;
} PwmChannel;

typedef struct
{
    PwmChannel*		channels;
    uint8_t			n_channels;
} PwmChannels;

typedef struct
{
    uint8_t  port;
    uint32_t baud;
    uint8_t  data_bits;
    uint8_t  parity;
    uint8_t  stop_bits;
} SerialPort;

typedef struct
{
    SerialPort* ports;
    uint8_t     n_ports;
} SerialPorts;

#endif  //__HW_DEFS_H__