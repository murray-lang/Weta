#include "../hw_serial.h"
#include <ch.h>
#include <hal.h>
#include <chprintf.h> // For debugging


void    
hw_serial_init(uint16_t flags)
{
	flags = flags;
	//sdInit();
}

void    
hw_serial_start(PSERIAL port)
{
	sdStart((SerialDriver *)port, NULL);
}

void    
hw_serial_start_all(SerialPorts* ports)
{
	//SerialConfig sc;
	//sc.sc_speed = 115200;
	sdStart(&SD1, NULL);
	sdStart(&SD2,NULL);
	/*
	uint8_t i;
	for (i = 0; i < ports->n_ports; i++)
		hw_serial_start(ports->ports[i]);
	*/
}

uint8_t 
hw_serial_read(PSERIAL port, uint8_t* buf, uint8_t length, int16_t timeout)
{
		// Negative timeout means infinite
	return (uint8_t)chIOReadTimeout(
		(SerialDriver *)port, 
		buf, 
		length, 
		timeout < 0 ? TIME_INFINITE : (uint16_t)timeout);
}

uint8_t    
hw_serial_read_byte(PSERIAL port)
{
	//msg_t c = chIOGet(&SD1);
	//chprintf(&SD2, "hw_serial_read_byte : %d\r\n", c);
	return chIOGet((SerialDriver *)port);
}

uint8_t 
hw_serial_write(PSERIAL port, uint8_t* buf, uint8_t length)
{
	return (uint8_t)chIOWriteTimeout((SerialDriver *)port, buf, length, TIME_INFINITE);
}

void    
hw_serial_write_byte(PSERIAL port, uint8_t value)
{
	//chprintf(&SD2, "hw_serial_write_byte : %d\r\n", value);
	chIOPut((SerialDriver *)port, value);
}

bool    
hw_serial_available(PSERIAL port)
{
	return !chIOGetWouldBlock((SerialDriver *)port);
}