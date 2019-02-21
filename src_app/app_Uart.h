

#ifndef _TINYBLUERATUART_H_
#define _TINYBLUERATUART_H_

// For printf
#define FORMAT_DEC	10
#define FORMAT_HEX  16
// end

#ifdef  PRODUCTION

#define putchr(msg)
#define puthex(msg)				UARTputhex(msg)
#define putstr(msg)
#define printf(msg1,msg2)
#define printfs(msg)

#else
#define putchr(msg)				UARTputchar(msg)
#define puthex(msg)				UARTputhex(msg)
#define putstr(msg)				UARTputstr(msg)

#define printf(msg1,msg2)		UARTprintf(msg1,msg2)
#define printfs(msg)			UARTputstr((msg))
#endif
//#define uart_put_next_line(msg)	UART_put_next_line(msg)

//#define UART_CMD_LENGTH _UART_CMD_LENGTH_
#define URC_ADDRESS 0x58

#define _EOS_ '\0' // end of string

#define UART_MESSAGE_USE_VIRTUAL_SERIAL_PORT		// All UARTxxxx function will go through USB-CDC if USB CDC is enabled.

/////////////////////////////////////////////
// Uart command structure
/////////////////////////////////////////////

//#define  MAX_QUEUE_SIZE			12
#define  MAX_QUEUE_SIZE_ARM		128
#define  NORMAL_LENGTH			6
enum _CMDTYPE_
{
    GETSTATUS       = 0x01,
    CHANGEMODE      = 0x02,
    PRESSKEY        = 0x03,
    UNPRESSKEY      = 0x04,
    DIRECT_CODE     = 0x06,
    T_DEVIATION     = 0x84,
    CARRIER_DEV     = 0x83,
    RESET_RAM       = 0x66,
    WRITENVRAM      = 0xD0,
    READNVRAM       = 0xD1,
    MWRITE_NVRAM    = 0xD8,
    MREAD_NVRAM     = 0xD9,
    CODE_VERSION    = 0xF1,
	REG_READ		= 0xF5,
	REG_WRITE		= 0xF6,
	DIMMING_CTRL	= 0xB1,
	LIGHT_SENSOR	= 0xB2,
	LIGHT_SENSOR2	= 0xBB,
	LASER_CTRL		= 0xB3,
	ROTATION_CTRL	= 0xB4,
	ROTATION_READY	= 0xB5,
	PWM_WRITE		= 0xC1,
	PWM_READ		= 0xC2,
	RECEVING_DATA	= 0x10,
	TRANSMIT_DATA	= 0x11,
	REPEAT_DATA		= 0x12,
	BUFFER_DATA		= 0x13,
	FLUSH_BUFFER_DATA_OR_TRANSMIT_DATA = 0x14,
	SET_URD_ADDRESS = 0x15,
};

enum _CHAMGE_MODE
{
    IR_PC_SRIPT_MODE  = 0x00,
    IR_ENC_MODE       = 0x01,
    IR_DEC_MODE       = 0x02,
    IR_AUTO_MODE      = 0x04,
    IR_MARCO_MODE     = 0x08,
	IR_LEARNING_MODE  = 0x10,
	IR_SIMU_MODE  	  = 0x20,
	IR_SELFTEST_MODE  = 0x40,
    IR_NOTHING_MODE   = 0x80
    
};

enum _CMD_MODE
{
    IR_ZERO			  = 0x00,
    IR_ONE			  = 0x01,
    IR_FREQ		      = 0x02
};

typedef enum _UartCommadEnumType
{
    _UART_CMD_ADDRESS_	=0x00,	//58
    _UART_CMD1_,				//CMD
    _UART_CMD2_,				// Length

    _UART_CMD3_,
    _UART_CMD4_,
    _UART_CMD5_,
    _UART_CMD6_,
    _UART_CMD7_,
    _UART_CMD8_,
    _UART_CMD9_,
    _UART_CMD10_,

    _UART_CMD11_,
    _UART_CMD12_,
    _UART_CMD13_,
    _UART_CMD14_,
    _UART_CMD15_,
    _UART_CMD16_,
    _UART_CMD17_,
    _UART_CMD18_,

    _UART_CMD19_,
    _UART_CMD20_,
    _UART_CMD21_,
    _UART_CMD22_,
    _UART_CMD23_,
    _UART_CMD24_,
    _UART_CMD25_,
    _UART_CMD26_,
    _UART_CMD27_,
    
   UART_RXD_MAX_LENGTH
}UartCommadEnumType;

typedef struct _UartCommadType
{
//    BYTE Buffer[MAX_QUEUE_SIZE_ARM];       // buffer
//    BYTE In_Index;                          // in index
//    BYTE Out_Index;                         // out index

    BYTE CmdBuffer[UART_RXD_MAX_LENGTH];    // command buffer
    BYTE Cmd_Index;                         // command index
    BYTE Cmd_Length;                        // command length
}UartCommadType;

/////////////////////////////////////////////
// Uart Variables
/////////////////////////////////////////////

GLOBAL UartCommadType sURC; // Uart command struct
// Add for learning mode
GLOBAL uint16_t  IRRear, IRRear_previous;
GLOBAL uint16_t  IRChecksum;
#define MAX_IRXmt_Size	(4*3)
GLOBAL Byte idata IRXmt[MAX_IRXmt_Size];
GLOBAL Byte idata IRXmt_previous[MAX_IRXmt_Size];
GLOBAL uint16_t	Request_RC_RawData_Index, Current_RC_RawData_index;
GLOBAL uint8_t  Wait_for_URD_Data;
GLOBAL Byte CurrentBuffer[UART_RXD_MAX_LENGTH];
// END
GLOBAL Byte bModeLEDFlashTimes;

GLOBAL Byte bResetRxdCmdFlag;

/////////////////////////////////////////////
// Uart subroutines
/////////////////////////////////////////////
GLOBAL void UARTputchar(BYTE ucVal);
GLOBAL void UARTputhex(BYTE ucVal);
GLOBAL void UARTputHEX_U(BYTE ucVal);
GLOBAL void UARTputdec(uint32_t ucVal);
GLOBAL void UARTputstr(char *pFmt);
GLOBAL void UARTprintf(BYTE code *pFmt, uint32_t wVal); 
GLOBAL void UART_Show_CPU_Timer();
GLOBAL void UART_Command_Proc(void);
GLOBAL void UART_CheckSum(Byte bNums, Byte *Datapoint, Byte bLen );
GLOBAL void UART_CheckSum2(Byte bNums, Byte *Datapoint, Byte bLen );
GLOBAL void UART_CheckSum3(Byte bNums, Byte *Datapoint, Byte bLen );
GLOBAL void UART_CheckSum4(Byte bNums, Byte *Datapoint, Byte bLen );
GLOBAL void UART_put_next_line(void);

// RS232 debug message for USB
GLOBAL void USB_DEBUG_PutHEX_U(uint8_t hex_value);
GLOBAL void USB_DEBUG_MSG(uint8_t *sent_data);
// end


GLOBAL uint8_t TryPushIRData(void);
GLOBAL void PushIRData(Byte  bLevel,uint32_t  wTimeStamp);
GLOBAL void IRSendDataOut(void);

//
//
//
#include "usb_common.h"
#if defined (USB_CDC_CLASS) || defined(USB_COMP_MOUSE_CDC_CLASS)
#else
// The following define is used if Virtual Serial port is employed instead of real UART port
// This define is only allowed when USB CDC is supported
#undef UART_MESSAGE_USE_VIRTUAL_SERIAL_PORT
#endif // #if defined (USB_CDC_CLASS) || (USB_COMP_MOUSE_CDC_CLASS)
//
//
//

#ifdef UART_MESSAGE_USE_VIRTUAL_SERIAL_PORT

#include "VirtualSerial.h"
#include "comp_Mouse_CDC.h"

static inline int16_t READ_ONE_BYTE(uint8_t *from_host_data)
{
	uint16_t return_value;
	return_value = VirtualSerial_OneByteFromHost(from_host_data);
	return return_value;
}

static inline void WRITE_ONE_BYTE(uint8_t output_char)
{
	VirtualSerial_OneByteToHost(output_char);
}

static inline void WRITE_MULTI_BYTE(uint8_t *to_host_data, uint16_t bytes_to_write )
{
	VirtualSerial_MultiByteToHost(to_host_data, bytes_to_write);
}

static inline void FLUSH_BYTE(void)
{
	VirtualSerial_FinishDataTyHost();
}

#else

#include "Uart.h"

static inline uint16_t READ_ONE_BYTE(uint8_t *from_host_data)
{
	uint16_t return_value;
	return_value = ReadOneByteFromUARTRingBuffer(from_host_data);
	return return_value;
}

static inline void WRITE_ONE_BYTE(uint8_t output_char)
{
	WriteOneByteToUARTRingBuffer(output_char);
}

static inline void WRITE_MULTI_BYTE(uint8_t *to_host_data, uint16_t bytes_to_write )
{
	WriteMultiByteToUARTRingBuffer(to_host_data, bytes_to_write);
}

static inline void FLUSH_BYTE(void)
{
}

#endif // URAT_MESSAGE_USE_VIRTUAL_SERIAL_PORT


#endif
