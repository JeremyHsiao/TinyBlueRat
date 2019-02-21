/*
 * usb_common.h
 *
 *  Created on: 2013/8/16
 *      Author: jeremy.hsiao
 */

#ifndef USB_COMMON_H_
#define USB_COMMON_H_

#include "board.h"
#include "string.h"
#include "USB.h"
#include <stdbool.h>
#include "Descriptors_CDC.h"
#include "Descriptors_Mouse.h"
#include "Descriptors_comp_Mouse_CDC.h"
#include "VirtualSerial.h"
#include "Mouse.h"
#include "comp_Mouse_CDC.h"

#define USB_COMP_MOUSE_CDC_CLASS // USB_MOUSE_CLASS // USB_CDC_CLASS // USB_MOUSE_CLASS

#ifdef USB_MOUSE_CLASS
#define MYUSB_CONFIGURATION_TYPE	USB_Descriptor_Configuration_Mouse_t
#endif // USB_MOUSE_CLASS

#ifdef USB_COMP_MOUSE_CDC_CLASS
#define MYUSB_CONFIGURATION_TYPE	USB_Descriptor_Configuration_comp_Mouse_CDC_t
#endif // USB_COMP_MOUSE_CDC_CLASS

#ifdef USB_CDC_CLASS
#define MYUSB_CONFIGURATION_TYPE	USB_Descriptor_Configuration_CDC_t
#endif // USB_MOUSE_CLASS


#ifdef _USB_COMMON_C
		USB_Descriptor_Device_t 		DeviceDescriptor;
		MYUSB_CONFIGURATION_TYPE 		ConfigurationDescriptor;
		USB_Descriptor_String_t 		*LanguageStringPtr;
		USB_Descriptor_String_t 		*ManufacturerStringPtr;
		USB_Descriptor_String_t 		*ProductStringPtr;
#else
extern USB_Descriptor_Device_t 				DeviceDescriptor;
extern MYUSB_CONFIGURATION_TYPE 			ConfigurationDescriptor;
extern USB_Descriptor_String_t 				*LanguageStringPtr;
extern USB_Descriptor_String_t 				*ManufacturerStringPtr;
extern USB_Descriptor_String_t 				*ProductStringPtr;
#endif // _USB_COMMON_C

extern void MyUSB_Init(void);
extern void USB_task_in_main_loop(void);

#endif /* USB_COMMON_H_ */
