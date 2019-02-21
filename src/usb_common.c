/*
 * usb_common.c
 *
 *  Created on: 2013/8/16
 *      Author: jeremy.hsiao
 */

#include "chip.h"
#include "board.h"
#include "string.h"
#include "USB.h"
#include <stdbool.h>
#include "Uart.h"

#include "VirtualSerial.h"
#include "Mouse.h"

#define _USB_COMMON_C
#include "usb_common.h"
#undef _USB_COMMON_C

// Common
#include "Define.h"
// Internal
#define  GLOBAL
#undef GLOBAL
// External
#define  GLOBAL extern
#include "app_Uart.h"
#undef GLOBAL

//
// Common Functions
//

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(uint8_t corenum,
									const uint16_t wValue,
									const uint8_t wIndex,
									const void * *const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void *Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	switch (DescriptorType) {
	case DTYPE_Device:
		Address = &DeviceDescriptor;
		Size    = sizeof(USB_Descriptor_Device_t);
		USB_DEBUG_MSG("Device\r\n");
		break;

	case DTYPE_Configuration:
		Address = &ConfigurationDescriptor;
		Size    = sizeof(MYUSB_CONFIGURATION_TYPE);
		USB_DEBUG_MSG("Configuration\r\n");
		break;

	case DTYPE_String:
		switch (DescriptorNumber) {
		case 0x00:
			Address = LanguageStringPtr;
			Size    = pgm_read_byte(&LanguageStringPtr->Header.Size);
			USB_DEBUG_MSG("DescriptorNumber\r\n");
			break;

		case 0x01:
			Address = ManufacturerStringPtr;
			Size    = pgm_read_byte(&ManufacturerStringPtr->Header.Size);
			USB_DEBUG_MSG("ManufacturerString\r\n");
			break;

		case 0x02:
			Address = ProductStringPtr;
			Size    = pgm_read_byte(&ProductStringPtr->Header.Size);
			USB_DEBUG_MSG("ProductString\r\n");
			break;
		}

		break;

#if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

	case HID_DTYPE_HID:
		Address = &ConfigurationDescriptor.HID_MouseHID;
		Size    = sizeof(USB_HID_Descriptor_HID_t);
		USB_DEBUG_MSG("HID_DTYPE\r\n");
		break;

	case HID_DTYPE_Report:
		Address = &MouseReport;
		Size    = MouseReportSize;
		USB_DEBUG_MSG("MouseReport\r\n");
		break;
#endif // #if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

	}

	*DescriptorAddress = Address;
	return Size;
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{USB_DEBUG_MSG("EVENT_USB_Device_Connect\r\n");}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{USB_DEBUG_MSG("EVENT_USB_Device_Disconnect\r\n");}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

#if defined(USB_CDC_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)
	ConfigSuccess &= CDC_Device_ConfigureEndpoints(CDC_IF_PTR);
#endif // #if defined(USB_CDC_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

#if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)
	ConfigSuccess &= HID_Device_ConfigureEndpoints(HID_Mouse_PTR);
	USB_Device_EnableSOFEvents(); // USB Mouse Only??
#endif // #if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

	USB_DEBUG_MSG("EVENT_USB_Device_ConfigurationChanged\r\n");
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
#if defined(USB_CDC_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)
	CDC_Device_ProcessControlRequest(CDC_IF_PTR);
#endif // #if defined(USB_CDC_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

#if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)
	HID_Device_ProcessControlRequest(HID_Mouse_PTR);
	USB_DEBUG_MSG("EVENT_USB_Device_ControlRequest\r\n");
#endif // #if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

}

#if defined(USB_CDC_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)
#if !defined(USB_DEVICE_ROM_DRIVER)
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo)
{
	/*TODO: add LineEncoding processing here
	 * this is just a simple statement, only Baud rate is set */
	// Serial_Init(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS, false);
	USB_DEBUG_MSG("EVENT_CDC_Device_LineEncodingChanged\r\n");
}

#else
void EVENT_UsbdCdc_SetLineCode(CDC_LINE_CODING *line_coding)
{
	// Serial_Init(VirtualSerial_CDC_Interface.State.LineEncoding.BaudRateBPS, false);
}

#endif
#endif // #if defined(USB_CDC_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

#if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)

/**
 * @brief Event handler for the USB device Start Of Frame event
 * @return Nothing
 */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(HID_Mouse_PTR);
}

#endif // #if defined(USB_MOUSE_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)
