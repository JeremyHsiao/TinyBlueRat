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
#include "Mouse.h"
#include "usb_common.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/


/** Buffer to hold the previously generated Mouse HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevMouseHIDReportBuffer[sizeof(USB_MouseReport_Data_t)];

/** nxpUSBlib HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Mouse_HID_Interface = {
	.Config = {
		.InterfaceNumber              = 0,

		.ReportINEndpointNumber       = MOUSE_EPNUM,
		.ReportINEndpointSize         = MOUSE_EPSIZE,
		.ReportINEndpointDoubleBank   = false,

		.PrevReportINBuffer           = PrevMouseHIDReportBuffer,
		.PrevReportINBufferSize       = sizeof(PrevMouseHIDReportBuffer),
		.PortNumber             = 0,
	},
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
#if defined(USB_MOUSE_CLASS) || defined (USB_COMP_MOUSE_CDC_CLASS)
USB_ClassInfo_HID_Device_t     	*HID_Mouse_PTR;
#endif // #if defined(USB_CDC_CLASS) || define (USB_COMP_MOUSE_CDC_CLASS)

/*****************************************************************************
 * Private functions
 ****************************************************************************/

void MyBoard_Buttons_Init(void)
{
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, MY_BUTTONS_BUTTON1_GPIO_PORT_NUM, MY_BUTTONS_BUTTON1_GPIO_BIT_NUM, false);
}

uint32_t MyButtons_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;
	if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, MY_BUTTONS_BUTTON1_GPIO_PORT_NUM, MY_BUTTONS_BUTTON1_GPIO_BIT_NUM) == 0x00) {
		ret |= BUTTONS_BUTTON1;
	}
	return ret;
}

void MYBoard_Joystick_Init(void)
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, MY_JOYSTICK_UP_GPIO_PORT_NUM, MY_JOYSTICK_UP_GPIO_BIT_NUM, (IOCON_FUNC0 | IOCON_MODE_PULLUP | PIO0_ALL_DIGITAL_MODE));
	Chip_IOCON_PinMuxSet(LPC_IOCON, MY_JOYSTICK_DOWN_GPIO_PORT_NUM, MY_JOYSTICK_DOWN_GPIO_BIT_NUM, (IOCON_FUNC0 | IOCON_MODE_PULLUP | PIO1_ALL_RESERVED));
	Chip_IOCON_PinMuxSet(LPC_IOCON, MY_JOYSTICK_LEFT_GPIO_PORT_NUM, MY_JOYSTICK_LEFT_GPIO_BIT_NUM, (IOCON_FUNC0 | IOCON_MODE_PULLUP | PIO1_ALL_RESERVED));
	Chip_IOCON_PinMuxSet(LPC_IOCON, MY_JOYSTICK_RIGHT_GPIO_PORT_NUM, MY_JOYSTICK_RIGHT_GPIO_BIT_NUM, (IOCON_FUNC0 | IOCON_MODE_PULLUP | PIO1_ALL_RESERVED));
	Chip_IOCON_PinMuxSet(LPC_IOCON, MY_JOYSTICK_PRESS_GPIO_PORT_NUM, MY_JOYSTICK_PRESS_GPIO_BIT_NUM, (IOCON_FUNC0 | IOCON_MODE_PULLUP | PIO1_ALL_RESERVED));
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, MY_JOYSTICK_UP_GPIO_PORT_NUM, MY_JOYSTICK_UP_GPIO_BIT_NUM, false);		// input
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, MY_JOYSTICK_DOWN_GPIO_PORT_NUM, MY_JOYSTICK_DOWN_GPIO_BIT_NUM, false);	// input
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, MY_JOYSTICK_LEFT_GPIO_PORT_NUM, MY_JOYSTICK_LEFT_GPIO_BIT_NUM, false);	// input
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, MY_JOYSTICK_RIGHT_GPIO_PORT_NUM, MY_JOYSTICK_RIGHT_GPIO_BIT_NUM, false);	// input
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, MY_JOYSTICK_PRESS_GPIO_PORT_NUM, MY_JOYSTICK_PRESS_GPIO_BIT_NUM, false);	// input
}

uint8_t MyJoystick_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;
	if ((Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, MY_JOYSTICK_UP_GPIO_PORT_NUM, MY_JOYSTICK_UP_GPIO_BIT_NUM)) == 0x00) {
		ret |= JOY_UP;
	}
	else if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, MY_JOYSTICK_DOWN_GPIO_PORT_NUM, MY_JOYSTICK_DOWN_GPIO_BIT_NUM) == 0x00) {
		ret |= JOY_DOWN;
	}
	else if ((Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, MY_JOYSTICK_LEFT_GPIO_PORT_NUM, MY_JOYSTICK_LEFT_GPIO_BIT_NUM)) == 0x00) {
		ret |= JOY_LEFT;
	}
	else if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, MY_JOYSTICK_RIGHT_GPIO_PORT_NUM, MY_JOYSTICK_RIGHT_GPIO_BIT_NUM) == 0x00) {
		ret |= JOY_RIGHT;
	}
	else if ((Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, MY_JOYSTICK_PRESS_GPIO_PORT_NUM, MY_JOYSTICK_PRESS_GPIO_BIT_NUM)) == 0x00) {
		ret |= JOY_PRESS;
	}
	return ret;
}

//#define USE_MY_INPUT_SYSTEM
#ifdef USE_MY_INPUT_SYSTEM
#include "input.h"
/* HID class driver callback function for the creation of HID reports to the host */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
										 uint8_t *const ReportID,
										 const uint8_t ReportType,
										 void *ReportData,
										 uint16_t *const ReportSize)
{
	USB_MouseReport_Data_t *MouseReport = (USB_MouseReport_Data_t *) ReportData;
	bool press = false;
	int8_t	temp_x, temp_y;

	*ReportSize = sizeof(USB_MouseReport_Data_t);

	// 8-bit
	temp_x = My_input_event_value(REL_X);
	if(temp_x>=127)
	{
		temp_x = 127;
	}
	else if(temp_x<=-127)
	{
		temp_x = -127;
	}

	// 8-bit
	temp_y = My_input_event_value(REL_Y);
	if(temp_y>=127)
	{
		temp_y = 127;
	}
	else if(temp_y<=-127)
	{
		temp_y = -127;
	}

	MouseReport->X = temp_x;
	MouseReport->Y = temp_y;

	if(My_input_event_value(BTN_LEFT))
	{
		MouseReport->Button |= (1 << 0);
		press = true;
	}

	if(My_input_event_value(BTN_RIGHT))
	{
		MouseReport->Button |= (1 << 1);
		press = true;
	}

	return press;
}

#else

/* HID class driver callback function for the creation of HID reports to the host */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
										 uint8_t *const ReportID,
										 const uint8_t ReportType,
										 void *ReportData,
										 uint16_t *const ReportSize)
{
	USB_MouseReport_Data_t *MouseReport = (USB_MouseReport_Data_t *) ReportData;

	uint8_t JoyStatus_LCL    = MyJoystick_GetStatus();
	uint8_t ButtonStatus_LCL = MyButtons_GetStatus();
	bool press = false;

	if (JoyStatus_LCL & JOY_UP) {
		MouseReport->Y = -1;
		press = true;
	}
	else if (JoyStatus_LCL & JOY_DOWN) {
		MouseReport->Y =  1;
		press = true;
	}

	if (JoyStatus_LCL & JOY_LEFT) {
		MouseReport->X = -1;
		press = true;
	}
	else if (JoyStatus_LCL & JOY_RIGHT) {
		MouseReport->X =  1;
		press = true;
	}

	if (JoyStatus_LCL & JOY_PRESS) {
		MouseReport->Button |= (1 << 0);
		press = true;
	}

	if (ButtonStatus_LCL & BUTTONS_BUTTON1) {
		MouseReport->Button |= (1 << 1);
		press = true;
	}

	*ReportSize = sizeof(USB_MouseReport_Data_t);
	return press;
}

#endif // #ifdef USE_MY_INPUT_SYSTEM


/* HID class driver callback function for the processing of HID reports from the host */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
										  const uint8_t ReportID,
										  const uint8_t ReportType,
										  const void *ReportData,
										  const uint16_t ReportSize)
{
	/* Unused (but mandatory for the HID class driver) in this demo, since there are no Host->Device reports */
}

#ifdef USB_MOUSE_CLASS
//
// Common function to be implemented
//

void HID_Mouse_Init(void)
{
	HID_Mouse_PTR = &Mouse_HID_Interface;

	MyBoard_Buttons_Init();
	MYBoard_Joystick_Init();
	USB_Init(HID_Mouse_PTR->Config.PortNumber, USB_MODE_Device);
}

// HID Mouse
void MyUSB_Init(void)
{
	// Init Descriptor
	DeviceDescriptor = DeviceDescriptor_Mouse;
	ConfigurationDescriptor = ConfigurationDescriptor_Mouse;
	LanguageStringPtr = (USB_Descriptor_String_t *) LanguageString_Mouse;
	ManufacturerStringPtr = (USB_Descriptor_String_t *) ManufacturerString_Mouse;
	ProductStringPtr = (USB_Descriptor_String_t *) ProductString_Mouse;
	// Init others
	//VirtualSerial_Init();
	HID_Mouse_Init();
}

inline void USB_task_in_main_loop(void)
{
//	VirtualSerial_USB_USBTask();
//	VirtualSerial_FinishDataTyHost();

	HID_Device_USBTask(HID_Mouse_PTR);
	USB_USBTask(HID_Mouse_PTR->Config.PortNumber, USB_MODE_Device);

}
// End of HID Mouse
#endif // #ifdef USB_MOUSE_CLASS
