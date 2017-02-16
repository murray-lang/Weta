/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: uart.c
 *
 * Description: Two UART mode configration and interrupt handler.
 *              Check your hardware connection while use this mode.
 *
 * Modification history:
 *     2014/3/12, v1.0 create this file.
*******************************************************************************/
#include <weta_platform.h>
#include <os_type.h>
#include <ets_sys.h>
#include <eagle_soc.h>
#include <uart_register.h>
#include "uart_util.h"


// UartDev is defined and initialized in rom code.
extern UartDevice UartDev;
static uint8_t uart1Buffer[UART_RX_BUFFER_SIZE] = {0};

RcvMsgBuff uart1RxBuffer =
    {
        .RcvBuffSize = sizeof(uart1Buffer),
        .pRcvMsgBuff = uart1Buffer,
        .pWritePos   = uart1Buffer,
        .pReadPos    = uart1Buffer,
        .TrigLvl     = 0,
        .BuffState   = EMPTY
    };

static void uart_get_intr_data(uint8_t port, RcvMsgBuff* pRxBuff);


void ICACHE_FLASH_ATTR
uart_rx_intr_handler(void *para)
{
	/* uart0 and uart1 intr combine together, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
	 * uart1 and uart0 respectively
	 */
    if(READ_PERI_REG(UART_INT_ST(UART0)))	//any UART0 stuff
    {
        uart_get_intr_data(UART0, &UartDev.rcv_buff);
    }
	if(READ_PERI_REG(UART_INT_ST(UART1)))	//any UART1 stuff
	{
        uart_get_intr_data(UART1, &uart1RxBuffer);
	}


}

void ICACHE_FLASH_ATTR
uart_register_intr_handler(void)
{
    ETS_UART_INTR_ATTACH(uart_rx_intr_handler, NULL);
}

/******************************************************************************
 * FunctionName : uart_tx_one_char
 * Description  : Internal used function
 *                Use uart1 interface to transfer one char
 * Parameters   : uint8 TxChar - character to tx
 * Returns      : OK
*******************************************************************************/
void ICACHE_FLASH_ATTR
uart_tx_byte(uint8 uart, uint8 TxChar)
{
    while (true){
        uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(uart)) & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S);
        if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126) {
            break;
        }
    }
    WRITE_PERI_REG(UART_FIFO(uart) , TxChar);
}

void ICACHE_FLASH_ATTR
uart_tx_bytes(uint8 port, uint8* bytes, uint16 length)
{
	uint16 i;

    for (i = 0; i < length; i++) 
	{
        uart_tx_byte(port, bytes[i]);
    }
}

bool ICACHE_FLASH_ATTR
uart_data_available(uint8 port)
{
	if (port == 0)
		return UartDev.rcv_buff.BuffState != EMPTY;
    if (port == 1)
        return uart1RxBuffer.BuffState != EMPTY;
	return false;
}

uint16 ICACHE_FLASH_ATTR
uart_available_bytes(uint8 port)
{
    RcvMsgBuff* buff;

    if (port == 0)
        buff = &UartDev.rcv_buff;
	else if (port == 1)
		buff = &uart1RxBuffer;
    else
        return 0;

	uart_disable_rx_interrupt(port);
	uint16 count;
		// If the read position and write position are the same then there is 
		// no data.
		// If the write position is after the read position then the available
		// data is the difference.
		// If the read position is after the write position then the write 
		// position has wrapped and the available data is the remainder of
		// the buffer after the read position plus the write position.
	if (buff->pWritePos == buff->pReadPos)
	{
		count = 0;
		
	}
	else if (buff->pWritePos > buff->pReadPos)
	{
		count = buff->pWritePos - buff->pReadPos;
	}
	else
	{
		count = UART_RX_BUFFER_SIZE - (buff->pReadPos - buff->pRcvMsgBuff)
			+ (buff->pWritePos - buff->pRcvMsgBuff);
	}
	uart_enable_rx_interrupt(port);
	return count;
}

uint16 ICACHE_FLASH_ATTR
uart_read_bytes(uint8 port, uint8* bytes, uint16 length)
{
	if (port == 1)
		return 0;	// Can't deal with uart1 at the moment
	
	uint16 toread = uart_available_bytes(port);
	if (toread == 0)
		return 0;
	if (toread > length)
		toread = length;
	uint16 i;
	for (i = 0; i < toread; i++)
	{
		if (UartDev.rcv_buff.pReadPos == UartDev.rcv_buff.pRcvMsgBuff + UART_RX_BUFFER_SIZE)
			UartDev.rcv_buff.pReadPos = UartDev.rcv_buff.pRcvMsgBuff;
		bytes[i] = *UartDev.rcv_buff.pReadPos++;
	}
	return toread;
}

void ICACHE_FLASH_ATTR   
uart_enable_rx_interrupt(uint8 port)
{
	SET_PERI_REG_MASK(UART_INT_ENA(port), UART_RXFIFO_FULL_INT_ENA);
}

void ICACHE_FLASH_ATTR  
uart_disable_rx_interrupt(uint8 port)
{
	CLEAR_PERI_REG_MASK(UART_INT_ENA(port), UART_RXFIFO_FULL_INT_ENA);
}

/******************************************************************************
 * FunctionName : uart1_write_char
 * Description  : Internal used function
 *                Do some special deal while tx char is '\r' or '\n'
 * Parameters   : char c - character to tx
 * Returns      : NONE
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
uart1_write_char(char c)
{
    if (c == '\n') {
        uart_tx_byte(1, '\r');
        uart_tx_byte(1, '\n');
    } else if (c == '\r') {
    } else {
        uart_tx_byte(1, c);
    }
}


void ICACHE_FLASH_ATTR
uart_get_intr_data(uint8_t port, RcvMsgBuff* pRxBuff)
{
    /* uart0 and uart1 intr combine togther, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
     * uart1 and uart0 respectively
     */
    uint8 RcvChar;

    if (UART_RXFIFO_FULL_INT_ST != (READ_PERI_REG(UART_INT_ST(port)) & UART_RXFIFO_FULL_INT_ST)) {
        return;
    }

    WRITE_PERI_REG(UART_INT_CLR(port), UART_RXFIFO_FULL_INT_CLR);

    while (READ_PERI_REG(UART_STATUS(port)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S)) {
        RcvChar = READ_PERI_REG(UART_FIFO(port)) & 0xFF;

        /* you can add your handle code below.*/
        *(pRxBuff->pWritePos) = RcvChar;

        // insert here for get one command line from uart
        if (RcvChar == '\r') {
            pRxBuff->BuffState = WRITE_OVER;
        }

        pRxBuff->pWritePos++;

        if (pRxBuff->pWritePos == (pRxBuff->pRcvMsgBuff + UART_RX_BUFFER_SIZE)) {
            // overflow ...we may need more error handle here.
            pRxBuff->pWritePos = pRxBuff->pRcvMsgBuff ;
        }
    }
}

