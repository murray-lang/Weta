#include <weta_platform.h>
#include "Arduino.h"
#include <HardwareSerial.h>

extern "C" {

void WETAFUNCATTR
hw_serial_init(SerialPorts *ports, uint16_t flags)
{
    flags = flags;

    int i;
    for (i = 0; i < ports->n_ports; i++)
    {
        if (ports->ports[i].port == 0)
            Serial.begin(ports->ports[i].baud, ports->ports[i].config);
        else if (ports->ports[i].port == 1)
            Serial1.begin(ports->ports[i].baud, ports->ports[i].config);
        else if (ports->ports[i].port == 2)
            Serial2.begin(ports->ports[i].baud, ports->ports[i].config);
        else if (ports->ports[i].port == 3)
            Serial3.begin(ports->ports[i].baud, ports->ports[i].config);

    }
}

uint8_t WETAFUNCATTR
hw_serial_read(SerialPort *port, uint8_t *buf, uint8_t length, int16_t timeout)
{
    if (port->port == 0)
        return Serial.readBytes(buf, length);
    else if (port->port == 1)
        return Serial1.readBytes(buf, length);
    else if (port->port == 2)
        return Serial2.readBytes(buf, length);
    else if (port->port == 3)
        return Serial3.readBytes(buf, length);
    return 0;
}

uint8_t WETAFUNCATTR
hw_serial_read_byte(SerialPort *port)
{
    if (port->port == 0)
        return Serial.read();
    else if (port->port == 1)
        return Serial1.read();
    else if (port->port == 2)
        return Serial2.read();
    else if (port->port == 3)
        return Serial3.read();
    return 0;
}

uint8_t WETAFUNCATTR
hw_serial_write(SerialPort *port, const uint8_t *buf, uint8_t length)
{
    if (port->port == 0)
        return Serial.write(buf, length);
    else if (port->port == 1)
        return Serial1.write(buf, length);
    else if (port->port == 2)
        return Serial2.write(buf, length);
    else if (port->port == 3)
        return Serial3.write(buf, length);
    return 0;
}

uint8_t WETAFUNCATTR
hw_serial_write_byte(SerialPort *port, uint8_t value)
{
    if (port->port == 0)
        return Serial.write(value);
    else if (port->port == 1)
        return Serial1.write(value);
    else if (port->port == 2)
        return Serial2.write(value);
    else if (port->port == 3)
        return Serial3.write(value);
    return 0;
}

bool WETAFUNCATTR
hw_serial_available(SerialPort *port)
{
    if (port->port == 0)
        return Serial.available() > 0;
    else if (port->port == 1)
        return Serial1.available() > 0;
    else if (port->port == 2)
        return Serial2.available() > 0;
    else if (port->port == 3)
        return Serial3.available() > 0;
    return false;
}

void WETAFUNCATTR
hw_serial_flush(SerialPort *port)
{
    if (port->port == 0)
        Serial.flush();
    else if (port->port == 1)
        Serial1.flush();
    else if (port->port == 2)
        Serial2.flush();
    else if (port->port == 3)
        Serial3.flush();
}

} //extern "C"


