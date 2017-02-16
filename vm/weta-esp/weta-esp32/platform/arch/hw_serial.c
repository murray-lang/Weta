#include <hw_serial.h>
//#include "freertos/FreeRTOS.h"
#include <driver/uart.h>
#include <stdio.h> // Debugging with printf()


#define UART_INTERRUPT_BASE 17

//static uart_dev_t* const uarts[UART_NUM_MAX] = { &UART0, &UART1, &UART2 };
//static QueueHandle_t queues[UART_NUM_MAX] = { 0, 0, 0 };

void WETAFUNCATTR    
hw_serial_init(SerialPorts* ports, uint16_t flags)
{
	flags = flags;

	int i;
    for (i = 0; i < ports->n_ports; i++)
    {
        uart_config_t uart_config = {
            .baud_rate           = (int)ports->ports[i].baud,
            .data_bits           = (uart_word_length_t)ports->ports[i].data_bits,
            .parity              = (uart_parity_t)ports->ports[i].parity,
            .stop_bits           = (uart_stop_bits_t)ports->ports[i].stop_bits,
            .flow_ctrl           = ports->ports[i].port == 1 ? UART_HW_FLOWCTRL_DISABLE : UART_HW_FLOWCTRL_RTS,
            .rx_flow_ctrl_thresh = 120,
        };

        uart_param_config((uart_port_t)ports->ports[i].port, &uart_config);

        uart_driver_install(
                (uart_port_t)ports->ports[i].port,
                1024,
                0,
                0,
                NULL, //&queues[ports->ports[i].port]
				0
        );

    }
    uart_set_pin(1, PIN_UART1_TX, PIN_UART1_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}


uint8_t WETAFUNCATTR 
hw_serial_read(SerialPort* port, uint8_t* buf, uint8_t length, int16_t timeout)
{
		// Get whatever is available now
	return (uint8_t)uart_read_bytes(
			(uart_port_t)port->port,
			buf,
			(uint32_t)length,
			timeout/portTICK_RATE_MS);
}

uint8_t WETAFUNCATTR    
hw_serial_read_byte(SerialPort* port)
{
	uint8_t val;
	hw_serial_read(port, &val, 1, -1);
	return val;
}

uint8_t WETAFUNCATTR 
hw_serial_write(SerialPort* port, const uint8_t* buf, uint8_t length)
{
	uart_write_bytes((uart_port_t)port->port, (const char*)buf, (size_t)length);
	return length; // Optimism born of necessity
}

uint8_t WETAFUNCATTR
hw_serial_write_byte(SerialPort* port, uint8_t value)
{
	return hw_serial_write(port, &value, 1);
}

bool WETAFUNCATTR    
hw_serial_available(SerialPort* port)
{

    size_t avail = uart_read_bytes_available(port->port);
    //if (avail > 0)
    //    printf("%d bytes available\r\n", avail);
    return avail > 0;
	//return ((READ_PERI_REG(UART_STATUS_REG(port->port))>>UART_RXFIFO_CNT_S) & UART_RXFIFO_CNT) > 0;

	//return uarts[port->port]->status.rxfifo_cnt > 0;
}

void WETAFUNCATTR
hw_serial_flush(SerialPort* port)
{
    uart_flush((uart_port_t)port->port);
}


