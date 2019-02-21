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
#include "board.h"
#include "string.h"

#include "VirtualSerial.h"
#include "usb_common.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#if defined(USB_CDC_CLASS) || defined (USB_COMP_MOUSE_CDC_CLASS)
/* Transmit and receive ring buffers */
RINGBUFF_T cdc_txring;

/* Transmit and receive buffers */
uint8_t						cdc_txbuff[CDC_UART_RB_SIZE];
USB_ClassInfo_CDC_Device_t	*CDC_IF_PTR;

#endif // #if defid(USB_CDC_CLASS) || (USB_COMP_MOUSE_CDC_CLASS)

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/** LPCUSBlib CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface = {
	.Config = {
		.ControlInterfaceNumber         = 0,

		.DataINEndpointNumber           = CDC_TX_EPNUM,
		.DataINEndpointSize             = CDC_TXRX_EPSIZE,
		.DataINEndpointDoubleBank       = false,

		.DataOUTEndpointNumber          = CDC_RX_EPNUM,
		.DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
		.DataOUTEndpointDoubleBank      = false,

		.NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
		.NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
		.NotificationEndpointDoubleBank = false,
		.PortNumber             = 0,
	},
};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs
 */
// static FILE USBSerialStream;

/*****************************************************************************
 * Private functions
 ****************************************************************************/
/*****************************************************************************
 * Public functions
****************************************************************************/

//inline void VirtualSerial_OneByteToHost(uint8_t output_char)
//{
//	uint8_t to_host_data[1];
//
//	to_host_data[0] = output_char;
//	CDC_Device_SendData(&VirtualSerial_CDC_Interface, (char *)  to_host_data, 1);
//}
//
//inline void VirtualSerial_MultiByteToHost(uint8_t *to_host_data, uint16_t bytes_to_write )
//{
//	CDC_Device_SendData(&VirtualSerial_CDC_Interface, (char *) to_host_data, bytes_to_write);
//}

//void VirtualSerial_OneByteToHost(uint8_t output_char)
//{
//	uint8_t to_host_data[1], ret;
//
//	to_host_data[0] = output_char;
//	ret = RingBuffer_Insert(&cdc_txring, to_host_data);
//	while(ret==0)
//	{
//		// Cannot insert to Ring buffer -> need to consume it first
//		VirtualSerial_FinishDataTyHost();
//		ret = RingBuffer_Insert(&cdc_txring, to_host_data);
//	}
//}
//
//void VirtualSerial_MultiByteToHost(uint8_t *to_host_data, uint16_t bytes_to_write )
//{
//	int ret, bytes = bytes_to_write;
//	uint8_t *p8 = to_host_data;
//
//	ret = RingBuffer_InsertMult(&cdc_txring, p8, bytes);
//	bytes -= ret;
//	p8 += ret;
//	VirtualSerial_FinishDataTyHost();
//
//	/* Do this till all bytes are queued */
//	while (bytes) {
//		/* A proper wait handler must be added here */
//		ret = RingBuffer_InsertMult(&cdc_txring, p8, bytes);
//		bytes -= ret;
//		p8 += ret;
//	}
//}

void VirtualSerial_OneByteToHost(uint8_t output_char)
{
	uint8_t to_host_data[1], ret;

	to_host_data[0] = output_char;
	ret = RingBuffer_Insert(&cdc_txring, to_host_data);
	while(ret==0)
	{
		// Cannot insert to Ring buffer -> need to consume it first
		VirtualSerial_FinishDataTyHost();
		ret = RingBuffer_Insert(&cdc_txring, to_host_data);
	}
}

void VirtualSerial_MultiByteToHost(uint8_t *to_host_data, uint16_t bytes_to_write )
{
	int ret, bytes = bytes_to_write;
	uint8_t *p8 = to_host_data;

	ret = RingBuffer_InsertMult(&cdc_txring, p8, bytes);
	bytes -= ret;
	p8 += ret;

	/* Do this till all bytes are queued */
	while (bytes>0)
	{
		// Flush out ring buffer data before continuing.
		VirtualSerial_FinishDataTyHost();
		/* A proper wait handler must be added here */
		ret = RingBuffer_InsertMult(&cdc_txring, p8, bytes);
		bytes -= ret;
		p8 += ret;
	}
}

void VirtualSerial_FinishDataTyHost(void)
{
	int bytes_to_write = -1;
	uint8_t to_host_data[VIRTUAL_SERIAL_UART_SIZE];

	bytes_to_write = RingBuffer_PopMult(&cdc_txring, (void *) to_host_data, VIRTUAL_SERIAL_UART_SIZE);
	while(bytes_to_write>0)
	{
		CDC_Device_SendData(CDC_IF_PTR, (char *) to_host_data, bytes_to_write);
		bytes_to_write = RingBuffer_PopMult(&cdc_txring, (void *) to_host_data, VIRTUAL_SERIAL_UART_SIZE);
	}
}

int16_t VirtualSerial_OneByteFromHost(uint8_t *from_host_data)
{
	if (CDC_Device_BytesReceived(CDC_IF_PTR))
	{
		from_host_data[0] = CDC_Device_ReceiveByte(CDC_IF_PTR);
		return 1;
	}
	else
	{
		return 0;
	}
}

int16_t VirtualSerial_MultiByteFromHost(uint8_t *from_host_data, uint16_t bytes_to_read )
{
	uint16_t	remaining_byte = bytes_to_read, index = 0;
	while ((remaining_byte>0)&&(CDC_Device_BytesReceived(CDC_IF_PTR)))
	{
		from_host_data[index] = CDC_Device_ReceiveByte(CDC_IF_PTR);
		remaining_byte--;
		index++;
	}
	return index;
}

#ifdef USB_CDC_CLASS

void VirtualSerial_USB_USBTask(void)
{
	// CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
	USB_USBTask(CDC_IF_PTR->Config.PortNumber, USB_MODE_Device);
}

//
// Common function to be implemented
//

void VirtualSerial_Init(void)
{
	CDC_IF_PTR = &VirtualSerial_CDC_Interface;

	USB_Init(CDC_IF_PTR->Config.PortNumber, USB_MODE_Device);

	RingBuffer_Init(&cdc_txring, cdc_txbuff, 1, CDC_UART_RB_SIZE);
}

// Virtual Serial
void MyUSB_Init(void)
{
	// Init Descriptor
	DeviceDescriptor = DeviceDescriptor_CDC;
	ConfigurationDescriptor = ConfigurationDescriptor_CDC;
	LanguageStringPtr = (USB_Descriptor_String_t *) LanguageString_CDC;
	ManufacturerStringPtr = (USB_Descriptor_String_t *) ManufacturerString_CDC;
	ProductStringPtr = (USB_Descriptor_String_t *) ProductString_CDC;
	// Init others
	VirtualSerial_Init();
}

inline void USB_task_in_main_loop(void)
{
	VirtualSerial_USB_USBTask();
	VirtualSerial_FinishDataTyHost();
}
// End of Virtual Serial
#endif // #ifdef USB_CDC_CLASS
