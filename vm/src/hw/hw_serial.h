#ifndef __HW_SERIAL_H__
#define  __HW_SERIAL_H__

#include <weta_platform.h>
#include <stdbool.h>

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

extern void    hw_serial_init(SerialPorts* sports, uint16_t flags);
extern uint8_t hw_serial_read(SerialPort* port, uint8_t* buf, uint8_t length, int16_t timeout);
extern uint8_t hw_serial_read_byte(SerialPort* port);
extern uint8_t hw_serial_write(SerialPort* port, const uint8_t* buf, uint8_t length);
extern void    hw_serial_write_byte(SerialPort* port, uint8_t value);
extern bool    hw_serial_available(SerialPort* port);
extern void    hw_serial_flush(SerialPort* port);

#endif // __HW_SERIAL_H__