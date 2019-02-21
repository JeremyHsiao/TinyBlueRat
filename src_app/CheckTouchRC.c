
/*
===============================================================================
 Name        : CheckTouchRC.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/
#include "board.h"
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "Timer.h"
#include "Uart.h"
#include "usb_common.h"

// Common
#include "Define.h"
#include "string.h"
// Internal
#define  GLOBAL
#include "CheckTouchRC.h"
#undef GLOBAL

// External
#define  GLOBAL extern
#include "app_Uart.h"
#include "Common.h"
#undef GLOBAL

#include "input.h"
#include "automata_common.h"

//
// Common IR variables
//
static U32 				_u32_1stDelayTimeMs;
static U32  			_u32_2ndDelayTimeMs;
static IRKeyProperty 	_ePrevKeyProperty;
static U8   			_u8PrevKeyCode;
static U8 				_u8PrevSystemCode;
static U8 				FactoryKeyEnable=false;
static uint32_t 		ulPreTime=0;
static uint32_t			_ulLastKeyPresentTime=0;
static MS_IR_KeyInfo 	_KeyReceived;   //temporary solution
//
//
//

//
// BEGIN NEC-only IR variables
//
#if (IR_TYPE_SEL == IR_TYPE_AOC)
static uint32_t _u32IRCount = 0;
static uint8_t  _u8IRHeadReceived=0;
static uint8_t  _u8IRRepeateDetect=0;
static uint8_t  _u8IRRepeated=0;
static uint8_t  _u8IRRepeatedNum=0;
static uint8_t  _u8IRType=0;
uint32_t	_u32IRData[IR_SWDECODE_MODE_BUF_LEN];
#define TPVFACKeyHeader {0x20,0x08,0x01}
#define PANDAKeyHeader {0x08,0xF7,0x00}
#define AOCKeyHeader {0x00,0xbd,0x00}
static struct TPV_HeadCode TPV_HeadCode_Group[]= {TPVFACKeyHeader,PANDAKeyHeader,AOCKeyHeader}; // Jeremy -
#endif // #if (IR_TYPE_SEL == IR_TYPE_AOC)
//
// END NEC
//

// Added for RC6-only IR variables
#if (IR_TYPE_SEL == IR_TYPE_EBONY)
static BOOL bRC56HasKey = FALSE;
static U32 _u32IRData = 0;
static U32 _u32IRPreData = 0;
static U8 _u8IRBit = 0;
static IrSourceType _irSourceType = IR_SOURCE_NULL;
static IrParityType _irParity = IR_PARITY_N;
static IrFrameHalfIdx _irFrameHalf = IR_1ST_FRAME_HALF;
static IRRC6State _irState = IR_RC6_IDLE;
static IRRC6DataState _irDataState = IR_RC6_DATA_IDLE;
static IRRC6DataTrailState _irDataTrailState = IR_RC6_DATA_TRAIL_IDLE;
static unsigned long   _ulPrevKeyTime = 0;
static U32 _ulSignalTime = 0;

//RCMM
#define MAX_RCBYTE_LEN  4
static BOOL StartDecodeFlag = FALSE;
static U8 RCBitsCnt;
static U32 tgbits = 0;
static U8 RCByte[MAX_RCBYTE_LEN];
static U8 _u8IrPreRcmmData[MAX_RCBYTE_LEN];
static U32 u16CustomerID;
static BOOL UpDataFlage = FALSE;
static U8 RCMode;
//RCMM

#endif // #if (IR_TYPE_SEL == IR_TYPE_EBONY)
//

inline uint32_t _MDrv_IR_GetSystemTime(void)
{
	return Chip_TIMER_ReadCount(LPC_TIMER32_0);
}

U8 calc_crc4( U32 nData )
{
	U32 mask = 0x00800000;
	U32 x4_x_1 = 0x00980000;
	U32 nValidBit = 20;

	while( nValidBit-- != 0 )
	{
		if( (nData & mask) != 0 )
		{
			nData = nData ^ x4_x_1;
		}
		x4_x_1 >>= 1;
		mask >>= 1;
	}
	return (U8)(nData & 0x0f);
}

#define DEBUG_IR(x)
// Define IrDa registers - a potential bug
#define REG_IR_BASE             0xBF807B00
#define REG_IR_OFC_UPB          (0x0003*4 + (REG_IR_BASE))

int Convert_uint_to_int(unsigned int uint_val)
{
	int ret_val;

	// uint_val is limited to 10 bits long
	if ((uint_val&0x200)!=0)
	{
		ret_val = (uint_val&0x3ff) - 0x400;
	}
	else
	{
		ret_val = uint_val & 0x1ff;
	}
	return ret_val;
}

#if (IR_TYPE_SEL == IR_TYPE_EBONY)
static U8 _MDrv_IR_RC5RC6ParseKey(U8 u8Control, U8 u8Inform)
{
	U8 u8RetVal = IRKEY_DUMY;
	//printk("u8Control %x\n", u8Control);
	//printk("Input %x\n", u8Inform);

	if(u8Control == 0x00)
	{
		u8RetVal = u8Inform;
	}
	else
	{
		//  Below are for other controller of other control mode
		if((131==u8Control) && (FactoryKeyEnable == false))
		{
			switch(u8Inform)
			{
			case 0x6D :
				u8RetVal = F_IRKEY_TEST;
				FactoryKeyEnable = true;
				break;

			default                    : u8RetVal = IRKEY_DUMY;                 break;
			}
		}
		else if((131==u8Control) && (FactoryKeyEnable == true))
		{
			switch(u8Inform)
			{
			case 0x6D :
				u8RetVal = F_IRKEY_TEST;
				FactoryKeyEnable = false;
				break;

			case 0x3E                  : u8RetVal = F_IRKEY_FAC;                break;
			default                    : u8RetVal = IRKEY_DUMY;                  break;
			}
		}
		else if((133==u8Control) && (FactoryKeyEnable == true))
		{
			switch(u8Inform)
			{
			case 0x7B                  : u8RetVal = F_IRKEY_VGA;                break;
			default                    : u8RetVal = IRKEY_DUMY;                  break;
			}
		}
		else if((137==u8Control) && (FactoryKeyEnable == true))
		{
			switch(u8Inform)
			{
			case 0x5A                  : u8RetVal = F_IRKEY_RST;                break;
			case 0x50                  : u8RetVal = F_IRKEY_CSM;                break;
			case 0x62                  : u8RetVal = F_IRKEY_PATTERN;            break;
			case 0x0F                  : u8RetVal = F_IRKEY_1;                  break;
			case 0x12                  : u8RetVal = F_IRKEY_2;                  break;
			case 0x4A                  : u8RetVal = F_IRKEY_3;                  break;
			case 0x30                  : u8RetVal = F_IRKEY_4;                  break;
			case 0x10                  : u8RetVal = F_IRKEY_5;                  break;
			case 0x11                  : u8RetVal = F_IRKEY_6;                  break;
			case 0x32                  : u8RetVal = F_IRKEY_7;                  break;
			case 0x35                  : u8RetVal = F_IRKEY_8;                  break;
			case 0x36                  : u8RetVal = F_IRKEY_9;                  break;
			case 0x33                  : u8RetVal = F_IRKEY_0;                  break;
			case 0x3F                  : u8RetVal = F_IRKEY_ANTENNA_CABLE;      break;
			case 0x6B                  : u8RetVal = F_IRKEY_PRE_CH;             break;
			case 0x5F                  : u8RetVal = F_IRKEY_VOL_MAX;            break;
			case 0x60                  : u8RetVal = F_IRKEY_VOL_BUZZ;           break;
			case 0x03                  : u8RetVal = F_IRKEY_CTC;                break;
			case 0x21                  : u8RetVal = F_IRKEY_MENU;               break;
			case 0x56                  : u8RetVal = F_IRKEY_CH_PLUS;            break;
			case 0x55                  : u8RetVal = F_IRKEY_CH_MINUS;           break;
			case 0x54                  : u8RetVal = F_IRKEY_B_I;                break;
			case 0x57                  : u8RetVal = F_IRKEY_CH_SCAN;            break;
			case 0x26                  : u8RetVal = F_IRKEY_UP;                 break;
			case 0x27                  : u8RetVal = F_IRKEY_DOWN;               break;
			case 0x0E                  : u8RetVal = F_IRKEY_ENTER;              break;
			case 0x5D                  : u8RetVal = F_IRKEY_VOL_L;              break;
			case 0x5E                  : u8RetVal = F_IRKEY_VOL_R;              break;
			case 0x4B                  : u8RetVal = F_IRKEY_CC_TT;              break;
			case 0x37                  : u8RetVal = F_IRKEY_MUTE;               break;
			case 0x46                  : u8RetVal = F_IRKEY_PIC;                break;
			case 0x53                  : u8RetVal = F_IRKEY_AUDIO;              break;
			case 0x75                  : u8RetVal = F_IRKEY_LOG_LED;            break;
			case 0x22                  : u8RetVal = F_IRKEY_2D_3D;              break;
			case 0x23                  : u8RetVal = F_IRKEY_ARC;                break;
			case 0x24                  : u8RetVal = F_IRKEY_CI_PLUS;            break;
			case 0x51                  : u8RetVal = F_IRKEY_VIRGIN;             break;
			case 0x00                  : u8RetVal = F_IRKEY_CVBS;               break;
			case 0x01                  : u8RetVal = F_IRKEY_YPBPR_SCART;        break;
			case 0x02                  : u8RetVal = F_IRKEY_HDMI;               break;
			case 0x74                  : u8RetVal = F_IRKEY_REGIN;              break;
			case 0x72                  : u8RetVal = F_IRKEY_CLONE;              break;
			case 0x73                  : u8RetVal = F_IRKEY_CLK;                break;
			case 0x34                  : u8RetVal = F_IRKEY_DCR;                break;
			case 0x31                  : u8RetVal = F_IRKEY_ADC;                break;
			case 0x0D                  : u8RetVal = F_IRKEY_BLK;                break;
			case 0x69                  : u8RetVal = F_IRKEY_W_P;                break;
			case 0x25                  : u8RetVal = F_IRKEY_LIGHT_SENSOR;       break;
			case 0x70                  : u8RetVal = F_IRKEY_USB;                break;
			case 0x71                  : u8RetVal = F_IRKEY_RJ45;               break;
			case 0x28                  : u8RetVal = F_IRKEY_RS232;              break;
			case 0x29                  : u8RetVal = F_IRKEY_EMPTY;              break;
			default                    : u8RetVal = IRKEY_DUMY;                  break;
			}
		}
	}

	//printk("Exit %x\n", u8RetVal);
	return u8RetVal;

}

BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    BOOL bRet = FALSE;

    if(bRC56HasKey)
    {
    	U8 rc_mode = (U8)((_u32IRData >> 17) & 0x07); //ben add 20131017

        (*pu8Key) = _MDrv_IR_RC5RC6ParseKey( (U8)((_u32IRData>>8)&0xFF), (U8)(_u32IRData&0xFF));
        //(*pu8Key) = (_u32IRData&0xFF);
        (*pu8System) = 0;

		if (rc_mode != 0) // TV using mode 0 , ben add 20131017
		{
			*pu8Key = KEYCODE_DUMMY;
		}

        if((*pu8Key) != KEYCODE_DUMMY)
        {
            if(_u8PrevKeyCode != (*pu8Key) || (_u32IRPreData != _u32IRData))
                _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
            else
                _ePrevKeyProperty = E_IR_KEY_PROPERTY_FOLLOWING;

            if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
            {
                (*pu8Flag) = 0;
                _ePrevKeyProperty = E_IR_KEY_PROPERTY_1st;
                _u8PrevKeyCode = (*pu8Key);
                _ulPrevKeyTime = _ulSignalTime = _MDrv_IR_GetSystemTime();
                _u32IRPreData = _u32IRData;
                bRet = TRUE;
            }
            else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_FOLLOWING)
            {
                #define RC6_CHK_RP_TIME       100000  //us
                _ulSignalTime = _MDrv_IR_GetSystemTime();
                if(_ulSignalTime - _ulPrevKeyTime > (RC6_CHK_RP_TIME/1000))
                {
                    (*pu8Flag) = 1;
                    _ulPrevKeyTime = _ulSignalTime;
                    bRet = TRUE;
                }
            }
        }
        else
        {
            _u8PrevKeyCode = KEYCODE_DUMMY;
        }

        bRC56HasKey = FALSE;
    }
    return bRet;
}

#if (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS) || (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)

static BOOL bTouchRCHasValue = FALSE;
static U32 _u32TouchRCData = 0, _u32TouchRCDataReverse = 0;

BOOL _MDrv_IR_GetKey_TouchRC(U32 *TouchRC_Value)
{
	BOOL bRet = FALSE;
	U32	temp_index;
	U32	temp_value, temp_value_reverse;

	if(bTouchRCHasValue == TRUE)
	{
		bTouchRCHasValue = FALSE;
		temp_value = _u32TouchRCDataReverse;
		temp_value_reverse = _u32TouchRCData;

		temp_value >>=1 ; // Remove leading code at LSB
		temp_value_reverse &= 0x0fffffff; // Mask out leading code at MSB

#if (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
		// The following CRC check is for TouchRC
		temp_index = calc_crc4(temp_value&0xffffff);
		if( temp_index != ((temp_value>>24)&0x0f) )
#elif (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
			temp_index = calc_crc4(temp_value_reverse>>4);
		if( temp_index != (temp_value_reverse&0x0f) )
#endif //
		{
			bRet = FALSE;
			DEBUG_IR(printk(" CRC_ERR "));
#ifdef CORE_M0			// Testing on M0 board
			UARTprintf("CRC:%x - ", temp_value&0x0fffffff);
			UARTputHEX_U(temp_index);
			UARTputstr("\r\n");
#endif // #ifdef CORE_M0			// Testing on M0 board

		}
		else
		{
			// Passed CRC4 check, covert the mouse value
			int return_x, return_y;
			BOOL button_0, button_1;


#if (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
			// Touch RC
			return_x = (temp_value)&0x3ff;
			return_y = (temp_value>>10)&0x3ff;
			// END
#elif (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
			return_x = (temp_value_reverse>>18)&0x3ff;
			return_y = (temp_value_reverse>>8)&0x3ff;
#endif //

			button_0 = (temp_value&(1<<22))!=0?TRUE:FALSE;			// "Back"?
			button_1 = (temp_value&(1<<23))!=0?TRUE:FALSE;			// "OK"?

			*TouchRC_Value =  (return_x) | (return_y<<10) | (temp_value&(0x3<<22));
			bRet = TRUE;

#ifdef CORE_M0			// Testing on M0 board
 #if (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)

/*
			// Test uint-to-int(with signed) conversion
			if(return_x&0x300)
			{
			  UARTputchar('-');
			  UARTprintf("%x:", 0x400-return_x);
			}
			else
			{
			  UARTprintf("%x:", return_x);
			}
			if(return_y&0x300)
			{
			  UARTputchar('-');
			  UARTprintf("%x:", 0x400-return_y);
			}
			else
			{
			  UARTprintf("%x:", return_y);
			}
*/

			UARTprintf("%x:", Convert_uint_to_int(return_x));
			UARTprintf("%x:", Convert_uint_to_int(return_y));

 #elif (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
			if((return_x&0x300)!=0)
			{
				UARTputchar( '0'+((return_x>>8)&0x3) );
			}
			UARTputHEX_U(return_x&0xff);
			UARTputchar(':');
			if((return_y&0x300)!=0)
			{
				UARTputchar( '0'+((return_y>>8)&0x3) );
			}
			UARTputHEX_U(return_y&0xff);
 #endif //
			if(button_0)
			{
				UARTputchar('+');
			}
			else
			{
				UARTputchar('-');
			}
			if(button_1)
			{
				UARTputchar('+');
			}
			else
			{
				UARTputchar('-');
			}
			UARTputchar('\r');
			UARTputchar('\n');
#else
			DEBUG_IR(printk(" @%d:%d:",return_x,return_y));
			if(button_0)
			{
				DEBUG_IR(printk("+"));
			}
			else
			{
				DEBUG_IR(printk("-"));
			}
			if(button_1)
			{
				DEBUG_IR(printk("+\r\n"));
			}
			else
			{
				DEBUG_IR(printk("-\r\n"));
			}
#endif		// #define CORE_M0
		}
	}
	return bRet;
}

void TouchRC_Decoder(U32 u32IrCounter)
{
	static U8 _u8TouchRCBit = 0;
	static TouchRCParityType _TouchRCParity = TOUCHRC_PARITY_N;
	static TouchRCFrameHalfIdx _TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
	static TouchRCState _TouchRCState = TOUCHRC_IDLE;
	static TouchRCDataState _TouchRCDataState = TOUCHRC_DATA_IDLE;
	TouchRCSignal TouchRCSignal=TOUCHRC_SIGNAL_NULL;

	#define TOUCHRC_TOTAL_LEN            (1+10+10+2+2+4)

	// For TouchRC
	if(u32IrCounter > IR_RP_TIMEOUT)
	{
		_TouchRCState = TOUCHRC_IDLE;
		_TouchRCParity = TOUCHRC_PARITY_N;
		//UARTputstr("\r\n-");
	}
	else if(IR_PULSE_1_LB < u32IrCounter && u32IrCounter <= IR_PULSE_1_HB)
	{
		TouchRCSignal=TOUCHRC_1T;
		//UARTputchar('1');
	}
	else if(IR_PULSE_2_LB < u32IrCounter && u32IrCounter <= IR_PULSE_2_HB)
	{
		TouchRCSignal=TOUCHRC_2T;
		//UARTputchar('2');
	}
	else if(IR_LEADING_TIME_LB < u32IrCounter && u32IrCounter <= IR_LEADING_TIME_HB)
	{
		_TouchRCState = TOUCHRC_IDLE;
		_TouchRCParity = TOUCHRC_PARITY_N;
		//UARTputchar('H');
	}
	else
	{
		TouchRCSignal = 0;
		if(_TouchRCState == TOUCHRC_IDLE)
		{
			return;
		}
		else
		{
			_TouchRCState = TOUCHRC_FAIL;
		}
		//UARTputstr("\r\nX");
	}

	_TouchRCParity = ((_TouchRCParity+1)%TOUCHRC_PARITY_TOTAL);

	//  Check State
	if(_TouchRCState == TOUCHRC_IDLE)
	{
		_u32TouchRCData = 0;
		_u32TouchRCDataReverse = 0;
		_u8TouchRCBit = 0;

		_TouchRCState = TOUCHRC_LEADER_BIT;
		_TouchRCDataState = TOUCHRC_DATA_IDLE;
	}
	else if(_TouchRCState == TOUCHRC_LEADER_BIT)
	{
		if(TouchRCSignal == TOUCHRC_1T)
		{
			//  Got the RC6 IR Header 1st start bit
			_TouchRCState = TOUCHRC_DATA;
			_TouchRCDataState = TOUCHRC_DATA_INSERT;
			_TouchRCFrameHalf = TOUCHRC_2ND_FRAME_HALF;
		}
		else
		{
			_TouchRCState = TOUCHRC_FAIL;
		}
	}
	else if(_TouchRCState == TOUCHRC_DATA)
	{
		switch(TouchRCSignal)
		{
		case TOUCHRC_1T:
			if(TOUCHRC_1ST_FRAME_HALF ==_TouchRCFrameHalf)
			{
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
			}
			_TouchRCFrameHalf = ((_TouchRCFrameHalf+1)%TOUCHRC_TOTAL_FRAME_HALF);

			break;

		case TOUCHRC_2T:
			if(TOUCHRC_1ST_FRAME_HALF ==_TouchRCFrameHalf)
			{
				_TouchRCDataState = TOUCHRC_DATA_RESET;	// Shouldn't be here
			}
			else
			{
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
			}
			break;

		default:
			_TouchRCState = TOUCHRC_FAIL;
			break;
		}
	}

	if(_TouchRCState == TOUCHRC_DATA)
	{
		if(_TouchRCDataState == TOUCHRC_DATA_INSERT)
		{
			if(_u8TouchRCBit < (TOUCHRC_TOTAL_LEN))
			{
				if(_TouchRCParity == TOUCHRC_PARITY_N)
				{
					_u32TouchRCData = ((_u32TouchRCData<<1) | 0x01);
					_u32TouchRCDataReverse = ((_u32TouchRCDataReverse>>1) | (1<<(TOUCHRC_TOTAL_LEN-1)));
				}
				else
				{
					_u32TouchRCData = (_u32TouchRCData<<1);
					_u32TouchRCDataReverse = (_u32TouchRCDataReverse>>1);
				}

				_u8TouchRCBit++;
				if(_u8TouchRCBit == TOUCHRC_TOTAL_LEN)
					_TouchRCState = TOUCHRC_FINISH;

			}
			else
			{
				_TouchRCDataState = TOUCHRC_DATA_RESET;
			}
		}

		if(_TouchRCDataState == TOUCHRC_DATA_RESET)
		{
			_TouchRCState = TOUCHRC_FAIL;
		}

		_TouchRCDataState = TOUCHRC_DATA_IDLE;
	}

	if(_TouchRCState == TOUCHRC_FINISH)
	{
		if(_u8TouchRCBit == TOUCHRC_TOTAL_LEN)
		{
			//  IR finish, Has Key
			//printk("\n@@ 0x%X(%u)\n", _u32TouchRCData, (_u32TouchRCData&0xFFFF));
			bTouchRCHasValue = TRUE;
#ifdef CORE_M0			// Testing on M0 board
			UART_Show_CPU_Timer();
#endif // #ifdef CORE_M0			// Testing on M0 board
		}
		else
		{
			_TouchRCState = TOUCHRC_FAIL;
		}
	}

	if(_TouchRCState == TOUCHRC_FAIL || _TouchRCState == TOUCHRC_FINISH)
	{
		_TouchRCState = TOUCHRC_IDLE;
		_TouchRCState = TOUCHRC_DATA_IDLE;
		_u8TouchRCBit = 0;
	}
}

void TouchRC_Decoder_Single_Edge(U32 u32IrCounter)
{
	static U8 						_u8TouchRCBit = 0;
	static TouchRCParityType 		_TouchRCParity = TOUCHRC_PARITY_N;
	static TouchRCFrameHalfIdx 		_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
	static TouchRCState 			_TouchRCState = TOUCHRC_IDLE;
	static TouchRCDataState 		_TouchRCDataState = TOUCHRC_DATA_IDLE;
	static Bool						blNeedToRestoreLeadingBit=FALSE;
	       TouchRCSignal			TouchRCSignal=TOUCHRC_SIGNAL_NULL;


	// Determine pulse-width
   	if(u32IrCounter > IR_NO_TOUCH_SIGNAL_TIMEOUT)
   	{
   		_TouchRCState = TOUCHRC_IDLE;
   		//UARTputchar('I');
   	}
   	else if(u32IrCounter > IR_LEADING_TIME_LB)	// Header
	{
		_TouchRCState = TOUCHRC_LEADER_BIT;
		//UARTputchar('L');
	}
	else if(IR_PULSE_2_LB < u32IrCounter && u32IrCounter <= IR_PULSE_2_HB)
	{
		TouchRCSignal=TOUCHRC_2T;
		//UARTputchar('2');
	}
	else if(IR_PULSE_3_LB < u32IrCounter && u32IrCounter <= IR_PULSE_3_HB)
	{
		TouchRCSignal=TOUCHRC_3T;
		//UARTputchar('3');
	}
	else if(IR_PULSE_4_LB < u32IrCounter && u32IrCounter <= IR_PULSE_4_HB)
	{
		TouchRCSignal=TOUCHRC_4T;
		//UARTputchar('4');
	}
	else
	{

		//UARTputchar('E');
		TouchRCSignal = TOUCHRC_SIGNAL_NULL;
		if(_TouchRCState == TOUCHRC_IDLE)
		{
			return;
		}
		else
		{
			_TouchRCState = TOUCHRC_FAIL;
		}
	}

	// Determine what is next state	(whole decoding)
	if(_TouchRCState == TOUCHRC_IDLE)
	{
		_u32TouchRCData = 0;
		_u32TouchRCDataReverse = 0;
		_u8TouchRCBit = 0;
		bTouchRCHasValue = FALSE;

		_TouchRCState = TOUCHRC_DATA;
		_TouchRCDataState = TOUCHRC_DATA_INSERT;
		_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
		TouchRCSignal = TOUCHRC_LEADING_SIGNAL;
		_TouchRCParity = TOUCHRC_PARITY_N;
		blNeedToRestoreLeadingBit = FALSE;
		//UARTputchar('i');
	}
	else if(_TouchRCState == TOUCHRC_LEADER_BIT)
	{
		if(blNeedToRestoreLeadingBit==TRUE)
		{
			blNeedToRestoreLeadingBit = FALSE;
			_u32TouchRCData = 0x01;
			_u32TouchRCDataReverse = (1<<(TOUCHRC_TOTAL_LEN-1));
			_u8TouchRCBit = 1;
			_TouchRCState = TOUCHRC_DATA;
			_TouchRCDataState = TOUCHRC_DATA_INSERT;
			_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
			bTouchRCHasValue = FALSE;

			// Adopted from below -- else if(_TouchRCState == TOUCHRC_DATA)
			switch(TouchRCSignal)
			{
				case TOUCHRC_2T:
						_TouchRCParity = TOUCHRC_PARITY_N;
					break;
				case TOUCHRC_3T:
						_TouchRCParity = TOUCHRC_PARITY_P;
						_TouchRCFrameHalf = TOUCHRC_2ND_FRAME_HALF;
					break;
				default:
					_TouchRCState = TOUCHRC_FAIL;
					break;
			}
			//UARTputchar('j');
		}
		// Check if one last bit (1) is missing
		else if((_u8TouchRCBit == (TOUCHRC_TOTAL_LEN-1))&&(_TouchRCFrameHalf!=TOUCHRC_1ST_FRAME_HALF))
		{
			_u32TouchRCData = ((_u32TouchRCData<<1) | 0x01);
			_u32TouchRCDataReverse = ((_u32TouchRCDataReverse>>1) | (1<<(TOUCHRC_TOTAL_LEN-1)));
			_TouchRCDataState = TOUCHRC_DATA_IDLE;
			_u8TouchRCBit = 0;
			bTouchRCHasValue = TRUE;
			blNeedToRestoreLeadingBit = TRUE;
#ifdef CORE_M0			// Testing on M0 board
//			UARTputchar('*');
//			UART_Show_CPU_Timer();
#endif // #ifdef CORE_M0			// Testing on M0 board
			return;		// No need to be further processed.
		}
		else
		{
			// Same as if(_TouchRCState == TOUCHRC_IDLE) {}
			_u32TouchRCData = 0;
			_u32TouchRCDataReverse = 0;
			_u8TouchRCBit = 0;
			bTouchRCHasValue = FALSE;

			_TouchRCState = TOUCHRC_DATA;
			_TouchRCDataState = TOUCHRC_DATA_INSERT;
			_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
			TouchRCSignal = TOUCHRC_LEADING_SIGNAL;
			_TouchRCParity = TOUCHRC_PARITY_N;
			//blNeedToRestoreLeadingBit = FALSE;
			//UARTputchar('k');
		}
	}
	else if(_TouchRCState == TOUCHRC_DATA)
	{
		//UARTputchar('d');
		switch(TouchRCSignal)
		{
			case TOUCHRC_2T:
				if(TOUCHRC_1ST_FRAME_HALF == _TouchRCFrameHalf)
				{
					_TouchRCParity = TOUCHRC_PARITY_N;
				}
				else
				{
					_TouchRCParity = TOUCHRC_PARITY_P;
				}
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
				break;
			case TOUCHRC_3T:
				if(TOUCHRC_1ST_FRAME_HALF == _TouchRCFrameHalf)
				{
					_TouchRCParity = TOUCHRC_PARITY_P;
					_TouchRCFrameHalf = TOUCHRC_2ND_FRAME_HALF;
				}
				else
				{
					_TouchRCParity = TOUCHRC_PARITY_NN;
					_TouchRCFrameHalf = TOUCHRC_1ST_FRAME_HALF;
				}
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
				break;

			case TOUCHRC_4T:
				if(TOUCHRC_1ST_FRAME_HALF == _TouchRCFrameHalf)
				{
					_TouchRCDataState = TOUCHRC_DATA_RESET;
				}
				else
				{
					_TouchRCParity = TOUCHRC_PARITY_NP;
					_TouchRCDataState = TOUCHRC_DATA_INSERT;
					//_TouchRCFrameHalf = TOUCHRC_2ND_FRAME_HALF;
				}
				break;

			case TOUCHRC_LEADING_SIGNAL:
				_TouchRCDataState = TOUCHRC_DATA_INSERT;
				break;

			default:
				_TouchRCState = TOUCHRC_FAIL;
				break;
		}
	}

	// Process Data insertion
	if(_TouchRCState == TOUCHRC_DATA)
	{
		if(_TouchRCDataState == TOUCHRC_DATA_INSERT)
		{
			if(_u8TouchRCBit < (TOUCHRC_TOTAL_LEN))	 	// still bit to be filled?
			{
				if(_TouchRCParity == TOUCHRC_PARITY_N)
				{
					_u32TouchRCData = ((_u32TouchRCData<<1) | 0x01);
					_u32TouchRCDataReverse = ((_u32TouchRCDataReverse>>1) | (1<<(TOUCHRC_TOTAL_LEN-1)));
					_u8TouchRCBit++;
				}
				else if(_TouchRCParity == TOUCHRC_PARITY_P)
				{
					_u32TouchRCData = (_u32TouchRCData<<1);
					_u32TouchRCDataReverse = (_u32TouchRCDataReverse>>1);
					_u8TouchRCBit++;
				}
				else if(_u8TouchRCBit < (TOUCHRC_TOTAL_LEN-1))
				{
					if(_TouchRCParity == TOUCHRC_PARITY_NN)
					{
						_u32TouchRCData = ((_u32TouchRCData<<2) | 0x03);
						_u32TouchRCDataReverse = (_u32TouchRCDataReverse>>2) | (3<<(TOUCHRC_TOTAL_LEN-2));
						_u8TouchRCBit+=2;
					}
					else if(_TouchRCParity == TOUCHRC_PARITY_NP)
					{
						_u32TouchRCData = (_u32TouchRCData<<2) | 0x02;
						_u32TouchRCDataReverse = (_u32TouchRCDataReverse>>2) | (1<<(TOUCHRC_TOTAL_LEN-2));
						_u8TouchRCBit+=2;
					}
					else
					{
						_TouchRCState = TOUCHRC_FAIL;
					}
			    }

				if(_u8TouchRCBit == TOUCHRC_TOTAL_LEN)
				{
					_TouchRCState = TOUCHRC_FINISH;
				}
			}
			else
			{
				_TouchRCDataState = TOUCHRC_DATA_RESET;
			}
		}

		if(_TouchRCDataState == TOUCHRC_DATA_RESET)
		{
			_TouchRCState = TOUCHRC_FAIL;
		}

		_TouchRCDataState = TOUCHRC_DATA_IDLE;
	}

	//UARTputHEX_U(_u8TouchRCBit);

	if(_TouchRCState == TOUCHRC_FINISH)
	{
		if(_u8TouchRCBit == TOUCHRC_TOTAL_LEN)
		{
			//  IR finish, Has Key
			//printk("\n@@ 0x%X(%u)\n", _u32TouchRCData, (_u32TouchRCData&0xFFFF));
#ifdef CORE_M0			// Testing on M0 board
//			UART_Show_CPU_Timer();
#endif // #ifdef CORE_M0			// Testing on M0 board
			bTouchRCHasValue = TRUE;
		}
		else
		{
			_TouchRCState = TOUCHRC_FAIL;
		}
	}

	if(_TouchRCState == TOUCHRC_FAIL || _TouchRCState == TOUCHRC_FINISH)
	{
		_TouchRCState = TOUCHRC_IDLE;
		_TouchRCDataState = TOUCHRC_DATA_IDLE;
		_u8TouchRCBit = 0;
		blNeedToRestoreLeadingBit = FALSE;
	}
	return;
}
#endif // #if((MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)||(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS))

void RC6_Decoder(U32 u32IrCounter)
{
	IrRC6Signal irSignal = IR_SIGNAL_NULL;

	//  For RC6
	//  Check Header
	if(u32IrCounter > IR_RP_TIMEOUT)
	{
		_irState = IR_RC6_IDLE;
		_irParity = IR_PARITY_N;
	}
	else if(T6_MIN < u32IrCounter && u32IrCounter <= T6_MAX)
	{
		irSignal = IR_6T;
	}
	else if(T2_MIN < u32IrCounter && u32IrCounter <= T2_MAX)
	{
		irSignal = IR_2T;
	}
	else if(T3_MIN < u32IrCounter && u32IrCounter <= T3_MAX)
	{
		irSignal = IR_3T;
	}
	else if(T1_MIN < u32IrCounter && u32IrCounter <= T1_MAX)
	{
		irSignal = IR_1T;
	}
	else
	{
		irSignal = 0;
		if(_irState == IR_RC6_IDLE)
		{
			return;
		}
		else
			_irState = IR_RC6_FAIL;
	}

	_irParity = ((_irParity+1)%IR_PARITY_TOTAL);

	//  Check State
	if(_irState == IR_RC6_IDLE)
	{
		_u32IRData = 0;
		_u8IRBit = 0;

		_irState = IR_RC6_LEADER_6T;
		_irDataState = IR_RC6_DATA_IDLE;
		_irDataTrailState = IR_RC6_DATA_TRAIL_IDLE;
		bRC56HasKey = FALSE;
	}
	else if(_irState == IR_RC6_LEADER_6T)
	{
		if(irSignal == IR_6T)
			_irState = IR_RC6_LEADER_2T;
		else
			_irState = IR_RC6_FAIL;
	}
	else if(_irState == IR_RC6_LEADER_2T)
	{
		if(irSignal == IR_2T)
		{
			//  Got the RC6 IR Header 1st start bit
			_irState = IR_RC6_DATA;
			_irDataState = IR_RC6_DATA_IDLE;
			_irDataTrailState = IR_RC6_DATA_TRAIL_IDLE;
			_irFrameHalf = IR_1ST_FRAME_HALF;
		}
		else
			_irState = IR_RC6_FAIL;
	}
	else if(_irState == IR_RC6_DATA)
	{
		switch(irSignal)
		{
		case IR_1T:
			if((_irDataTrailState & IR_RC6_DATA_TRAIL_INSERT) == IR_RC6_DATA_TRAIL_INSERT)
			{
				_irDataState = IR_RC6_DATA_RESET;
			}
			else if(IR_1ST_FRAME_HALF ==_irFrameHalf)
			{
				_irDataState = IR_RC6_DATA_INSERT;
			}

			_irFrameHalf = ((_irFrameHalf+1)%IR_TOTAL_FRAME_HALF);

			break;

		case IR_2T:
			if(IR_1ST_FRAME_HALF ==_irFrameHalf)
			{
				_irDataState = IR_RC6_DATA_TRAIL;
			}
			else
			{
				if((_irDataTrailState & IR_RC6_DATA_TRAIL_INSERT) == IR_RC6_DATA_TRAIL_INSERT)
				{
					if(_u8IRBit == IR_RC6_CONT_START_BIT)
					{
						_irDataTrailState -= IR_RC6_DATA_TRAIL_INSERT;
						_irFrameHalf = IR_1ST_FRAME_HALF;
					}
					else
					{
						_irDataState = IR_RC6_DATA_RESET;
					}
				}
				else
				{
					_irDataState = IR_RC6_DATA_INSERT;
				}
			}

			break;

		case IR_3T:
			if(IR_1ST_FRAME_HALF == _irFrameHalf)
			{
				_irDataState = IR_RC6_DATA_RESET;
			}
			else
			{
				if((_irDataTrailState & IR_RC6_DATA_TRAIL_INSERT) == IR_RC6_DATA_TRAIL_INSERT)
				{
					if(_u8IRBit == IR_RC6_CONT_START_BIT)
					{
						_irDataTrailState -= IR_RC6_DATA_TRAIL_INSERT;
						_irDataState = IR_RC6_DATA_INSERT;
					}
					else
					{
						_irDataState = IR_RC6_DATA_RESET;
					}
				}
				else
				{
					_irDataState = IR_RC6_DATA_TRAIL;
				}
			}

			break;

		default:
			_irState = IR_RC6_FAIL;
			break;
		}
	}

	if(_irState == IR_RC6_DATA)
	{
		if(_irDataState == IR_RC6_DATA_TRAIL)
		{
			if(_u8IRBit == (IR_RC6_CONT_START_BIT - 1))
			{
				_irDataTrailState = IR_RC6_DATA_TRAIL_I_R;
				_irDataState = IR_RC6_DATA_INSERT;
				_irFrameHalf = IR_2ND_FRAME_HALF;
			}
			else
			{
				_irDataState = IR_RC6_DATA_RESET;
			}
		}

		if(_irDataState == IR_RC6_DATA_INSERT)
		{
			if(_u8IRBit < (IR_RC6_TOTAL_LEN))
			{
				if(_irParity == IR_PARITY_N)
				{
					_u32IRData = ((_u32IRData<<1) | 0x01);
				}
				else
				{
					_u32IRData = (_u32IRData<<1);
				}

				_u8IRBit++;
				if(_u8IRBit == IR_RC6_TOTAL_LEN)
					_irState = IR_RC6_FINISH;
			}
			else
			{
				_irDataState = IR_RC6_DATA_RESET;
			}
		}

		if(_irDataState == IR_RC6_DATA_RESET)
		{
			_irState = IR_RC6_FAIL;
		}

		_irDataState = IR_RC6_DATA_IDLE;
	}

	if(_irState == IR_RC6_FINISH)
	{
		if(_u8IRBit == (IR_RC6_TOTAL_LEN) &&
				((_irDataTrailState&IR_RC6_DATA_TRAIL_RECEIVE) & IR_RC6_DATA_TRAIL_RECEIVE))
		{
			//  IR finish, Has Key
			//printk("\n@@ 0x%X(%u)\n", _u32IRData, (_u32IRData&0xFFFF));
#ifdef CORE_M0			// Testing on M0 board
//			UART_Show_CPU_Timer();
#endif // #ifdef CORE_M0			// Testing on M0 board
			bRC56HasKey = TRUE;
		}
		else
			_irState = IR_RC6_FAIL;
	}

	if(_irState == IR_RC6_FAIL || _irState == IR_RC6_FINISH)
	{
		_irState = IR_RC6_IDLE;
		_irDataState = IR_RC6_DATA_IDLE;
		_irDataTrailState = IR_RC6_DATA_TRAIL_IDLE;
		_u8IRBit = 0;
	}
}

inline void RC6_Decoder_Siggle_Edge_Last_Bit_TimeOut(void)
{
	_u32IRData = ((Return_RC6_Value() << 1)|1);
	bRC56HasKey = TRUE;
	Clear_RC6_Automata();
	//UARTputchar('*');
}

void RC6_Decoder_Single_Edge(U32 u32IrCounter)
{
	Bool	Automata_OK;

	ClearMyTimer_us();
	// here we deal with the situation when a not-long-enough (<=IR_RP_TIMEOUT+2ms)  pulse-width occurred at missing last bit
	// at main-loop, it deal with long-enough pulse-width occurred at missing last bit
	if(Check_RC6_if_missing_last_bit()==TRUE)
	{
		// Required when Timeout mechanism is implemented
		if(ARC6_T3_MAX < u32IrCounter)
		{
			RC6_Decoder_Siggle_Edge_Last_Bit_TimeOut();
			if(RC6_AUTOMATA_TIMEOUT_TIME==u32IrCounter)
			{
				return;
			}
		}
	}

	if(ARC6_T2_MIN < u32IrCounter && u32IrCounter <= ARC6_T2_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_2T_VALUE);
		//UARTputchar('2');
	}
	else if(ARC6_T3_MIN < u32IrCounter && u32IrCounter <= ARC6_T3_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_3T_VALUE);
		//UARTputchar('3');
	}
	else if(ARC6_T4_MIN < u32IrCounter && u32IrCounter <= ARC6_T4_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_4T_VALUE);
		//UARTputchar('4');
	}
	else if(ARC6_T5_MIN < u32IrCounter && u32IrCounter <= ARC6_T5_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_5T_VALUE);
		//UARTputchar('5');
	}
	else if(ARC6_T6_MIN < u32IrCounter && u32IrCounter <= ARC6_T6_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_6T_VALUE);
		//UARTputchar('6');
	}
	else if(ARC6_T8_MIN < u32IrCounter && u32IrCounter <= ARC6_T8_MAX)
	{
		Automata_OK = Update_RC6_Automata(RC6_8T_VALUE);
		//UARTputchar('8');
	}
	else if(u32IrCounter > ARC6_IR_RP_TIMEOUT)
	{
		Automata_OK = Update_RC6_Automata(RC6_IR_TIMEOUT);
		//UARTputchar('T');
	}
	else
	{
		Clear_RC6_Automata();
		//UARTputchar('X');
		return;
	}

	if(Automata_OK==TRUE)
	{
		if(Check_RC6_Automata_Finished()==TRUE)
		{
			_u32IRData = Return_RC6_Value();
			bRC56HasKey = TRUE;
			Clear_RC6_Automata();
			//UARTputchar(' ');
		}
		// Required when Timeout mechanism is implemented
		else if(Check_RC6_if_missing_last_bit()==TRUE)
		{
			SetMyTimer_us(ARC6_T3_MAX+1);
		}
	}
	else
	{
		// Error State;
		Clear_RC6_Automata();
		//UARTputchar('F');
		//UARTputchar('\n');
	}
}

void NEC_Decoder_Single_Edge(U32 u32IrCounter)
{
	Bool	Automata_OK;

	if(u32IrCounter > ANEC_IR_RP_TIMEOUT)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_BLANK_MIN);
		//UARTputchar('^');
	}
	else if(ANEC_TRPT_WAIT_MIN < u32IrCounter && u32IrCounter <= ANEC_TRPT_WAIT_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_FINISH_REPEAT);
		//UARTputchar('_');
	}
	else if(ANEC_TDATA_WAIT_MIN < u32IrCounter && u32IrCounter <= ANEC_TDATA_WAIT_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_FINISH_DATA);
		//UARTputchar('-');
	}
	else if(ANEC_TLD_MIN < u32IrCounter && u32IrCounter <= ANEC_TLD_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_LEADING);
		//UARTputchar('L');
	}
	else if(ANEC_TRPT_MIN < u32IrCounter && u32IrCounter <= ANEC_TRPT_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T_REPEAT);
		//UARTputchar('R');
	}
	else if(ANEC_T1_MIN < u32IrCounter && u32IrCounter <= ANEC_T1_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T1_VALUE);
		//UARTputchar('1');
	}
	else if(ANEC_T0_MIN < u32IrCounter && u32IrCounter <= ANEC_T0_MAX)
	{
		Automata_OK = Update_NEC_Automata(NEC_T0_VALUE);
		//UARTputchar('0');
	}
	else
	{
		Automata_OK = FALSE;
		//UARTputchar('x');
	}

	if(Automata_OK==FALSE)
	{
		// Error State;
		Clear_NEC_Automata();
		//UARTputchar('f');
	}
}

BOOL _MDrv_IR_GetKey_NEC_Single_Sided(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
	BOOL bRet = FALSE;

	// For Compatible purpose only
	static uint32_t _u32IRCount = 0;
	static uint8_t  _u8IRHeadReceived=0;
	static uint8_t  _u8IRRepeateDetect=0;
	static uint8_t  _u8IRRepeated=0;
	static uint8_t  _u8IRRepeatedNum=0;
	static uint8_t  _u8IRType=0;
	uint32_t	_u32IRData[IR_SWDECODE_MODE_BUF_LEN];
	#ifndef TPVFACKeyHeader
	#define TPVFACKeyHeader {0x20,0x08,0x01}
	#endif
	#ifndef PANDAKeyHeader
	#define PANDAKeyHeader {0x08,0xF7,0x00}
	#endif
	#ifndef AOCKeyHeader
	#define AOCKeyHeader {0x00,0xbd,0x00}
	#endif
	static struct TPV_HeadCode TPV_HeadCode_Group[]= {TPVFACKeyHeader,PANDAKeyHeader,AOCKeyHeader}; // Jeremy -

	U32 u8Byte, u8Bit;
	U8 u8IRSwModeBuf[IR_RAW_DATA_NUM];
	U32 *pu32IRData = NULL;

////	*pu8System = 0;

	// Update _u8IRRepeated value
    if(Return_NEC_if_Repeat()!=FALSE)
    {
	   _u8IRRepeated = 1;
    }
    else
    {
       _u8IRRepeated = 0;
    }
/*
	for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
	{
		u8IRSwModeBuf[u8Byte] = 0;
	}
*/
	if(_u8IRRepeated)//repeat key
	{
		/* TPV Jack 130628, remove for filter repeat key*/
		//if (_u8IRRepeatedNum < 5)
		//if (_u8IRRepeatedNum < 10) // delay the first repeate key
		//{
		//	_u8IRRepeatedNum++;
		//    	return FALSE;
		//}

		_u8IRRepeated = 0;
		_u8IRHeadReceived = 0;//clear head receive flag
		*pu8Key = _u8PrevKeyCode;
		*pu8Flag = 1;
		bRet = TRUE;
		*pu8System = _u8PrevSystemCode;//timl

		goto done;
	}
////	if (_u32IRCount<(2+IR_RAW_DATA_NUM*8))
	if(Check_NEC_Automata_Finished()==FALSE)
		return FALSE; //not complete yet

#if 0
	DEBUG_IR(printk("_u32IRCount=%d", _u32IRCount));
	for( u8Byte=0; u8Byte<_u32IRCount; u8Byte++)
	{
		DEBUG_IR(printk(" %d", _u32IRData[u8Byte]));
	}

	if( _u32IRData[0] > IR_HDC_LOB && _u32IRData[1] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[1] < REG_IR_OFC_UPB+IR_LG01H_UPB )
	{
		pu32IRData = &_u32IRData[2];
		DEBUG_IR(printk(" H1 "));
	}
	else if( _u32IRData[1] > IR_HDC_LOB && _u32IRData[2] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[2] < REG_IR_OFC_UPB+IR_LG01H_UPB )
	{
		pu32IRData = &_u32IRData[3];
		DEBUG_IR(printk(" H2 "));
	}
	else
	{
		if( _u32IRData[0] > IR_LEADING_TIME_LB )
		{
			DEBUG_IR(printk(" H3.1 "));
			return FALSE; 	// Could be Touch RC, return for checking within TouchRC
		}
		else if( _u32IRData[1] > IR_LEADING_TIME_LB )
		{
			DEBUG_IR(printk(" H3.2 "));
			return FALSE; 	// Could be Touch RC, return for checking within TouchRC
		}
		else
		{
			DEBUG_IR(printk(" invalid leader code\n"));
			bRet = FALSE;
			goto done;
		}
	}

	for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
	{
		for( u8Bit=0; u8Bit<8; u8Bit++)
		{
			u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
			u8IRSwModeBuf[u8Byte] >>= 1;

			if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
			{
				u8IRSwModeBuf[u8Byte] |= 0x00;
			}
			else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
			{
				u8IRSwModeBuf[u8Byte] |= 0x80;
			}
			else
			{
				DEBUG_IR(printk(" invalid waveform,0x%x\n",u32BitLen));
				bRet = FALSE;
				goto done;
			}
		}
	}
	//printk("TPV IRKey=%d--%d\n",u8IRSwModeBuf[1],_u8IRHeaderCode1);
#endif // 0
	u8Byte = Return_NEC_Value();
	u8IRSwModeBuf[0] = u8Byte & 0xff;
	u8Byte >>=8;
	u8IRSwModeBuf[1] = u8Byte & 0xff;
	u8Byte >>=8;
	u8IRSwModeBuf[2] = u8Byte & 0xff;
	u8Byte >>=8;
	u8IRSwModeBuf[3] = u8Byte & 0xff;
	*pu8System = u8IRSwModeBuf[1];//timl

	if(1>0)
	{
		if(u8IRSwModeBuf[0] == 0x20&&u8IRSwModeBuf[1] == 0x08
				&&u8IRSwModeBuf[2] == 0x6D&&u8IRSwModeBuf[3] == 0x92)
		{
			FactoryKeyEnable=1;
		}
		else if (FactoryKeyEnable==1 )
		{
			if(u8IRSwModeBuf[0] == 0xDF&&u8IRSwModeBuf[1] == 0xF7
					&&u8IRSwModeBuf[2] == 0x92&&u8IRSwModeBuf[3] == 0x6D)
			{
				FactoryKeyEnable=2;
				*pu8Key = 0x00;
				*pu8System=0xFF;
				*pu8Flag = 0;
				bRet = TRUE;
				_u8PrevKeyCode = *pu8Key;
				_u8PrevSystemCode = *pu8System;
				_u8IRRepeateDetect = 1;
				_u8IRHeadReceived = 0;
				_u8IRType = 0;
				goto done;
			}
			else
			{
				FactoryKeyEnable=0;
			}
		}
	}
	if(2>0)
	{
		int index;
		//    		for (index=0;index< sizeof(TPV_HeadCode_Group)/sizeof(struct TPV_HeadCode);index++)			// Jeremy -
		for (index=0;index< 3;index++)			// Jeremy -
		{
			if(u8IRSwModeBuf[0] == TPV_HeadCode_Group[index].Headcode1)
			{
				if(u8IRSwModeBuf[1] ==  TPV_HeadCode_Group[index].Headcode2)
				{
					if(FactoryKeyEnable==2||TPV_HeadCode_Group[index].FactoryRC==0)
					{
						if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
						{
							*pu8Key = u8IRSwModeBuf[2];
							*pu8Flag = 0;
							bRet = TRUE;
							_u8PrevKeyCode = *pu8Key;
							_u8PrevSystemCode = *pu8System;
							_u8IRRepeateDetect = 1;
							_u8IRHeadReceived = 0;
							_u8IRType = 0;
							goto done;
						}
					}
				}
			}
		}
	}

	DEBUG_IR(printk(" invalid data\n"));
	bRet = FALSE;

	done:
////	_u32IRCount = 0;
	return bRet;
}

#if((MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)||(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS))
void _MDrv_IR_ISR(uint32_t wTimeStamp)
{
	U8 u8Key=0, u8RepeatFlag=0;
	U8 u8System = 0;
	uint32_t 	temp_value=0;
	BOOL bHaveKey = FALSE, bHaveTouchKey = FALSE;

/*
	temp_value = _MDrv_IR_GetSystemTime();
	if (ulPreTime > temp_value)
	{
		diff = 0x100000000 - ulPreTime + temp_value;
	}
	else
	{
		diff = temp_value - ulPreTime;
	}
	ulPreTime = temp_value;
*/

	//TouchRC_Decoder(wTimeStamp);
	TouchRC_Decoder_Single_Edge(wTimeStamp);

    //RC6_Decoder(wTimeStamp);
	RC6_Decoder_Single_Edge(wTimeStamp);
	NEC_Decoder_Single_Edge(wTimeStamp);

	ulPreTime = _MDrv_IR_GetSystemTime();

    //if(MSTAR_FANTASY_PROTOCOL_SUPPORTED)
    if((bHaveKey=_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag)) != 0)
    {
        _ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();

        //temporary solution, need to implement ring buffer for this
        _KeyReceived.u8Key = u8Key;
        _KeyReceived.u8System = u8System;
        _KeyReceived.u8Flag = u8RepeatFlag;
        _KeyReceived.u8Valid = 1;
#ifdef CORE_M0			// Testing on M0 board
        UARTputstr("RC6@");
        UARTputHEX_U(u8System&0xff);
		UARTputHEX_U(u8Key&0xff);
		UARTputchar('\r');
		UARTputchar('\n');
#endif // #ifdef CORE_M0			// Testing on M0 board
    }
    else if ((bHaveKey=_MDrv_IR_GetKey_NEC_Single_Sided(&u8Key, &u8System, &u8RepeatFlag)) != 0)
    {
		_ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();

		//temporary solution, need to implement ring buffer for this
		_KeyReceived.u8Key = u8Key;
		_KeyReceived.u8System = u8System;
		_KeyReceived.u8Flag = u8RepeatFlag;
		_KeyReceived.u8Valid = 1;
		if(u8RepeatFlag)
		{
		  UARTputstr("RNEC:");
		}
		else
		{
			UARTputstr("NEC:");
		}
		UARTputHEX_U(u8System&0xff);
		UARTputHEX_U(u8Key&0xff);
		UARTputstr("\r\n");
    }
	else if ((bHaveTouchKey=_MDrv_IR_GetKey_TouchRC(&temp_value)) != 0)
	{
		_ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();

	}
}
#endif // #if((MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)||(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS))

#if (MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_PIXART)
void _MDrv_IR_ISR(uint32_t wTimeStamp)
{
//    U16 u16IrCounter;
    U32 tmp, u16IrCounter, diff;
    unsigned long ulPreTime;

   	u16IrCounter = wTimeStamp;

   	tmp = _MDrv_IR_GetSystemTime();
   	if (ulPreTime > tmp)
   	{
   		diff = 0x100000000 - ulPreTime + tmp;
   	}
   	else
   	{
   		diff = tmp - ulPreTime;
   	}
   	ulPreTime = tmp;
    if ((diff > IR_TIMEOUT_CYC/1000))//reset
    {
        RCBitsCnt = 0;
        _u8PrevKeyCode = 0xff;
    }
    //i++;
    //if(i % 2)
       // printk("%d:%d\n",i,u16IrCounter);
    //return IRQ_HANDLED;

    if(P25_MIN < u16IrCounter && u16IrCounter < P25_MAX)
    {
        tgbits = 0x00;
        RCByte[0] = 0x00;
        RCByte[1] = 0x00;
        RCByte[2] = 0x00;
        RCByte[3] = 0x00;
        RCBitsCnt = 0;
        RCMode = 0;

        StartDecodeFlag = TRUE;
        //printk("START\n");
        //head code start
    }
    else if( (P16_MIN < u16IrCounter && u16IrCounter < P16_MAX) && StartDecodeFlag) //! it is 00 bit sequence
    {
        tgbits = 0x00;
        //printk("00\n");
        //_u32IrRcmmData <<= 2;
        //_u32IrRcmmData |= tgbits;
        RCByte[RCBitsCnt>>3] <<= 2;
        RCByte[RCBitsCnt>>3] |= tgbits;
        RCBitsCnt += 2;
    }
    else if( (P22_MIN < u16IrCounter && u16IrCounter< P22_MAX) && StartDecodeFlag) //! it is 01 bit sequence
    {
        tgbits = 0x01;
        //printk("01\n");
        //_u32IrRcmmData <<= 2;
        //_u32IrRcmmData |= tgbits;
        RCByte[RCBitsCnt>>3] <<= 2;
        RCByte[RCBitsCnt>>3] |= tgbits;
        RCBitsCnt += 2;
    }
    else if( (P28_MIN < u16IrCounter && u16IrCounter < P28_MAX) && StartDecodeFlag) //! it is 10 bit sequence
    {
        tgbits = 0x02;
        //printk("10\n");
        //_u32IrRcmmData <<= 2;
        //_u32IrRcmmData |= tgbits;
        RCByte[RCBitsCnt>>3] <<= 2;
        RCByte[RCBitsCnt>>3] |= tgbits;
        RCBitsCnt += 2;

    }
    else if( (P34_MIN < u16IrCounter && u16IrCounter < P34_MAX) && StartDecodeFlag) //! it is 11 bit sequence
    {
        tgbits = 0x03;
        //printk("11\n");
        //_u32IrRcmmData <<= 2;
        //_u32IrRcmmData |= tgbits;
        RCByte[RCBitsCnt>>3] <<= 2;
        RCByte[RCBitsCnt>>3] |= tgbits;
        RCBitsCnt += 2;
    }
    else
    {
        StartDecodeFlag = FALSE;
        RCBitsCnt = 0;
        UpDataFlage = FALSE;
        tgbits = 0x00;
        RCByte[0] = 0x00;
        RCByte[1] = 0x00;
        RCByte[2] = 0x00;
        RCByte[3] = 0x00;
    }

    /*
    if( RCBitsCnt == 12)
    {
        printk("12\n");
        if(RCByte[0] & 0xC0)
        {
            RCMode = RCMMBASIC_MODE;

            tgbits = 1 << ((RCByte[0]&0xC0) >> 6);
            if(tgbits == RC_MODE)
            {
                RCBitsCnt = 0;
                u16CustomerID = 0;
                RCMode = 0;
                UpDataFlage = FALSE;
                tgbits = 0x00;
                RCByte[0] = 0x00;
                RCByte[1] = 0x00;
                RCByte[2] = 0x00;
                RCByte[3] = 0x00;

            }

            RCMode |= tgbits; //BASIC_Mouse, //BASIC_keyboard, //BASIC_joystick
            UpDataFlage = TRUE;
            //SaveRCMMIRData();
        }
    }*/
    if(RCBitsCnt == 24)
    {
        if(RCByte[0] & 0x20)
        {
            RCMode |= RCMMOEM_LONGID_MODE;

            tgbits = (RCByte[1]&0x0C) >> 2;
            RCMode |= 1<<tgbits; //OEM_LONGID_RC, //OEM_LONGID_Mouse, //OEM_LONGID_keyboard, //OEM_LONGID_joystick
        }
        else if( ((RCByte[0]&0xFC)>>2) == 0x03 )
        {
            RCMode |= RCMMOEM_SHORTID_MODE;

            tgbits = (RCByte[1]&0x0C) >> 2;
            RCMode |= 1<<tgbits; //OEM_SHORTID_RC, //OEM_SHORTID_Mouse, //OEM_SHORTID_keyboard, //OEM_SHORTID_joystick
        }

        if( (RCMode & MOUSE_MODE) || (RCMode & KEYBOARD_MODE) )
        {
            StartDecodeFlag = FALSE;
            UpDataFlage = TRUE;
        }

    }
    else if(RCBitsCnt >= 32)
    {
        if( (RCMode & RC_MODE) || (RCMode & JOYSTICK_MODE) )
        {
            StartDecodeFlag = FALSE;
            UpDataFlage = TRUE;
        }
        else
        {
            RCBitsCnt = 0;
            u16CustomerID = 0;
            RCMode = 0;
            UpDataFlage = FALSE;
            tgbits = 0x00;
            RCByte[0] = 0x00;
            RCByte[1] = 0x00;
            RCByte[2] = 0x00;
            RCByte[3] = 0x00;
        }
    }
    ulPreTime = _MDrv_IR_GetSystemTime();

}
#endif //

#endif // #if (IR_TYPE_SEL == IR_TYPE_EBONY)

#if (IR_TYPE_SEL == IR_TYPE_AOC)
BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
	BOOL bRet = FALSE;

	U32 u8Byte, u8Bit;
	U8 u8IRSwModeBuf[IR_RAW_DATA_NUM];
	U32 *pu32IRData = NULL;
	*pu8System = 0;

	for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
	{
		u8IRSwModeBuf[u8Byte] = 0;
	}

	if(_u8IRRepeated)//repeat key
	{
		/* TPV Jack 130628, remove for filter repeat key*/
		//if (_u8IRRepeatedNum < 5)
		//if (_u8IRRepeatedNum < 10) // delay the first repeate key
		//{
		//	_u8IRRepeatedNum++;
		//    	return FALSE;
		//}

		_u8IRRepeated = 0;
		_u8IRHeadReceived = 0;//clear head receive flag
		*pu8Key = _u8PrevKeyCode;
		*pu8Flag = 1;
		bRet = TRUE;
		*pu8System = _u8PrevSystemCode;//timl

		goto done;
	}
	if (_u32IRCount<(2+IR_RAW_DATA_NUM*8))
		return FALSE; //not complete yet

	DEBUG_IR(printk("_u32IRCount=%d", _u32IRCount));
	for( u8Byte=0; u8Byte<_u32IRCount; u8Byte++)
	{
		DEBUG_IR(printk(" %d", _u32IRData[u8Byte]));
	}

	if( _u32IRData[0] > IR_HDC_LOB && _u32IRData[1] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[1] < REG_IR_OFC_UPB+IR_LG01H_UPB )
	{
		pu32IRData = &_u32IRData[2];
		DEBUG_IR(printk(" H1 "));
	}
	else if( _u32IRData[1] > IR_HDC_LOB && _u32IRData[2] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[2] < REG_IR_OFC_UPB+IR_LG01H_UPB )
	{
		pu32IRData = &_u32IRData[3];
		DEBUG_IR(printk(" H2 "));
	}
	else
	{
		if( _u32IRData[0] > IR_LEADING_TIME_LB )
		{
			DEBUG_IR(printk(" H3.1 "));
			return FALSE; 	// Could be Touch RC, return for checking within TouchRC
		}
		else if( _u32IRData[1] > IR_LEADING_TIME_LB )
		{
			DEBUG_IR(printk(" H3.2 "));
			return FALSE; 	// Could be Touch RC, return for checking within TouchRC
		}
		else
		{
			DEBUG_IR(printk(" invalid leader code\n"));
			bRet = FALSE;
			goto done;
		}
	}

	for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
	{
		for( u8Bit=0; u8Bit<8; u8Bit++)
		{
			u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
			u8IRSwModeBuf[u8Byte] >>= 1;

			if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
			{
				u8IRSwModeBuf[u8Byte] |= 0x00;
			}
			else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
			{
				u8IRSwModeBuf[u8Byte] |= 0x80;
			}
			else
			{
				DEBUG_IR(printk(" invalid waveform,0x%x\n",u32BitLen));
				bRet = FALSE;
				goto done;
			}
		}
	}
	//printk("TPV IRKey=%d--%d\n",u8IRSwModeBuf[1],_u8IRHeaderCode1);
	*pu8System = u8IRSwModeBuf[1];//timl

	if(1>0)
	{
		if(u8IRSwModeBuf[0] == 0x20&&u8IRSwModeBuf[1] == 0x08
				&&u8IRSwModeBuf[2] == 0x6D&&u8IRSwModeBuf[3] == 0x92)
		{
			FactoryKeyEnable=1;
		}
		else if (FactoryKeyEnable==1 )
		{
			if(u8IRSwModeBuf[0] == 0xDF&&u8IRSwModeBuf[1] == 0xF7
					&&u8IRSwModeBuf[2] == 0x92&&u8IRSwModeBuf[3] == 0x6D)
			{
				FactoryKeyEnable=2;
				*pu8Key = 0x00;
				*pu8System=0xFF;
				*pu8Flag = 0;
				bRet = TRUE;
				_u8PrevKeyCode = *pu8Key;
				_u8PrevSystemCode = *pu8System;
				_u8IRRepeateDetect = 1;
				_u8IRHeadReceived = 0;
				_u8IRType = 0;
				goto done;
			}
			else
			{
				FactoryKeyEnable=0;
			}
		}
	}
	if(2>0)
	{
		int index;
		//    		for (index=0;index< sizeof(TPV_HeadCode_Group)/sizeof(struct TPV_HeadCode);index++)			// Jeremy -
		for (index=0;index< 3;index++)			// Jeremy -
		{
			if(u8IRSwModeBuf[0] == TPV_HeadCode_Group[index].Headcode1)
			{
				if(u8IRSwModeBuf[1] ==  TPV_HeadCode_Group[index].Headcode2)
				{
					if(FactoryKeyEnable==2||TPV_HeadCode_Group[index].FactoryRC==0)
					{
						if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
						{
							*pu8Key = u8IRSwModeBuf[2];
							*pu8Flag = 0;
							bRet = TRUE;
							_u8PrevKeyCode = *pu8Key;
							_u8PrevSystemCode = *pu8System;
							_u8IRRepeateDetect = 1;
							_u8IRHeadReceived = 0;
							_u8IRType = 0;
							goto done;
						}
					}
				}
			}
		}
	}

	DEBUG_IR(printk(" invalid data\n"));
	bRet = FALSE;

	done:
	_u32IRCount = 0;
	return bRet;
}

#if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)||(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
BOOL _MDrv_IR_GetKey_TouchRC(U32 *TouchRC_Value)
{
	BOOL bRet = FALSE;
	U32 u8Bit;
	U32	temp_index;
	U8  pulse_state;
	U32	temp_value, temp_value_reverse;
	U32 data_start_pos;

	// Always return if it is NEC RC Style
	// This part is required if we need to support other RCs (NEC RC here),
	// and assumption is that other RCs packet length is longer than Touch RC
	// ps: repeat code is processed in NEC decoder before we do NEC check here.
	if (_u32IRCount<(10))
	{
		return FALSE; // data is not sufficient to determine header with RC type
	}

	//
	// NEC Stype
	//
	BOOL blLooksLikeNEC=FALSE;
	if( _u32IRData[0] > IR_HDC_LOB && _u32IRData[1] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[1] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
	temp_index=2;
        blLooksLikeNEC = TRUE;
    }
    else if( _u32IRData[1] > IR_HDC_LOB && _u32IRData[2] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[2] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
    	temp_index=3;
        blLooksLikeNEC = TRUE;
    }
    else
    {
    	blLooksLikeNEC = FALSE;
    }

	if(blLooksLikeNEC == TRUE)
	{
		for( u8Bit=0; u8Bit<6; u8Bit++)		// Check 3 bits
		{
			u32 u32BitLen = _u32IRData[u8Bit+temp_index];

			if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
			{
			}
			else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
			{
			}
			else
			{
				blLooksLikeNEC = FALSE;
			}
		}
	}
	if (blLooksLikeNEC==TRUE)
	{
		DEBUG_IR(printk("NH "));
		return FALSE; //not complete yet
	}
	//
	// END of check whether it is NEC
	//

    //
    // Check if Touch RC style Header
    //
	if( (_u32IRData[0] > IR_LEADING_TIME_LB) && (_u32IRData[1] >IR_PULSE_2_LB) && (_u32IRData[1]<IR_PULSE_4_HB) )
	{
		data_start_pos=1;
	}
	else if( ( _u32IRData[1] > IR_LEADING_TIME_LB) && (_u32IRData[2] >IR_PULSE_2_LB) && (_u32IRData[2]<IR_PULSE_4_HB) )
	{
		data_start_pos=2;
	}
	else if( ( _u32IRData[2] > IR_LEADING_TIME_LB) && (_u32IRData[3] >IR_PULSE_2_LB) && (_u32IRData[3]<IR_PULSE_4_HB) )
	{
		data_start_pos=3;
	}
	else
	{
		// Neither NON-NEC nor non-touch RC, reset

		DEBUG_IR(printk(" NN:", _u32IRCount));
		for( temp_index=0; temp_index<_u32IRCount; temp_index++)
		{
			DEBUG_IR(printk(" %d", _u32IRData[temp_index]));
		}
		_u32IRCount = 0;
		return FALSE;
	}

	//DEBUG_IR(printk("T"));

	// Testing Purpose
/*
    	DEBUG_IR(printk("Touch_Cnt=%d", _u32IRCount));
	for( temp_index=0; temp_index<_u32IRCount; temp_index++)
    	{
       		DEBUG_IR(printk(" %d", _u32IRData[temp_index]));
    	}
	// End of Testing Purpose
*/
	if (_u32IRCount<(IR_TOUCH_MINIMAL_IRCOUNT-1-data_start_pos))
	{
		// Minimal possible count number before a complete Touch RC packet is received
		return FALSE; //not complete yet
	}

	//DEBUG_IR(printk("I"));
	// Check if total data time has exceeded (one_packet-leading-single_high), leading code not included.
	temp_value= 0;
	for (temp_index=data_start_pos;temp_index<_u32IRCount;temp_index++)
	{
		temp_value+=_u32IRData[temp_index];
	}
	if(temp_value<IR_DATA_LENGTH_LO)
	{
		return FALSE; //not complete yet
	}
	// End

	//DEBUG_IR(printk("J"));
	u8Bit=29;			// Leading + D0 ~ D27
	temp_index=data_start_pos;
	temp_value=0;
	temp_value_reverse=0;
	pulse_state = FALSE;
	do
	{
		U32 u32BitLen;

		if(temp_index>=_u32IRCount) // Data are already all read-out
		{
			if((pulse_state==FALSE)&&(u8Bit==1))	// if last bit at half-pulse state
			{
				temp_value >>= 1;
				temp_value |= (1<<28);		// push 1
				temp_value_reverse <<= 1;
				temp_value_reverse |= (1);
				u8Bit--;
				pulse_state = TRUE;
			}
			else
			{
				return FALSE; //not complete yet
			}
		}
		else	// Data is available for processing
		{
			u32BitLen = _u32IRData[temp_index];
			if(pulse_state==FALSE)
			{
				if( u32BitLen > IR_PULSE_2_LB && u32BitLen < IR_PULSE_2_HB )
				{
					temp_value >>= 1;
					temp_value |= (1<<28);		// push 1
					temp_value_reverse <<= 1;
					temp_value_reverse |= (1);
					u8Bit--;
					// pulse_state = FALSE;
				}
				else if ( u32BitLen > IR_PULSE_3_LB && u32BitLen < IR_PULSE_3_HB )
				{
					temp_value >>= 2;
					temp_value |= (1<<27);		// push 1 then 0
					temp_value_reverse <<= 2;
					temp_value_reverse |= (1<<1);
					u8Bit-=2;
					pulse_state = TRUE;
				}
				else if ( u32BitLen > IR_PULSE_4_LB && u32BitLen < IR_PULSE_4_HB )
				{
					temp_value >>= 2;
					temp_value |= (1<<27);		// push 1 then 0
					temp_value_reverse <<= 2;
					temp_value_reverse |= (1<<1);
					u8Bit-=2;
					// pulse_state = FALSE;
				}
				else
				{
					// This state is possible for last bit if it is "1";
					if(u8Bit==1)	// if last bit
					{
						temp_value >>= 1;
						temp_value |= (1<<28);		// push 1
						temp_value_reverse <<= 1;
						temp_value_reverse |= (1);
						u8Bit--;
						pulse_state = TRUE;
					}
					else
					{
						// Error pulse width
						u8Bit = 0;
						pulse_state = 0xff;
					}
				}
			}
			else if(pulse_state==TRUE)
			{
				if( u32BitLen > IR_PULSE_2_LB && u32BitLen < IR_PULSE_2_HB )
				{
					temp_value >>= 1;
					// temp_value |= (0<<28);		// push 0
					temp_value_reverse <<= 1;
					//temp_value_reverse |= (0);
					u8Bit--;
					// pulse_state = TRUE;
				}
				else if( u32BitLen > IR_PULSE_3_LB && u32BitLen < IR_PULSE_3_HB )
				{
					temp_value >>= 1;
					// temp_value |= (0<<28);		// push 0
					temp_value_reverse <<= 1;
					//temp_value_reverse |= (0);
					u8Bit--;
					pulse_state = FALSE;
				}
				else
				{
					// Error pulse width
					u8Bit = 0;
					pulse_state = 0xfe;
				}
			}
			else
			{
				// Error pulse state
				u8Bit = 0;
				pulse_state = 0xfd;
			}
			temp_index++;
		} // END == Data is available for processing
	}
	while (u8Bit>0);

	if((pulse_state>=0xfd)&&(pulse_state<=0xff))		// Error pulse, discard
	{
		bRet = FALSE;
		DEBUG_IR(printk("_EP:"));

		for( temp_index=0; temp_index<_u32IRCount; temp_index++)
		{
			DEBUG_IR(printk(" %d", _u32IRData[temp_index]));
		}
		_u32IRCount = 0;

		return bRet;
	}

	//DEBUG_IR(printk("M"));

	temp_value >>=1 ; // Remove leading code at LSB
	temp_value_reverse &= 0x0fffffff; // Mask out leading code at MSB

#if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
	// The following CRC check is for TouchRC
	temp_index = calc_crc4(temp_value&0xffffff);
	if( temp_index != ((temp_value>>24)&0x0f) )
#elif(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
	temp_index = calc_crc4(temp_value_reverse>>4);
	if( temp_index != (temp_value_reverse&0x0f) )
#endif // MOUSE_IR_TYPE_SEL??
	{
		bRet = FALSE;
		DEBUG_IR(printk(" CRC_ERR "));
	}
	else
	{
		// Passed CRC4 check, covert the mouse value
		int return_x, return_y;
		BOOL button_0, button_1;


#if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
// Touch RC
		return_x = (temp_value)&0x3ff;
		return_y = (temp_value>>10)&0x3ff;
// END
#elif(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
		return_x = (temp_value_reverse>>18)&0x3ff;
		return_y = (temp_value_reverse>>8)&0x3ff;
#endif // #ifdef TPi_Touch_RC

		button_0 = (temp_value&(1<<22))!=0?TRUE:FALSE;			// "Back"?
		button_1 = (temp_value&(1<<23))!=0?TRUE:FALSE;			// "OK"?

		*TouchRC_Value =  (return_x) | (return_y<<10) | (temp_value&(0x3<<22)) | (0x3<<20);
		bRet = TRUE;

#ifdef CORE_M0			// Testing on M0 board
  #if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
		// Test uint-to-int conversion
		UARTprintf("%x:", Convert_uint_to_int(return_x));
		UARTprintf("%x", Convert_uint_to_int(return_y));
		input_report(REL_X, Convert_uint_to_int(return_x));
		input_report(BTN_LEFT, button_1);
		input_report(BTN_RIGHT, button_0);
		input_sync();
  #elif(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
		if((return_x&0x300)!=0)
		{
			UARTputchar( '0'+((return_x>>8)&0x3) );
		}
		UARTputHEX_U(return_x&0xff);
		UARTputchar(':');
		if((return_y&0x300)!=0)
		{
			UARTputchar( '0'+((return_y>>8)&0x3) );
		}
		UARTputHEX_U(return_y&0xff);
  #endif //  TPi_Touch_RC
		if(button_0)
		{
			UARTputchar('+');
		}
		else
		{
			UARTputchar('-');
		}
		if(button_1)
		{
			UARTputchar('+');
		}
		else
		{
			UARTputchar('-');
		}
		UARTputchar('\r');
		UARTputchar('\n');
#else
		DEBUG_IR(printk(" @%d:%d:",return_x,return_y));
		if(button_0)
		{
			DEBUG_IR(printk("+"));
		}
		else
		{
			DEBUG_IR(printk("-"));
		}
		if(button_1)
		{
			DEBUG_IR(printk("+\r\n"));
		}
		else
		{
			DEBUG_IR(printk("-\r\n"));
		}
#endif		// #define CORE_M0

	}

	_u32IRCount = 0;
	return bRet;
}

BOOL _MDrv_IR_GetKey_TouchRC_Reverse_Edge(U32 *TouchRC_Value)
{
	BOOL bRet = FALSE;
	U32 u8Bit;
	U32	temp_index;
	U8  pulse_state;
	U32	temp_value, temp_value_reverse;
	U32 data_start_pos;

	// Always return if it is NEC RC Style
	// This part is required if we need to support other RCs (NEC RC here),
	// and assumption is that other RCs packet length is longer than Touch RC
	// ps: repeat code is processed in NEC decoder before we do NEC check here.
	if (_u32IRCount<(10))
	{
		return FALSE; // data is not sufficient to determine header with RC type
	}

/*
	//
	// NEC Stype
	//
	BOOL blLooksLikeNEC=FALSE;
	if( _u32IRData[0] > IR_HDC_LOB && _u32IRData[1] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[1] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
	temp_index=2;
        blLooksLikeNEC = TRUE;
    }
    else if( _u32IRData[1] > IR_HDC_LOB && _u32IRData[2] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[2] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
    	temp_index=3;
        blLooksLikeNEC = TRUE;
    }
    else
    {
    	blLooksLikeNEC = FALSE;
    }

	if(blLooksLikeNEC == TRUE)
	{
		for( u8Bit=0; u8Bit<6; u8Bit++)		// Check 3 bits
		{
			u32 u32BitLen = _u32IRData[u8Bit+temp_index];

			if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
			{
			}
			else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
			{
			}
			else
			{
				blLooksLikeNEC = FALSE;
			}
		}
	}
	if (blLooksLikeNEC==TRUE)
	{
		DEBUG_IR(printk("NH "));
		return FALSE; //not complete yet
	}
	//
	// END of check whether it is NEC
	//
*/

    //
    // Check if Touch RC style Header
    //
	if( (_u32IRData[0] > IR_LEADING_TIME_LB) && (_u32IRData[1] >IR_PULSE_2_LB) && (_u32IRData[1]<IR_PULSE_4_HB) )
	{
		data_start_pos=1;
	}
	else if( ( _u32IRData[1] > IR_LEADING_TIME_LB) && (_u32IRData[2] >IR_PULSE_2_LB) && (_u32IRData[2]<IR_PULSE_4_HB) )
	{
		data_start_pos=2;
	}
	else if( ( _u32IRData[2] > IR_LEADING_TIME_LB) && (_u32IRData[3] >IR_PULSE_2_LB) && (_u32IRData[3]<IR_PULSE_4_HB) )
	{
		data_start_pos=3;
	}
	else
	{
		// Neither NON-NEC nor non-touch RC, reset

		DEBUG_IR(printk(" NN:", _u32IRCount));
		for( temp_index=0; temp_index<_u32IRCount; temp_index++)
		{
			DEBUG_IR(printk(" %d", _u32IRData[temp_index]));
		}
		_u32IRCount = 0;
		return FALSE;
	}

	//DEBUG_IR(printk("T"));

	// Testing Purpose
/*
    	DEBUG_IR(printk("Touch_Cnt=%d", _u32IRCount));
	for( temp_index=0; temp_index<_u32IRCount; temp_index++)
    	{
       		DEBUG_IR(printk(" %d", _u32IRData[temp_index]));
    	}
	// End of Testing Purpose
*/

/*
	if (_u32IRCount<(IR_TOUCH_MINIMAL_IRCOUNT-1-data_start_pos))
	{
		// Minimal possible count number before a complete Touch RC packet is received
		return FALSE; //not complete yet
	}

	//DEBUG_IR(printk("I"));
	// Check if total data time has exceeded (one_packet-leading-single_high), leading code not included.
	temp_value= 0;
	for (temp_index=data_start_pos;temp_index<_u32IRCount;temp_index++)
	{
		temp_value+=_u32IRData[temp_index];
	}
	if(temp_value<IR_DATA_LENGTH_LO)
	{
		return FALSE; //not complete yet
	}
	// End
*/

	//DEBUG_IR(printk("J"));
	u8Bit=29-1;			// D0 ~ D27, leading is set directly
	temp_index=data_start_pos;
	temp_value=(1<<28);
	temp_value_reverse=1;
	pulse_state = FALSE;
	do
	{
		U32 u32BitLen;

		if(temp_index>=_u32IRCount) // Data are already all read-out
		{
//			if((pulse_state==FALSE)&&(u8Bit==1))	// if last bit at half-pulse state
//			{
//				temp_value >>= 1;
//				temp_value |= (1<<28);		// push 1
//				temp_value_reverse <<= 1;
//				temp_value_reverse |= (1);
//				u8Bit--;
//				pulse_state = TRUE;
//			}
//			else
			{
				return FALSE; //not complete yet
			}
		}
		else	// Data is available for processing
		{
			u32BitLen = _u32IRData[temp_index];
			if(pulse_state==FALSE)
			{
				if( u32BitLen > IR_PULSE_2_LB && u32BitLen < IR_PULSE_2_HB )
				{
					temp_value >>= 1;
					temp_value |= (1<<28);		// push 1
					temp_value_reverse <<= 1;
					temp_value_reverse |= (1);
					u8Bit--;
					// pulse_state = FALSE;
				}
				else if ( u32BitLen > IR_PULSE_3_LB && u32BitLen < IR_PULSE_3_HB )
				{
					temp_value >>= 1;
					// temp_value |= (0<<28);		// push 0
					temp_value_reverse <<= 1;
					//temp_value_reverse |= (0);
					u8Bit--;
					pulse_state = TRUE;
				}
				else
				{
//					// This state is possible for last bit if it is "1";
//					if(u8Bit==1)	// if last bit
//					{
//						temp_value >>= 1;
//						temp_value |= (1<<28);		// push 1
//						temp_value_reverse <<= 1;
//						temp_value_reverse |= (1);
//						u8Bit--;
//						pulse_state = TRUE;
//					}
//					else
					{
						// Error pulse width
						u8Bit = 0;
						pulse_state = 0xff;
					}
				}
			}
			else if(pulse_state==TRUE)
			{
				if ( u32BitLen > IR_PULSE_2_LB && u32BitLen < IR_PULSE_2_HB )
				{
					temp_value >>= 1;
					// temp_value |= (0<<28);		// push 0
					temp_value_reverse <<= 1;
					//temp_value_reverse |= (0);
					u8Bit--;
					// pulse_state = TRUE;
				}
				else if ( u32BitLen > IR_PULSE_3_LB && u32BitLen < IR_PULSE_3_HB )
				{
					temp_value >>= 2;
					temp_value |= (3<<27);		// push 1 then 1
					temp_value_reverse <<= 2;
					temp_value_reverse |= (3<<1);
					u8Bit-=2;
					pulse_state = FALSE;
				}
				else if ( u32BitLen > IR_PULSE_4_LB && u32BitLen < IR_PULSE_4_HB )
				{
					temp_value >>= 2;
					temp_value |= (1<<27);		// push 1 then 0
					temp_value_reverse <<= 2;
					temp_value_reverse |= (1<<1);
					u8Bit-=2;
					pulse_state = FALSE;
				}
				else
				{
					// Error pulse width
					u8Bit = 0;
					pulse_state = 0xfe;
				}
			}
			else
			{
				// Error pulse state
				u8Bit = 0;
				pulse_state = 0xfd;
			}
			temp_index++;
		} // END == Data is available for processing
	}
	while (u8Bit>0);

	if((pulse_state>=0xfd)&&(pulse_state<=0xff))		// Error pulse, discard
	{
		bRet = FALSE;
		DEBUG_IR(printk("_EP:"));

		for( temp_index=0; temp_index<_u32IRCount; temp_index++)
		{
			DEBUG_IR(printk(" %d", _u32IRData[temp_index]));
		}
		_u32IRCount = 0;

		return bRet;
	}

	//DEBUG_IR(printk("M"));

	temp_value >>=1 ; // Remove leading code at LSB
	temp_value_reverse &= 0x0fffffff; // Mask out leading code at MSB

#if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
	// The following CRC check is for TouchRC
	temp_index = calc_crc4(temp_value&0xffffff);
	if( temp_index != ((temp_value>>24)&0x0f) )
#elif(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
	temp_index = calc_crc4(temp_value_reverse>>4);
	if( temp_index != (temp_value_reverse&0x0f) )
#endif // MOUSE_IR_TYPE_SEL??
	{
		bRet = FALSE;
		DEBUG_IR(printk(" CRC_ERR "));
	}
	else
	{
		// Passed CRC4 check, covert the mouse value
		int return_x, return_y;
		BOOL button_0, button_1;


#if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
// Touch RC
		return_x = (temp_value)&0x3ff;
		return_y = (temp_value>>10)&0x3ff;
// END
#elif(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
		return_x = (temp_value_reverse>>18)&0x3ff;
		return_y = (temp_value_reverse>>8)&0x3ff;
#endif // #ifdef TPi_Touch_RC

		button_0 = (temp_value&(1<<22))!=0?TRUE:FALSE;			// "Back"?
		button_1 = (temp_value&(1<<23))!=0?TRUE:FALSE;			// "OK"?

		*TouchRC_Value =  (return_x) | (return_y<<10) | (temp_value&(0x3<<22)) | (0x3<<20);
		bRet = TRUE;

#ifdef CORE_M0			// Testing on M0 board
  #if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
		// Test uint-to-int conversion
		UARTprintf("%x:", Convert_uint_to_int(return_x));
		UARTprintf("%x", Convert_uint_to_int(return_y));
		input_report(REL_X, Convert_uint_to_int(return_x));
		input_report(BTN_LEFT, button_1);
		input_report(BTN_RIGHT, button_0);
		input_sync();
  #elif(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
		if((return_x&0x300)!=0)
		{
			UARTputchar( '0'+((return_x>>8)&0x3) );
		}
		UARTputHEX_U(return_x&0xff);
		UARTputchar(':');
		if((return_y&0x300)!=0)
		{
			UARTputchar( '0'+((return_y>>8)&0x3) );
		}
		UARTputHEX_U(return_y&0xff);
  #endif //  TPi_Touch_RC
		if(button_0)
		{
			UARTputchar('+');
		}
		else
		{
			UARTputchar('-');
		}
		if(button_1)
		{
			UARTputchar('+');
		}
		else
		{
			UARTputchar('-');
		}
		UARTputchar('\r');
		UARTputchar('\n');
#else
		DEBUG_IR(printk(" @%d:%d:",return_x,return_y));
		if(button_0)
		{
			DEBUG_IR(printk("+"));
		}
		else
		{
			DEBUG_IR(printk("-"));
		}
		if(button_1)
		{
			DEBUG_IR(printk("+\r\n"));
		}
		else
		{
			DEBUG_IR(printk("-\r\n"));
		}
#endif		// #define CORE_M0

	}

	_u32IRCount = 0;
	return bRet;
}
#endif // #if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)||(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)

void _MDrv_IR_ISR(uint32_t wTimeStamp)
{
	uint8_t		bHaveKey = FALSE;
	uint32_t 	tmp, diff;
	uint8_t u8Key=0, u8RepeatFlag=0;
	uint8_t u8System = 0;

	tmp = _MDrv_IR_GetSystemTime();
	if (ulPreTime > tmp)
	{
		diff = 0x100000000 - ulPreTime + tmp;
	}
	else
	{
		diff = tmp - ulPreTime;
	}

	if (diff > IR_NO_SIGNAL_TIMOUT)//||_u8IRRepeated) //timeout or not handler yet
	{
		_u32IRCount = 0;
		_u8IRRepeateDetect=0;
		_u8IRRepeated=0;
		_u8IRRepeatedNum=0;
		_u8IRHeadReceived=0;
	}

	if (_u32IRCount <IR_SWDECODE_MODE_BUF_LEN)
	{
		tmp=wTimeStamp;

		// if(tmp<IR_LG0_LOB)//error signal for NEC
		if(tmp<IR_PULSE_2_LB)//error signal -- too small even for TouchRC protocol
		{
			_u32IRCount = 0;
			_u8IRRepeateDetect=0;
			_u8IRRepeated=0;
			_u8IRRepeatedNum=0;
			_u8IRHeadReceived=0;
		}

		// We must double check if the following combined condition works as we thought
		if ( ( tmp>IR_LEADING_TIME_LB ) || ( tmp>(IR_HDC_LOB)) )//Head received
		{
			_u32IRCount=0;
			_u8IRHeadReceived=1;
		}

		if(_u8IRHeadReceived)//begin data received
		{
			_u32IRData[_u32IRCount++] = tmp;
			if(_u8IRRepeateDetect)
			{
				if( tmp>(IR_OFC_RP_LOB) && tmp<(IR_OFC_RP_UPB) )
				{
					if(_u32IRCount<4)//for quickly change ir
					{
						_u8IRRepeated=1;
						_u8IRHeadReceived=0;
					}
				}
			}
		}
	}
	ulPreTime = _MDrv_IR_GetSystemTime();

	if ((bHaveKey=_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag)) != 0)
	{
		_ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();

		//temporary solution, need to implement ring buffer for this
		_KeyReceived.u8Key = u8Key;
		_KeyReceived.u8System = u8System;
		_KeyReceived.u8Flag = u8RepeatFlag;
		_KeyReceived.u8Valid = 1;
		if(u8RepeatFlag)
		{
		  UARTputstr("RTP:");
		}
		UARTputstr("NEC:");
		UARTputHEX_U(u8System&0xff);
		UARTputHEX_U(u8Key&0xff);
		UARTputstr("\r\n");
	}
	else if ((bHaveKey=_MDrv_IR_GetKey_TouchRC(&tmp)) != 0)			// _MDrv_IR_GetKey_TouchRC_Reverse_Edge  // _MDrv_IR_GetKey_TouchRC
	{
		_ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();
	}
}
#endif // #if (IR_TYPE_SEL == IR_TYPE_AOC)

//
// END
//

