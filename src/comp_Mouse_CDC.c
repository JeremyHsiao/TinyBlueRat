/*
 * @brief Make your board becomes a USB mouse
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
#include "Io.h"
#include "usb_common.h"
#include "comp_Mouse_CDC.h"
#include "Descriptors_comp_Mouse_CDC.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/** Buffer to hold the previously generated Mouse HID report, for comparison purposes inside the HID class driver. */
static uint8_t comp_Mouse_CDC_PrevMouseHIDReportBuffer[sizeof(USB_MouseReport_Data_t)];

/** nxpUSBlib HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t comp_Mouse_CDC_Mouse_HID_Interface = {
	.Config = {
		.InterfaceNumber              = COMP_MOUSE_CDC_HID_INTERFACE_NO,

		.ReportINEndpointNumber       = COMP_MOUSE_CDC_MOUSE_EPNUM,
		.ReportINEndpointSize         = COMP_MOUSE_CDC_MOUSE_EPSIZE,
		.ReportINEndpointDoubleBank   = false,

		.PrevReportINBuffer           = comp_Mouse_CDC_PrevMouseHIDReportBuffer,
		.PrevReportINBufferSize       = sizeof(comp_Mouse_CDC_PrevMouseHIDReportBuffer),
		.PortNumber             = 0,
	},
};

USB_ClassInfo_CDC_Device_t comp_Mouse_CDC_VirtualSerial_CDC_Interface = {
	.Config = {
		.ControlInterfaceNumber         = COMP_MOUSE_CDC_CDC_FUNCTION_INTERFACE_NO,

		.DataINEndpointNumber           = COMP_MOUSE_CDC_CDC_TX_EPNUM,
		.DataINEndpointSize             = COMP_MOUSE_CDC_CDC_TXRX_EPSIZE,
		.DataINEndpointDoubleBank       = false,

		.DataOUTEndpointNumber          = COMP_MOUSE_CDC_CDC_RX_EPNUM,
		.DataOUTEndpointSize            = COMP_MOUSE_CDC_CDC_TXRX_EPSIZE,
		.DataOUTEndpointDoubleBank      = false,

		.NotificationEndpointNumber     = COMP_MOUSE_CDC_CDC_NOTIFICATION_EPNUM,
		.NotificationEndpointSize       = COMP_MOUSE_CDC_CDC_NOTIFICATION_EPSIZE,
		.NotificationEndpointDoubleBank = false,
		.PortNumber             = 0,
	},
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

//
// MyUSB Functions
//
#ifdef USB_COMP_MOUSE_CDC_CLASS

void comp_Mouse_CDC_HID_Mouse_Init(void)
{
	// For CDC
	CDC_IF_PTR = &comp_Mouse_CDC_VirtualSerial_CDC_Interface;
	RingBuffer_Init(&cdc_txring, cdc_txbuff, 1, CDC_UART_RB_SIZE);

	// For HID Mouse
	HID_Mouse_PTR = &comp_Mouse_CDC_Mouse_HID_Interface;
	MyBoard_Buttons_Init();
	MYBoard_Joystick_Init();

	// USB_Init one time is sufficient?
	USB_Init(HID_Mouse_PTR->Config.PortNumber, USB_MODE_Device);
}

//
// Common function to be implemented
//

void MyUSB_Init(void)
{
	// Init Descriptor
	DeviceDescriptor = DeviceDescriptor_comp_Mouse_CDC;
	ConfigurationDescriptor = ConfigurationDescriptor_comp_Mouse_CDC;
	LanguageStringPtr = (USB_Descriptor_String_t *) LanguageString_comp_Mouse_CDC;
	ManufacturerStringPtr = (USB_Descriptor_String_t *) ManufacturerString_comp_Mouse_CDC;
	ProductStringPtr = (USB_Descriptor_String_t *) ProductString_comp_Mouse_CDC;
	// Init others
	comp_Mouse_CDC_HID_Mouse_Init();
}

inline void USB_task_in_main_loop(void)
{
//	VirtualSerial_USB_USBTask();
	USB_USBTask(CDC_IF_PTR->Config.PortNumber, USB_MODE_Device);
	VirtualSerial_FinishDataTyHost();
	USB_USBTask(CDC_IF_PTR->Config.PortNumber, USB_MODE_Device); // Add another one in between

	HID_Device_USBTask(HID_Mouse_PTR);
	USB_USBTask(HID_Mouse_PTR->Config.PortNumber, USB_MODE_Device);

}
#endif // #ifdef USB_MOUSE_CLASS
