/*
 * @brief Make your board becomes a USB Virtual Com
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * Copyright(C) Dean Camera, 2011, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"
#include "Board.h"
#include "string.h"
#include "i2c.h"
#include "Uart.h"
#include "usb_common.h"
#include "Timer.h"
#include "adc.h"
#include "ExtEEPROM.h"
#include "InternalEEpromAPI.h"
#include "VirtualSerial.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

extern int main_blinky(void);
extern void SystemInit(void);

/*****************************************************************************
 * Private functions
 ****************************************************************************/
/** Checks for data input, reply back to the host. */
void EchoCharacter(void)
{
	/* Echo back character */
	uint8_t recv_byte[1];
	if (VirtualSerial_OneByteFromHost(recv_byte)!=0) {
		VirtualSerial_OneByteToHost(recv_byte[0]);
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
static void SetupHardware(void)
{
    SystemInit();
	Board_Init();
	MyUSB_Init();
	UARTInit(115200);
#ifdef _MY_UNIT_TEST_
	main_blinky();
#else
	Timer_Init();
#endif // #ifdef _MY_UNIT_TEST_
	I2CInit();
	ADCInit();

}

char MyUARTTestMessageL1[] = "Welcome to the World.\r\n";
char MyUARTTestMessageL2[] = "Build date: " __DATE__ "\r\n";
char MyUARTTestMessageL3[] = "Build time: " __TIME__ "\r\n";

static void TestUARTBasicAPI(void)
{
	WriteMultiByteToUARTWaitFinish(MyUARTTestMessageL1,sizeof(MyUARTTestMessageL1)-1);
	WriteMultiByteToUARTRingBuffer(MyUARTTestMessageL2,sizeof(MyUARTTestMessageL2)-1);
	WriteMultiByteToUARTRingBuffer(MyUARTTestMessageL3,sizeof(MyUARTTestMessageL3)-1);
}

static void TestReadWriteExternalEEPROM(void)
{
	uint8_t	I2CRx[sizeof(MyUARTTestMessageL1)];

	WriteExtEEPROM_OneByte(0x102,'0');
	WriteExtEEPROM_MultiByte(0,MyUARTTestMessageL1,sizeof(MyUARTTestMessageL1)-1);
	WriteExtEEPROM_OneByte(1,'Z');
	ReadExtEEPROM(0, I2CRx, sizeof(MyUARTTestMessageL1)-1);
}

static void TestReadWriteInternalEEPROM(void)
{
	uint8_t		TestBuffer[sizeof(MyUARTTestMessageL1)];

	memcpy(TestBuffer,MyUARTTestMessageL1,sizeof(MyUARTTestMessageL1));
	writeInternalEEPROM(0x100, TestBuffer, sizeof(MyUARTTestMessageL1));

	// Clear Content before reading back so that we can be sure whether function works.
	memset(TestBuffer,0,sizeof(MyUARTTestMessageL1));
	readInternalEEPROM(0x100, TestBuffer, sizeof(MyUARTTestMessageL1));
}

static void TestUARTRxTx(void)
{
    int			bytes_to_read;
    uint8_t		uart_rx_data[32];

	bytes_to_read=ReadMultiByteFromUARTRingBuffer(uart_rx_data,32);
	while (bytes_to_read>0)
	{
		WriteMultiByteToUARTRingBuffer(uart_rx_data,bytes_to_read);
		bytes_to_read=ReadMultiByteFromUARTRingBuffer(uart_rx_data,32);
	}
}

static void TestADC_CH0(void)
{
	uint32_t	adc_value;
	uint8_t		value_string[8], index = 4;	// We want to see only 4 digits

	adc_value = ADCRead(ADC_CH0);

	do
	{
		uint8_t		temp;
		temp = (adc_value & 0x0f);
		if(temp>=10)
		{
			temp = temp - 10 + 'A';
		}
		else
		{
			temp = temp + '0';
		}
		index --;
		value_string[index] = temp;
		adc_value >>= 4;
	}
	while (index>0);

	WriteMultiByteToUARTRingBuffer(value_string,sizeof(value_string));

}

extern uint8_t show_message_off, show_message_on;
#ifdef _MY_UNIT_TEST_
void TestIrRx(void) {}
void TestIrTx(void) {}
void TestIrTxRxInit(void) {}
#else
void TestIrRx(void);
void TestIrTx(void);
void TestIrTxRxInit(void);
int app_main (void);
#endif //

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */

int main(void)
{
#ifdef _MY_UNIT_TEST_
	SetupHardware();
	TestIrTxRxInit();
	TestUARTBasicAPI();
	//TestReadWriteExternalEEPROM();
	//TestReadWriteInternalEEPROM();

	for (;; ) {
		EchoCharacter();				// Read and write back to Virtual Serial Port
		USB_task_in_main_loop();
//		if(show_message_off==1)
//		{
//			UARTputstr("123\r\n");
//			show_message_off=0;
//		}
//		if(show_message_on==1)
//		{
//			UARTputstr("987\r\n");
//			show_message_on=0;
//		}

		TestIrRx();
		TestUARTRxTx();
		//TestADC_CH0();

		USB_task_in_main_loop();
	}
#else
	app_main();
#endif
	return 0;
}

