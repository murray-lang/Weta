#include <hw.h>
//#include "freertos/FreeRTOS.h"
#include <driver/uart.h>
#include <stdio.h> // Debugging with printf()


#define UART_INTERRUPT_BASE 17

//static uart_dev_t* const uarts[UART_NUM_MAX] = { &UART0, &UART1, &UART2 };
//static QueueHandle_t queues[UART_NUM_MAX] = { 0, 0, 0 };

void WETAFUNCATTR    
hw_serial_init(struct _Hardware* hw, uint16_t flags)
{
	flags = flags;

	int i;
    for (i = 0; i < MAX_UARTS; i++)
    {
        if (hw->sports[i].port != 0xFF)
            hw_serial_config(hw, &hw->sports[i]);

            // Espressif has written uart_driver_install() in a way that assumes
            // that it is only called once. So just install them all now.
        uart_driver_install(
                (uart_port_t)i,
                1024,
                0,
                0,
                NULL, //&queues[ports->ports[i].port]
				0
        );

    }
    //uart_set_pin(1, PIN_UART1_TX, PIN_UART1_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

bool WETAFUNCATTR
hw_serial_config(
    struct _Hardware* hw,
    SerialPort* config
)
{
    if (config->port >= MAX_UARTS)
        return false;

        // If the SerialPort structure is not already pointing to our internal
        // structure then copy it across member for member
    if (config != &hw->sports[config->port]) {
        hw->sports[config->port] = *config;
    }

    uart_config_t uart_config = {
        .baud_rate           = (int)config->baud,
        .data_bits           = config->params.databits == SERIAL_DATABITS_5 ? UART_DATA_5_BITS :
                               config->params.databits == SERIAL_DATABITS_6 ? UART_DATA_6_BITS :
                               config->params.databits == SERIAL_DATABITS_7 ? UART_DATA_7_BITS :
                               UART_DATA_8_BITS,
        .parity              = config->params.parity == SERIAL_PARITY_NONE ? UART_PARITY_DISABLE :
                               config->params.parity == SERIAL_PARITY_EVEN ? UART_PARITY_EVEN :
                               UART_PARITY_ODD,
        .stop_bits           = config->params.stopbits == SERIAL_STOPBITS_1 ? UART_STOP_BITS_1 :
                               config->params.stopbits == SERIAL_STOPBITS_1P5 ? UART_STOP_BITS_1_5 :
                               UART_STOP_BITS_2,
        .flow_ctrl           = config->params.flow == SERIAL_FLOW_DISABLE ? UART_HW_FLOWCTRL_DISABLE :
                               config->params.flow == SERIAL_FLOW_RTS ? UART_HW_FLOWCTRL_RTS:
                               config->params.flow == SERIAL_FLOW_CTS ? UART_HW_FLOWCTRL_CTS:
                               UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 120,
    };

    return uart_param_config(config->port, &uart_config) == ESP_OK;
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




