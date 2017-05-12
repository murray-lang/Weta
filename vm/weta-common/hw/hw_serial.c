
#include <hw.h>

uint8_t WETAFUNCATTR
hw_serial_read_i(SerialPort *ports, uint8_t i, uint8_t *buf, uint8_t length, int16_t timeout)
{
    if (i >= MAX_UARTS)
        return 0;

    return hw_serial_read(&ports[i], buf, length, timeout);
}

uint8_t WETAFUNCATTR
hw_serial_read_byte_i(SerialPort *ports, uint8_t i)
{
    if (i >= MAX_UARTS)
        return 0;   // Hmm... should return value as param

    return hw_serial_read_byte(&ports[i]);
}

uint8_t WETAFUNCATTR
hw_serial_write_i(SerialPort *ports, uint8_t i, const uint8_t *buf, uint8_t length)
{
    if (i >= MAX_UARTS)
        return 0;

    return hw_serial_write(&ports[i], buf, length);
}

uint8_t WETAFUNCATTR
hw_serial_write_byte_i(SerialPort *ports, uint8_t i, uint8_t value)
{
    if (i >= MAX_UARTS)
        return 0;

    return hw_serial_write_byte(&ports[i], value);
}

bool WETAFUNCATTR
hw_serial_available_i(SerialPort *ports, uint8_t i)
{
    if (i >= MAX_UARTS)
        return false;
    return hw_serial_available(&ports[i]);
}

void hw_serial_flush_i(SerialPort *ports, uint8_t i)
{
    if (i < MAX_UARTS)
        return hw_serial_flush(&ports[i]);
}

