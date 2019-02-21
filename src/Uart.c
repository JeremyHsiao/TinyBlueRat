/*==========================================================================
|
|  Project Name : LCD TV RC controller
|
|  File Name	: Uart.c
|
|===========================================================================
|
|  Copyright TPV Technology Group /P-Harmony Monitors (Taiwan) Ltd.
|  All rights are reserved. Reproduction in full or in part is prohibited
|  without the prior written consent of the copyright owner.
|
|  COMPANY CONFIDENTIAL
|
|  Date 		Author(s)		Status & Comments
|---------------------------------------------------------------------------
|  2007/04/27	HW Hsu 			Draft
|
|===========================================================================
|
|  Brief Description
|
|  This file is Uart control.
|
|  Note
|
|  The KEIL C51 Compiler ( Version 7.07a ) is used.
|
==========================================================================*/

//#define _UART_C_

#include "chip.h"
#include "board.h"
#include "string.h"
#include "Uart.h"
#include "Io.h"
#include "LPC11U37_401.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Transmit and receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Ring buffer size */
#define UART_RB_SIZE (256)		// Due to ring buffer implementation, UART_RB_SIZE must be a number of 2 ^ n

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RB_SIZE], txbuff[UART_RB_SIZE];


/*****************************************************************************
** Function name:		UART_IRQHandler
**
** Descriptions:		UART interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART_IRQHandler(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}

/*****************************************************************************
** Function name:		UARTInit
**
** Descriptions:		Initialize UART0 port, setup pin select,
**				clock, parity, stop bits, FIFO, etc.
**
** parameters:			UART baudrate
** Returned value:		None
**
*****************************************************************************/
int UARTInit(uint32_t baudrate)
{
// Jeremy: board is assumed to be inited before.
//	Board_Init();
	Board_UART_Init();
	// TxRx Pin Mux has been already set in  SystemInit();
	//Chip_IOCON_PinMuxSet(LPC_IOCON, UART_RX_GPIO_PORT_NUM, UART_RX_GPIO_BIT_NUM, (PIO0_18_FUNC_RX | PIO0_18_DEFAULT));
	//Chip_IOCON_PinMuxSet(LPC_IOCON, UART_TX_GPIO_PORT_NUM, UART_TX_GPIO_BIT_NUM, (PIO0_19_FUNC_TX | PIO0_19_DEFAULT));

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, baudrate);

	// Jeremy: use 2 stop bits for higher baudrate
	if (baudrate>115200)
	{
		Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_2BIT));
	}
	else
	{
		Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	}
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);

	/* Before using the ring buffers, initialize them using the ring
   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_RB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);

	return 1;

}

int UART_DeInit(void)
{
	/* DeInitialize UART0 peripheral */
	NVIC_DisableIRQ(UART0_IRQn);
	Chip_UART_DeInit(LPC_USART);

	return 1;
}

//
// Here is my own API based on LPC Open API
//
inline int ReadOneByteFromUARTRingBuffer(uint8_t *return_data)
{
	int pop_byte_cnt;

	//int Chip_UART_ReadRB(LPC_USART_T *pUART, RINGBUFF_T *pRB, void *data, int bytes)
	pop_byte_cnt = Chip_UART_ReadRB(LPC_USART, &rxring, return_data, 1);
	return pop_byte_cnt;
}

inline void WriteOneByteToUARTRingBuffer(uint8_t sent_data)
{
	uint8_t temp_data[1];

	temp_data[0] = sent_data;
	Chip_UART_SendRB(LPC_USART, &txring, temp_data, 1);
}

inline int ReadMultiByteFromUARTRingBuffer(uint8_t *return_data, uint16_t bytes_to_read)
{
	int pop_byte_cnt;

	pop_byte_cnt = Chip_UART_ReadRB(LPC_USART, &rxring, return_data, bytes_to_read);
	return pop_byte_cnt;
}

inline void WriteMultiByteToUARTRingBuffer(uint8_t *sent_data, uint16_t bytes_to_write)
{
	Chip_UART_SendRB(LPC_USART, &txring, (const void *)sent_data, bytes_to_write);
}

inline void WriteMultiByteToUARTWaitFinish(uint8_t *sent_data, uint16_t bytes_to_write)
{
	Chip_UART_SendBlocking(LPC_USART, (const void *)sent_data, bytes_to_write);
}



