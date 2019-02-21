/*
 * @brief Mouse device class declarations, definitions for using in application
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

#ifndef __DESCRIPTORS_COMP_MOUSE_CDC_H_
#define __DESCRIPTORS_COMP_MOUSE_CDC_H_

#include "USB.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup Mouse_Device_Descriptor Class descriptors
 * @ingroup USB_Mouse_Device_18xx43xx USB_Mouse_Device_17xx40xx USB_Mouse_Device_11Uxx
 * @{
 */

/** @brief	Type define for the device configuration descriptor structure. This must be defined in the
 *          application code, as the configuration descriptor contains several sub-descriptors which
 *          vary between devices, and which describe the device's usage to the host.
 */
typedef struct {
	USB_Descriptor_Configuration_Header_t Config;

	// HID Mouse
	USB_Descriptor_Interface_t            HID_Interface;
	USB_HID_Descriptor_HID_t              HID_MouseHID;
	USB_Descriptor_Endpoint_t             HID_ReportINEndpoint;

	// Reserved for generic HID use
//	USB_Descriptor_Interface_t            HID_Vendor_Interface;
//	USB_HID_Descriptor_HID_t              HID_GenericHID;
//	USB_Descriptor_Endpoint_t             HID_VendorReportINEndpoint;

	// IAD for CDC in composite device
	USB_Descriptor_Interface_Association_t	 IAD_Interface;

	USB_Descriptor_Interface_t               CDC_CCI_Interface;
	USB_CDC_Descriptor_FunctionalHeader_t    CDC_Functional_Header;
	USB_CDC_Descriptor_FunctionalACM_t       CDC_Functional_ACM;
	USB_CDC_Descriptor_FunctionalUnion_t     CDC_Functional_Union;
	USB_Descriptor_Endpoint_t                CDC_NotificationEndpoint;

	USB_Descriptor_Interface_t               CDC_DCI_Interface;
	USB_Descriptor_Endpoint_t                CDC_DataOutEndpoint;
	USB_Descriptor_Endpoint_t                CDC_DataInEndpoint;

	unsigned char                            HID_Termination;
} USB_Descriptor_Configuration_comp_Mouse_CDC_t;

/** Endpoint number of the Mouse HID reporting IN endpoint. */
#define COMP_MOUSE_CDC_MOUSE_EPNUM            (1)		// EP1

/** Size in bytes of the Mouse HID reporting IN endpoint. */
#define COMP_MOUSE_CDC_MOUSE_EPSIZE           (8)

//
// For Vendor HID
//
///** Endpoint number of the Generic HID reporting IN endpoint. */
//#define GENERIC_IN_EPNUM          (0) // (MOUSE_EPNUM+1)
//
///** Size in bytes of the Generic HID reporting endpoint. */
//#define GENERIC_EPSIZE            64
//
///** Size in bytes of the Generic HID reports (including report ID byte). */
//#define GENERIC_REPORT_SIZE       24
//
#define COMP_MOUSE_CDC_HID_INTERFACE_NO          (0)		// IF0

//
// CDC
//

/** Endpoint number of the CDC device-to-host notification IN endpoint. */
#define COMP_MOUSE_CDC_CDC_NOTIFICATION_EPNUM         (2)		// EP2

/** Endpoint number of the CDC device-to-host data IN endpoint. */
#define COMP_MOUSE_CDC_CDC_TX_EPNUM                   (COMP_MOUSE_CDC_CDC_NOTIFICATION_EPNUM+1)		// EP3

/** Endpoint number of the CDC host-to-device data OUT endpoint. */
#define COMP_MOUSE_CDC_CDC_RX_EPNUM               	   (COMP_MOUSE_CDC_CDC_TX_EPNUM+1)				// EP4

/** Size in bytes of the CDC device-to-host notification IN endpoint. */
#define COMP_MOUSE_CDC_CDC_NOTIFICATION_EPSIZE        (8)

/** Size in bytes of the CDC data IN and OUT endpoints. */
#define COMP_MOUSE_CDC_CDC_TXRX_EPSIZE                (64) //16


#define COMP_MOUSE_CDC_CDC_FUNCTION_INTERFACE_NO          	(1)												// IF1
#define COMP_MOUSE_CDC_CDC_DATA_INTERFACE_NO          		(COMP_MOUSE_CDC_CDC_FUNCTION_INTERFACE_NO+1)	// IF2

extern USB_Descriptor_Device_t DeviceDescriptor_comp_Mouse_CDC;
extern USB_Descriptor_Configuration_comp_Mouse_CDC_t ConfigurationDescriptor_comp_Mouse_CDC;
extern uint8_t LanguageString_comp_Mouse_CDC[];
extern uint8_t ManufacturerString_comp_Mouse_CDC[];
extern uint8_t ProductString_comp_Mouse_CDC[];
extern uint8_t SerialNumberString_comp_Mouse_CDC[];

extern USB_Descriptor_HIDReport_Datatype_t MouseReport_comp_Mouse_CDC[];
extern uint32_t		GenericReport_comp_Mouse_CDCSize;


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __DESCRIPTORS_COMP_MOUSE_CDC_H_ */
