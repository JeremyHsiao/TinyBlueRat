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

#include "Descriptors_Mouse.h"

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
USB_Descriptor_HIDReport_Datatype_t MouseReport[] = {
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

uint32_t	MouseReportSize = sizeof(MouseReport);

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
USB_Descriptor_Device_t DeviceDescriptor_Mouse = {
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(01.10),
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x1fc9,	/* NXP */
	.ProductID              = 0x2041,
	.ReleaseNumber          = VERSION_BCD(00.01),

	.ManufacturerStrIndex   = 0x01,
	.ProductStrIndex        = 0x02,
	.SerialNumStrIndex      = NO_DESCRIPTOR,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
USB_Descriptor_Configuration_Mouse_t ConfigurationDescriptor_Mouse = {
	.Config = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

		.TotalConfigurationSize = sizeof(ConfigurationDescriptor_Mouse) - 1,		// termination byte not included in size
		.TotalInterfaces        = 1,

		.ConfigurationNumber    = 1,
		.ConfigurationStrIndex  = NO_DESCRIPTOR,

		.ConfigAttributes       = (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),

		.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
	},

	.HID_Interface = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

		.InterfaceNumber        = 0x00,
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
		.HIDReportLength        = sizeof(MouseReport)
	},

	.HID_ReportINEndpoint = {
		.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

		// Begin
		// .EndpointAddress        = (ENDPOINT_DESCRIPTOR_DIR_IN | MOUSE_EPNUM),
		.EndpointAddress        = (ENDPOINT_DIR_IN | MOUSE_EPNUM),
		// End
		.Attributes             = (EP_TYPE_INTERRUPT | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
		.EndpointSize           = MOUSE_EPSIZE,
		.PollingIntervalMS      = 0x01
	},
	.HID_Termination = 0x00
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
uint8_t LanguageString_Mouse[] = {
	USB_STRING_LEN(1),
	DTYPE_String,
	WBVAL(LANGUAGE_ID_ENG),
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
uint8_t ManufacturerString_Mouse[] = {
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
uint8_t ProductString_Mouse[] = {
	USB_STRING_LEN(20),
	DTYPE_String,
	WBVAL('L'),
	WBVAL('P'),
	WBVAL('C'),
	WBVAL('U'),
	WBVAL('S'),
	WBVAL('B'),
	WBVAL('l'),
	WBVAL('i'),
	WBVAL('b'),
	WBVAL(' '),
	WBVAL('M'),
	WBVAL('o'),
	WBVAL('u'),
	WBVAL('s'),
	WBVAL('e'),
	WBVAL(' '),
	WBVAL('D'),
	WBVAL('e'),
	WBVAL('m'),
	WBVAL('o'),
};

/*****************************************************************************
 * Private functions
 ****************************************************************************/

