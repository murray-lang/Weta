#ifndef __HW_SERIAL_H__
#define  __HW_SERIAL_H__

#include <weta_platform.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _Hardware;

extern void hw_serial_init(struct _Hardware* hw, uint16_t flags);

extern uint8_t hw_serial_read_i(SerialPorts *ports, uint8_t i, uint8_t *buf, uint8_t length, int16_t timeout);
extern uint8_t hw_serial_read(SerialPort *port, uint8_t *buf, uint8_t length, int16_t timeout);

extern uint8_t hw_serial_read_byte_i(SerialPorts *ports, uint8_t i);
extern uint8_t hw_serial_read_byte(SerialPort *port);

extern uint8_t hw_serial_write_i(SerialPorts *ports, uint8_t i, const uint8_t *buf, uint8_t length);
extern uint8_t hw_serial_write(SerialPort *port, const uint8_t *buf, uint8_t length);

extern uint8_t hw_serial_write_byte_i(SerialPorts *ports, uint8_t i, uint8_t value);
extern uint8_t hw_serial_write_byte(SerialPort *port, uint8_t value);

extern bool hw_serial_available_i(SerialPorts *ports, uint8_t i);
extern bool hw_serial_available(SerialPort *port);

extern void hw_serial_flush_i(SerialPorts *ports, uint8_t i);
extern void hw_serial_flush(SerialPort *port);

#ifdef __cplusplus
}
#endif

#endif // __HW_SERIAL_H__