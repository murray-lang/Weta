#include <weta_platform.h>
#include <hw.h>

uint8_t WETAFUNCATTR
hw_serial_read_i(SerialPorts *ports, uint8_t i, uint8_t *buf, uint8_t length, int16_t timeout)
{
    if (i >= ports->n_ports)
        return 0;

    return hw_serial_read(&ports->ports[i], buf, length, timeout);
}

uint8_t WETAFUNCATTR
hw_serial_read_byte_i(SerialPorts *ports, uint8_t i)
{
    if (i >= ports->n_ports)
        return 0;   // Hmm... should return value as param

    return hw_serial_read_byte(&ports->ports[i]);
}

uint8_t WETAFUNCATTR
hw_serial_write_i(SerialPorts *ports, uint8_t i, const uint8_t *buf, uint8_t length)
{
    if (i >= ports->n_ports)
        return 0;

    return hw_serial_write(&ports->ports[i], buf, length);
}

uint8_t WETAFUNCATTR
hw_serial_write_byte_i(SerialPorts *ports, uint8_t i, uint8_t value)
{
    if (i >= ports->n_ports)
        return 0;

    return hw_serial_write_byte(&ports->ports[i], value);
}

bool WETAFUNCATTR
hw_serial_available_i(SerialPorts *ports, uint8_t i)
{
    if (i >= ports->n_ports)
        return false;
    return hw_serial_available(&ports->ports[i]);
}

void hw_serial_flush_i(SerialPorts *ports, uint8_t i)
{
    if (i < ports->n_ports)
        return hw_serial_flush(&ports->ports[i]);
}

