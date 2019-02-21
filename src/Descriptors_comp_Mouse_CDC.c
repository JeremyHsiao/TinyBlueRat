/*
 * @brief USB Device Descriptors, for library use when in USB device mode. Descriptors are special
 *        computer-readable structures which the host requests upon device enumeration, to determine
 *        the device's capabilities and functions
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

#include "Descriptors_comp_Mouse_CDC.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/** HID class report descriptor. This is a special descriptor constructed with values from the
 *  USBIF HID class specification to describe the reports and capabilities of the HID device. This
 *  descriptor is parsed by the host and its contents used to determine what data (and in what encoding)
 *  the device will send, and what it may be sent back from the host. Refer to the HID specification for
 *  more details on HID report descriptors.
 */
USB_Descriptor_HIDReport_Datatype_t MouseReport_comp_Mouse_CDC[] = {
		/* Use the HID class driver's standard Mouse report.
		 *   Min X/Y Axis values: -1
		 *   Max X/Y Axis values:  1
		 *   Min physical X/Y Axis values (used to determine resolution): -1
		 *   Max physical X/Y Axis values (used to determine resolution):  1
		 *   Buttons: 3
		 *   Absolute screen coordinates: false
		 */
		HID_DESCRIPTOR_MOUSE(-1, 1, -1, 1, 3, false)
};

uint32_t	GenericReport_comp_Mouse_CDCSize = sizeof(MouseReport_comp_Mouse_CDC);

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
USB_Descriptor_Device_t DeviceDescriptor_comp_Mouse_CDC = {
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(01.10),
	.Class                  = USB_CSCP_IADDeviceClass,
	.SubClass               = USB_CSCP_IADDeviceSubclass,
	.Protocol               = USB_CSCP_IADDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x1fc9,	/* NXP */
	.ProductID              = 0x2060,
	.ReleaseNumber          = VERSION_BCD(00.01),

	.ManufacturerStrIndex   = 0x01,
	.ProductStrIndex        = 0x02,
	.SerialNumStrIndex      = 0x03,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
USB_Descriptor_Configuration_comp_Mouse_CDC_t ConfigurationDescriptor_comp_Mouse_CDC = {
	.Config = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

		.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_comp_Mouse_CDC_t) - 1,		// termination byte not included in size
		.TotalInterfaces        = (1+2),

		.ConfigurationNumber    = 1,
		.ConfigurationStrIndex  = NO_DESCRIPTOR,

		.ConfigAttributes       = (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),

		.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
	},

	.HID_Interface = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

		.InterfaceNumber        = COMP_MOUSE_CDC_HID_INTERFACE_NO,
		.AlternateSetting       = 0x00,

		.TotalEndpoints         = 1,

		.Class                  = HID_CSCP_HIDClass,
		.SubClass               = HID_CSCP_BootSubclass,
		.Protocol               = HID_CSCP_MouseBootProtocol,

		.InterfaceStrIndex      = NO_DESCRIPTOR
	},

	.HID_MouseHID = {
		.Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},

		.HIDSpec                = VERSION_BCD(01.11),
		.CountryCode            = 0x00,
		.TotalReportDescriptors = 1,
		.HIDReportType          = HID_DTYPE_Report,
		.HIDReportLength        = sizeof(MouseReport_comp_Mouse_CDC)
	},

	.HID_ReportINEndpoint = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

		// Begin
		// .EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_IN | MOUSE_EPNUM),
		.EndpointAddress        = (ENDPOINT_DIR_IN | COMP_MOUSE_CDC_MOUSE_EPNUM),
		// End
		.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.EndpointSize           = COMP_MOUSE_CDC_MOUSE_EPSIZE,
		.PollingIntervalMS      = 0x01
	},

// Begin of HID Vendor Specific
//	.HID_Vendor_Interface = {
//		.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},
//
//		.InterfaceNumber        = 0x00,
//		.AlternateSetting       = 0x00,
//
//		.TotalEndpoints         = 1,
//
//		.Class                  = HID_CSCP_HIDClass,
//		.SubClass               = HID_CSCP_NonBootSubclass,
//		.Protocol               = HID_CSCP_NonBootProtocol,
//
//		.InterfaceStrIndex      = NO_DESCRIPTOR
//	},
//
//	.HID_GenericHID = {
//		.Header                 = {.Size = sizeof(USB_HID_Descriptor_HID_t), .Type = HID_DTYPE_HID},
//
//		.HIDSpec                = VERSION_BCD(01.11),
//		.CountryCode            = 0x00,
//		.TotalReportDescriptors = 1,
//		.HIDReportType          = HID_DTYPE_Report,
//		.HIDReportLength        = sizeof(GenericReport)
//	},
//
//	.HID_VendorReportINEndpoint = {
//		.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},
//
//		.EndpointAddress        = (ENDPOINT_DIR_IN | GENERIC_IN_EPNUM),
//		.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
//		.EndpointSize           = GENERIC_EPSIZE,
//		.PollingIntervalMS      = 0x01
//	},
// End of HID Vendor Specific

	.IAD_Interface = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Interface_Association_t), .Type = DTYPE_InterfaceAssociation},

		.FirstInterfaceIndex    = COMP_MOUSE_CDC_CDC_FUNCTION_INTERFACE_NO,
		.TotalInterfaces        = 0x02,
		.Class                  = CDC_CSCP_CDCClass,
		.SubClass               = CDC_CSCP_ACMSubclass,
		.Protocol               = CDC_CSCP_NoSpecificProtocol,
		.IADStrIndex      	    = NO_DESCRIPTOR
	},

	.CDC_CCI_Interface = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

		.InterfaceNumber        = COMP_MOUSE_CDC_CDC_FUNCTION_INTERFACE_NO,
		.AlternateSetting       = 0,

		.TotalEndpoints         = 1,

		.Class                  = CDC_CSCP_CDCClass,
		.SubClass               = CDC_CSCP_ACMSubclass,
		.Protocol               = CDC_CSCP_ATCommandProtocol,

		.InterfaceStrIndex      = NO_DESCRIPTOR
	},

	.CDC_Functional_Header = {
		.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalHeader_t), .Type = DTYPE_CSInterface},
		.Subtype                = CDC_DSUBTYPE_CSInterface_Header,

		.CDCSpecification       = VERSION_BCD(01.10),
	},

	.CDC_Functional_ACM = {
		.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalACM_t), .Type = DTYPE_CSInterface},
		.Subtype                = CDC_DSUBTYPE_CSInterface_ACM,

		.Capabilities           = 0x06,
	},

	.CDC_Functional_Union = {
		.Header                 = {.Size = sizeof(USB_CDC_Descriptor_FunctionalUnion_t), .Type = DTYPE_CSInterface},
		.Subtype                = CDC_DSUBTYPE_CSInterface_Union,

		.MasterInterfaceNumber  = COMP_MOUSE_CDC_CDC_FUNCTION_INTERFACE_NO,
		.SlaveInterfaceNumber   = COMP_MOUSE_CDC_CDC_DATA_INTERFACE_NO,
	},

	.CDC_NotificationEndpoint = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

		//			.EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_IN | CDC_NOTIFICATION_EPNUM),
		.EndpointAddress        = (ENDPOINT_DIR_IN | COMP_MOUSE_CDC_CDC_NOTIFICATION_EPNUM),
		.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.EndpointSize           = COMP_MOUSE_CDC_CDC_NOTIFICATION_EPSIZE,
		.PollingIntervalMS      = 0x10
	},

	.CDC_DCI_Interface = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

		.InterfaceNumber        = COMP_MOUSE_CDC_CDC_DATA_INTERFACE_NO,
		.AlternateSetting       = 0,

		.TotalEndpoints         = 2,

		.Class                  = CDC_CSCP_CDCDataClass,
		.SubClass               = CDC_CSCP_NoDataSubclass,
		.Protocol               = CDC_CSCP_NoDataProtocol,

		.InterfaceStrIndex      = NO_DESCRIPTOR
	},

	.CDC_DataOutEndpoint = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

		//			.EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_OUT | CDC_RX_EPNUM),
		.EndpointAddress        = (ENDPOINT_DIR_OUT | COMP_MOUSE_CDC_CDC_RX_EPNUM),
		.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.EndpointSize           = COMP_MOUSE_CDC_CDC_TXRX_EPSIZE,
		.PollingIntervalMS      = 0x01
	},

	.CDC_DataInEndpoint = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

		//			.EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_IN | CDC_TX_EPNUM),
		.EndpointAddress        = (ENDPOINT_DIR_IN | COMP_MOUSE_CDC_CDC_TX_EPNUM),
		.Attributes             = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.EndpointSize           = COMP_MOUSE_CDC_CDC_TXRX_EPSIZE,
		.PollingIntervalMS      = 0x01
	},

	.HID_Termination = 0x00
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
uint8_t LanguageString_comp_Mouse_CDC[] = {
	USB_STRING_LEN(1),
	DTYPE_String,
	WBVAL(LANGUAGE_ID_ENG),
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
uint8_t ManufacturerString_comp_Mouse_CDC[] = {
	USB_STRING_LEN(3),
	DTYPE_String,
	WBVAL('N'),
	WBVAL('X'),
	WBVAL('P'),
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
uint8_t ProductString_comp_Mouse_CDC[] = {
	USB_STRING_LEN(20),
	DTYPE_String,
	WBVAL('T'),
	WBVAL('P'),
	WBVAL('V'),
	WBVAL('A'),
	WBVAL('O'),
	WBVAL('C'),
	WBVAL(' '),
	WBVAL('M'),
	WBVAL('y'),
	WBVAL('O'),
	WBVAL('w'),
	WBVAL('n'),
	WBVAL('U'),
	WBVAL('S'),
	WBVAL('B'),
	WBVAL(' '),
	WBVAL('D'),
	WBVAL('e'),
	WBVAL('m'),
	WBVAL('o'),
};

//
// Serial Number String
//
uint8_t SerialNumberString_comp_Mouse_CDC[] = {
	USB_STRING_LEN(20),
	DTYPE_String,
	WBVAL('S'),
	WBVAL('N'),
	WBVAL('1'),
	WBVAL('2'),
	WBVAL('3'),
	WBVAL('4'),
	WBVAL('5'),
	WBVAL('6'),
	WBVAL('7'),
	WBVAL('8'),
	WBVAL('9'),
	WBVAL('0'),
	WBVAL('A'),
	WBVAL('B'),
	WBVAL('C'),
	WBVAL('E'),
	WBVAL('F'),
	WBVAL('Z'),
	WBVAL('Y'),
	WBVAL('X'),
};

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

