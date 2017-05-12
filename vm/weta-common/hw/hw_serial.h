#ifndef __HW_SERIAL_H__
#define  __HW_SERIAL_H__

#include <weta_platform.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum  {
    SERIAL_DATABITS_5,
    SERIAL_DATABITS_6,
    SERIAL_DATABITS_7,
    SERIAL_DATABITS_8
} eSerialDataBits;

typedef enum  {
    SERIAL_PARITY_NONE,
    SERIAL_PARITY_ODD,
    SERIAL_PARITY_EVEN
} eSerialParity;

typedef enum  {
    SERIAL_STOPBITS_NONE,   // Never used - just a placeholder
    SERIAL_STOPBITS_1,
    SERIAL_STOPBITS_2,
    SERIAL_STOPBITS_1P5
} eSerialStopBits;

typedef enum  {
    SERIAL_FLOW_DISABLE,
    SERIAL_FLOW_RTS,
    SERIAL_FLOW_CTS,
    SERIAL_FLOW_RTS_CTS
} eSerialFlow;

typedef struct
{
    eSerialStopBits	stopbits : 2;
    eSerialParity   parity	 : 2;
    eSerialDataBits databits : 2;
    eSerialFlow     flow     : 2;
} SerialParams;

typedef struct
{
    uint8_t      port;
    uint32_t     baud;
    union
    {
        SerialParams params;
        uint8_t      paramsbyte;
    };
} SerialPort;

typedef SerialPort SerialPorts[MAX_UARTS];

struct _Hardware;

extern void hw_serial_init(struct _Hardware* hw, uint16_t flags);

extern bool hw_serial_config(
    struct _Hardware* hw,
    SerialPort * config
);

extern uint8_t hw_serial_read_i(
    SerialPort *ports,
    uint8_t i,
    uint8_t *buf,
    uint8_t length,
    int16_t timeout
);
extern uint8_t hw_serial_read(
    SerialPort *port,
    uint8_t *buf,
    uint8_t length,
    int16_t timeout
);

extern uint8_t hw_serial_read_byte_i(
    SerialPort *ports,
    uint8_t i
);
extern uint8_t hw_serial_read_byte(SerialPort *port);

extern uint8_t hw_serial_write_i(
    SerialPort *ports,
    uint8_t i,
    const uint8_t *buf,
    uint8_t length
);
extern uint8_t hw_serial_write(
    SerialPort *port,
    const uint8_t *buf,
    uint8_t length);

extern uint8_t hw_serial_write_byte_i(
    SerialPort *ports,
    uint8_t i,
    uint8_t value);

extern uint8_t hw_serial_write_byte(
    SerialPort *port,
    uint8_t value
);

extern bool hw_serial_available_i(
    SerialPort *ports,
    uint8_t i
);
extern bool hw_serial_available(SerialPort *port);

extern void hw_serial_flush_i(
    SerialPort *ports,
    uint8_t i
);
extern void hw_serial_flush(SerialPort *port);

#ifdef __cplusplus
}
#endif

#endif // __HW_SERIAL_H__