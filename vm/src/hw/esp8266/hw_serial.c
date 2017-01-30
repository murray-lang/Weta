#include "../hw_serial.h"
#include "../hw_time.h"
#include <driver/uart.h>
#include <lwip/sys.h>


void WETAFUNCATTR    
hw_serial_init(uint16_t flags)
{
	flags = flags;
}

void WETAFUNCATTR    
hw_serial_start(SPORT port)
{
	//Nothing to do
}

void WETAFUNCATTR    
hw_serial_start_all(SerialPorts* ports)
{
	//Nothing to do
}

uint8_t WETAFUNCATTR 
hw_serial_read(SPORT port, uint8_t* buf, uint8_t length, int16_t timeout)
{
	uint32 startTime = hw_time_now();
	uint16 available = 0;
		// Keep checking the available bytes until the timeout expires
		// (or forever if the timeout is infinite (<0))
	while ((available = uart_available_bytes(port)) < length)
	{
		if (timeout >= 0 && (system_get_time() - startTime)/1000 >= (uint32)timeout)
			break;
		hw_time_waitms(1000);
	}
	if (available == 0)
		return 0;
		// Get whatever is available now
	return (uint8_t)uart_read_bytes(port, buf, available);
}

uint8_t WETAFUNCATTR    
hw_serial_read_byte(SPORT port)
{
	uint8_t val;
	hw_serial_read(port, &val, 1, -1);
	return val;
}

uint8_t WETAFUNCATTR 
hw_serial_write(SPORT port, uint8_t* buf, uint8_t length)
{
	uart_tx_bytes(port, buf, (uint16)length);
	return length; // Optimism born of necessity
}

void WETAFUNCATTR    
hw_serial_write_byte(SPORT port, uint8_t value)
{
	uart_tx_byte(port, value);
}

bool WETAFUNCATTR    
hw_serial_available(SPORT port)
{
	return uart_available_bytes(port) > 0;
}
