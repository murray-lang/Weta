#ifndef UART_UTIL_H
#define UART_UTIL_H
#include <weta_platform.h>
//#include "uart_register.h"
//#include "eagle_soc.h"
#include <uart.h>


extern void uart_register_intr_handler(void);
extern void uart_rx_intr_handler(void *para);

extern void uart_tx_byte(uint8 port, uint8 val);
extern void uart_tx_bytes(uint8 port, uint8* bytes, uint16 length);

extern bool   uart_data_available(uint8 port);
extern uint16 uart_available_bytes(uint8 port);
extern uint16 uart_read_bytes(uint8 port, uint8* bytes, uint16 length);
extern void   uart_reset_rx_buffer(uint8 port);
extern void   uart_enable_rx_interrupt(uint8 port);
extern void   uart_disable_rx_interrupt(uint8 port);

#endif

