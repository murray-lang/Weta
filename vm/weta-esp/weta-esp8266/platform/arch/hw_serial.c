#include <weta_platform.h>
#include <hw_time.h>
#include <uart_register.h>
#include "uart_util.h"


void WETAFUNCATTR    
hw_serial_init(SerialPorts *sports, uint16_t flags)
{
	flags = flags;

	int i;
	for (i = 0; i < sports->n_ports; i++)
	{
		UART_SetBaudrate(sports->ports[i].port, sports->ports[i].baud);
		UART_SetWordLength(sports->ports[i].port, sports->ports[i].data_bits);
		UART_SetStopBits(sports->ports[i].port, sports->ports[i].stop_bits);
		UART_SetParity(sports->ports[i].port, sports->ports[i].parity);
	}
}

uint8_t WETAFUNCATTR
hw_serial_read(SerialPort *port, uint8_t* buf, uint8_t length, int16_t timeout)
{
	uint32 startTime = hw_time_now();
	uint16 available = 0;
		// Keep checking the available bytes until the timeout expires
		// (or forever if the timeout is infinite (<0))
	while ((available = uart_available_bytes(port->port)) < length)
	{
		if (timeout >= 0 && (hw_time_now() - startTime)/*/1000*/ >= (uint32)timeout)
			break;
		hw_time_waitms(1000);
	}
	if (available == 0)
		return 0;
		// Get whatever is available now
	return (uint8_t)uart_read_bytes(port->port, buf, available);
}

uint8_t WETAFUNCATTR    
hw_serial_read_byte(SerialPort *port)
{
	uint8_t val;
	hw_serial_read(port, &val, 1, -1);
	return val;
}

uint8_t WETAFUNCATTR 
hw_serial_write(SerialPort *port, uint8_t* buf, uint8_t length)
{
	uart_tx_bytes(port->port, buf, (uint16)length);
	return length; // Optimism born of necessity
}

void WETAFUNCATTR    
hw_serial_write_byte(SerialPort *port, uint8_t value)
{
	uart_tx_byte(port->port, value);
}

bool WETAFUNCATTR    
hw_serial_available(SerialPort *port)
{
	return uart_data_available(port->port);
}

void WETAFUNCATTR
hw_serial_flush(void)
{

}
