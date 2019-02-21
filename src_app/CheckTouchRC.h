/*
 * CheckTouchRC.h
 *
 *  Created on: 2013/12/12
 *      Author: Jeremy.Hsiao
 */

#ifndef CHECKTOUCHRC_H_
#define CHECKTOUCHRC_H_

#define IR_TYPE_AOC                 11
#define IR_TYPE_EBONY               12

#define MOUSE_IR_TYPE_XMARTV        101
#define MOUSE_IR_TYPE_TOUCHPLUS     102
#define MOUSE_IR_TYPE_PIXART        103
//#define MOUSE_IR_TYPE_RCMM          104
//
//
//
#define IR_TYPE_SEL                 IR_TYPE_EBONY //IR_TYPE_AOC // IR_TYPE_EBONY
#define MOUSE_IR_TYPE_SEL           MOUSE_IR_TYPE_TOUCHPLUS//MOUSE_IR_TYPE_PIXART//MOUSE_IR_TYPE_TOUCHPLUS
//
//
//
typedef uint8_t 	U8;
typedef uint8_t 	u8;
typedef uint32_t 	U32;
typedef uint32_t 	u32;
typedef uint8_t		BOOL;
#define IR_RAW_DATA_NUM 4
#define IR_SWDECODE_MODE_BUF_LEN 100

#define irGetMinCnt(time, tolerance)        ((u32)(((double)time)*((double)1-(double)tolerance)))
#define irGetMaxCnt(time, tolerance)        ((u32)(((double)time)*((double)1+(double)tolerance)))
#define irGetCnt(time)                      ((u32)(time))

struct TPV_HeadCode
{
	U8 Headcode1;
	U8 Headcode2;
	U8 FactoryRC;
};

typedef struct
{
	uint8_t u8Key;
	uint8_t u8System;
	uint8_t u8Flag;
	uint8_t u8Valid;
} MS_IR_KeyInfo;

// RC5/6
typedef enum _IrSourceType
{
    IR_SOURCE_RC6 = 0,
    IR_SOURCE_RC5,

    IR_SOURCE_NULL      = 0xFF
} IrSourceType;

typedef enum _IrParityType
{
    IR_PARITY_N = 0,
    IR_PARITY_P,

    IR_PARITY_TOTAL,

}   IrParityType;

typedef enum _IrFrameHalfIdx
{
    IR_1ST_FRAME_HALF = 0,
    IR_2ND_FRAME_HALF,

    IR_TOTAL_FRAME_HALF,
}   IrFrameHalfIdx;

typedef enum _IrRC6Signal
{
    IR_SIGNAL_NULL = 0,
    IR_1T = 1,
    IR_2T,
    IR_3T,
    IR_6T,

}   IrRC6Signal;

/*
typedef enum _IrRC6Signal_Single_Sided
{
    IR_SIGNAL_SINGLE_SIDED_NULL = 0,
    IR_SINGLE_SIDED_2T = 1,
    IR_SINGLE_SIDED_3T,
    IR_SINGLE_SIDED_4T,
    IR_SINGLE_SIDED_5T,
    IR_SINGLE_SIDED_6T,
}   IrRC6Signal_Single_Side;
*/

typedef enum _IRRC6State
{
    IR_RC6_IDLE= 0,
    IR_RC6_LEADER_6T,
    IR_RC6_LEADER_2T,
    IR_RC6_DATA,
    IR_RC6_FAIL,
    IR_RC6_FINISH,

}   IRRC6State;

typedef enum _IRRC6DataState
{
    IR_RC6_DATA_IDLE = 0,
    IR_RC6_DATA_TRAIL,
    IR_RC6_DATA_INSERT,
    IR_RC6_DATA_RESET,

}   IRRC6DataState;

typedef enum _IRRC6DataTrailState
{
    IR_RC6_DATA_TRAIL_IDLE = 0,
    IR_RC6_DATA_TRAIL_RECEIVE = 0x01,
    IR_RC6_DATA_TRAIL_INSERT = 0x02,
    IR_RC6_DATA_TRAIL_I_R = 0x03,

}   IRRC6DataTrailState;

typedef enum _IrCommandType     //  Follow the table, use decimal
{
    IRKEY_POWER             = 0x0C,

    IRKEY_PLAY              = 0x2C,
    IRKEY_PAUSE             = 0x30,
    IRKEY_RECORD            = 0x37,
    IRKEY_STOP              = 0x31,
    IRKEY_FORWARD           = 0x28,
    IRKEY_BACKWARD          = 0x2B,

    IRKEY_INPUT_SOURCE      = 0x38,
    IRKEY_TV_INPUT          = 0x9F,      // TV

    IRKEY_EXIT              = 0x41,
    IRKEY_BACK              = 0x0A,

    IRKEY_3D                = 0xBA,
    IRKEY_EPG               = 0xCC,      //  Guide
    IRKEY_MENU              = 0x54,       //  Home Key

  	IRKEY_MIX               = 0x69,

    IRKEY_CHANNEL_LIST      = 0xD2,
    IRKEY_INFO              = 0x0F,
    IRKEY_ZOOM              = 0xF5,


    IRKEY_OPTION            = 0x40,       //  connect to subpage

    IRKEY_UP                = 0x58,
    IRKEY_DOWN              = 0x59,
    IRKEY_LEFT              = 0x5A,
    IRKEY_RIGHT             = 0x5B,
    IRKEY_SELECT            = 0x5C,

    IRKEY_CH_PLUS           = 0x4C,       //  Same with Channel +
    IRKEY_CH_MINUS          = 0x4D,       //  Same with Channel -

    IRKEY_VOLUME_PLUS       = 0x10,
    IRKEY_VOLUME_MINUS      = 0x11,
    IRKEY_MUTE              = 0x0D,

    IRKEY_RED               = 0x6D,
    IRKEY_GREEN             = 0x6E,
    IRKEY_YELLOW            = 0x6F,
    IRKEY_BLUE              = 0x70,

    IRKEY_NUM_0             = 0x00,
    IRKEY_NUM_1             = 0x01,
    IRKEY_NUM_2             = 0x02,
    IRKEY_NUM_3             = 0x03,
    IRKEY_NUM_4             = 0x04,
    IRKEY_NUM_5             = 0x05,
    IRKEY_NUM_6             = 0x06,
    IRKEY_NUM_7             = 0x07,
    IRKEY_NUM_8             = 0x08,
    IRKEY_NUM_9             = 0x09,

    IRKEY_SUBTITLE          = 0x4B,
    IRKEY_SMART_TV          = 0xBE, //  Smart TV
    IRKEY_TTX               = 0x3C,
    IRKEY_SETUP             = 0xBF, //MVK_SETUP   //Joy added 2013.08.14

    F_IRKEY_TEST            = 0xA0,
    F_IRKEY_RST             = 0xA1,
    F_IRKEY_FAC             = 0xA2,
    F_IRKEY_CSM             = 0xA3,
    F_IRKEY_PATTERN         = 0xA4,
    F_IRKEY_1               = 0xA5,
    F_IRKEY_2               = 0xA6,
    F_IRKEY_3               = 0xA7,
    F_IRKEY_4               = 0xA8,
    F_IRKEY_5               = 0xA9,
    F_IRKEY_6               = 0xAA,
    F_IRKEY_7               = 0xAB,
    F_IRKEY_8               = 0xAC,
    F_IRKEY_9               = 0xAD,
    F_IRKEY_0               = 0xAE,
    F_IRKEY_ANTENNA_CABLE   = 0xAF,
    F_IRKEY_PRE_CH          = 0xB0,
    F_IRKEY_VOL_MAX         = 0xB1,
    F_IRKEY_VOL_BUZZ        = 0xB2,
    F_IRKEY_CTC             = 0xB3,
    F_IRKEY_MENU            = 0xB4,
    F_IRKEY_CH_PLUS         = 0xB5,
    F_IRKEY_CH_MINUS        = 0xB6,
    F_IRKEY_B_I             = 0xB7,
    F_IRKEY_CH_SCAN         = 0xB8,
    F_IRKEY_UP              = 0xB9,
    F_IRKEY_DOWN            = 0xBB,
    F_IRKEY_ENTER           = 0xBC,
    F_IRKEY_VOL_L           = 0xBD,
    F_IRKEY_VOL_R           = 0xDB, // 0xBF->0xDB //0xBF->MVK_SETUP //Joy modified 2013.08.14; For New IR REmote Controller
    F_IRKEY_CC_TT           = 0xC0,
    F_IRKEY_MUTE            = 0xC1,
    F_IRKEY_PIC             = 0xC2,
    F_IRKEY_AUDIO           = 0xC3,
    F_IRKEY_LOG_LED         = 0xC4,
    F_IRKEY_2D_3D           = 0xC5,
    F_IRKEY_ARC             = 0xC6,
    F_IRKEY_CI_PLUS         = 0xC7,
    F_IRKEY_VIRGIN          = 0xC8,
    F_IRKEY_CVBS            = 0xC9,
    F_IRKEY_YPBPR_SCART     = 0xCA,
    F_IRKEY_HDMI            = 0xCB,
    F_IRKEY_VGA             = 0xCD,
    F_IRKEY_REGIN           = 0xCE,
    F_IRKEY_CLONE           = 0xCF,
    F_IRKEY_CLK             = 0xD0,
    F_IRKEY_DCR             = 0xD1,
    F_IRKEY_ADC             = 0xD3,
    F_IRKEY_BLK             = 0xD4,
    F_IRKEY_W_P             = 0xD5,
    F_IRKEY_LIGHT_SENSOR    = 0xD6,
    F_IRKEY_USB             = 0xD7,
    F_IRKEY_RJ45            = 0xD8,
    F_IRKEY_RS232           = 0xD9,
    F_IRKEY_EMPTY           = 0xDA,

//  Below are Useless Keys
}IrCommandType;

typedef enum
{
    E_IR_KEY_PROPERTY_INIT,
    E_IR_KEY_PROPERTY_1st,
    E_IR_KEY_PROPERTY_FOLLOWING
} IRKeyProperty;
// RC5/6

#if (IR_TYPE_SEL == IR_TYPE_AOC)
// IR Timing define for NEC
#define IR_HEADER_CODE_TIME     9000    // us		// leading/repeat code high time
#define IR_OFF_CODE_TIME        4500    // us		// leading code low time
#define IR_OFF_CODE_RP_TIME     2810    // us		// repeat code low time
#define IR_LOGI_01H_TIME        560     // us	// Data 0/1 pulse time (high time)
#define IR_LOGI_0_TIME          1120    // us		// Data 0 total time
#define IR_LOGI_1_TIME          2250    // us		// Data 1 total time
#define IR_TIMEOUT_CYC          140000  // us

#define IR_RP_TIMEOUT           irGetCnt(IR_TIMEOUT_CYC)
#define IR_HDC_UPB              irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB              irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB              irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB              irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB           irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB           irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB            irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB            irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB              irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB              irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB              irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB              irGetMinCnt(IR_LOGI_1_TIME, 0.2)
// End NEC


// IR Definition for Touch RC
#define IR_NO_SIGNAL_TIMOUT_TIME	(140000)
#define IR_LEADING_TIME_MID_TIME	(9746)
#define IR_PULSE_1_MID_TIME			(263)
#define IR_LEADING_TIME_LB_TIME		(IR_LEADING_TIME_MID_TIME-(IR_LEADING_TIME_MID_TIME*1/10))
#define IR_LEADING_TIME_HB_TIME		(IR_LEADING_TIME_MID_TIME+(IR_LEADING_TIME_MID_TIME*1/10))
#define IR_PULSE_1_LB_TIME			(IR_PULSE_1_MID_TIME-(IR_PULSE_1_MID_TIME*2/10))
#define IR_PULSE_1_HB_TIME			(IR_PULSE_1_MID_TIME+(IR_PULSE_1_MID_TIME*2/10))
#define IR_PULSE_2_MID_TIME			(IR_PULSE_1_MID_TIME*2)
#define IR_PULSE_3_MID_TIME			(IR_PULSE_1_MID_TIME*3)
#define IR_PULSE_4_MID_TIME			(IR_PULSE_1_MID_TIME*4)
#define IR_PACKET_LENGTH_TIME       (25000)

#if(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_TOUCHPLUS)
 #define IR_DATA_LENGTH_LO_TIME      (IR_PACKET_LENGTH_TIME-(IR_LEADING_TIME_MID_TIME)-(IR_PULSE_1_HB_TIME*8))
#elif(MOUSE_IR_TYPE_SEL==MOUSE_IR_TYPE_XMARTV)
 #define IR_DATA_LENGTH_LO_TIME      (IR_PACKET_LENGTH_TIME-(IR_LEADING_TIME_MID_TIME)-(IR_PULSE_1_HB_TIME))
#endif // MOUSE_IR_TYPE_SEL==???

#define IR_TOUCH_MINIMAL_IRCOUNT    ((IR_PACKET_LENGTH_TIME-IR_LEADING_TIME_MID_TIME)/IR_PULSE_4_MID_TIME)

#define IR_NO_SIGNAL_TIMOUT		irGetCnt(IR_NO_SIGNAL_TIMOUT_TIME)
#define IR_LEADING_TIME_MID		irGetCnt(IR_LEADING_TIME_MID_TIME)
#define IR_PULSE_1_MID			irGetCnt(IR_PULSE_1_MID_TIME)
#define IR_PULSE_2_MID			irGetCnt(IR_PULSE_2_MID_TIME)
#define IR_PULSE_3_MID			irGetCnt(IR_PULSE_3_MID_TIME)
#define IR_PULSE_4_MID			irGetCnt(IR_PULSE_4_MID_TIME)

#define IR_LEADING_TIME_LB		irGetMinCnt(IR_LEADING_TIME_MID_TIME,0.1)
#define IR_LEADING_TIME_HB		irGetMaxCnt(IR_LEADING_TIME_MID_TIME,0.1)
#define IR_PULSE_1_LB			irGetMinCnt(IR_PULSE_1_MID_TIME,0.2)
#define IR_PULSE_1_HB			irGetMaxCnt(IR_PULSE_1_MID_TIME,0.2)
#define IR_PULSE_2_LB			irGetMinCnt(IR_PULSE_2_MID_TIME,0.2)
#define IR_PULSE_2_HB			irGetMaxCnt(IR_PULSE_2_MID_TIME,0.2)
#define IR_PULSE_3_LB			irGetMinCnt(IR_PULSE_3_MID_TIME,0.2)
#define IR_PULSE_3_HB			irGetMaxCnt(IR_PULSE_3_MID_TIME,0.2)
#define IR_PULSE_4_LB			irGetMinCnt(IR_PULSE_4_MID_TIME,0.2)
#define IR_PULSE_4_HB			irGetMaxCnt(IR_PULSE_4_MID_TIME,0.2)

#define IR_PACKET_LENGTH		irGetCnt(IR_PACKET_LENGTH_TIME)
#define IR_DATA_LENGTH_LO		irGetCnt(IR_DATA_LENGTH_LO_TIME)
// END

#endif // #if (IR_TYPE_SEL == IR_TYPE_AOC)

#if (IR_TYPE_SEL == IR_TYPE_EBONY)

typedef enum _TouchRCParityType
{
	TOUCHRC_PARITY_N = 0,
	TOUCHRC_PARITY_P,
	TOUCHRC_PARITY_TOTAL,
	TOUCHRC_PARITY_NN,		// NN & NP is for testing purpose only
    TOUCHRC_PARITY_NP,
}   TouchRCParityType;

typedef enum _TouchRCFrameHalfIdx
{
	TOUCHRC_1ST_FRAME_HALF = 0,
	TOUCHRC_2ND_FRAME_HALF,
	TOUCHRC_TOTAL_FRAME_HALF,
}   TouchRCFrameHalfIdx;

typedef enum _TouchRCSignal
{
    TOUCHRC_SIGNAL_NULL = 0,
    TOUCHRC_1T = 1,
    TOUCHRC_2T,
    TOUCHRC_3T,
    TOUCHRC_4T,
    TOUCHRC_5T,
    TOUCHRC_6T,
	TOUCHRC_LEADING_SIGNAL,
}   TouchRCSignal;

typedef enum _TouchRCState
{
	TOUCHRC_IDLE= 0,
	TOUCHRC_LEADER_BIT,
	TOUCHRC_DATA,
	TOUCHRC_FAIL,
	TOUCHRC_FINISH,
}   TouchRCState;

typedef enum _TouchRCDataState
{
	TOUCHRC_DATA_IDLE = 0,
	TOUCHRC_DATA_TRAIL,
	TOUCHRC_DATA_INSERT,
	TOUCHRC_DATA_RESET,

}   TouchRCDataState;

// Begin RC5/6
#define IR_INT_NP_EDGE_TRIG
#define IR_MODE_SEL             IR_TYPE_SWDECODE_MODE
// IR Header code define
#define IR_HEADER_CODE0         0x80    // Custom 0
#define IR_HEADER_CODE1         0x7F    // Custom 1

// IR Timing define
#define IR_HEADER_CODE_TIME     2664    // us
#define IR_OFF_CODE_TIME        4500    // us
#define IR_OFF_CODE_RP_TIME     2500    // us
#define IR_LOGI_01H_TIME        560     // us
#define IR_LOGI_0_TIME          1120    // us
#define IR_LOGI_1_TIME          2240    // us

#define IR_TIMEOUT_CYC          40000   // us

#define IR_CHK_REPAT_TIME       250000  // us
#define IR_CHK_NOISE_TIME       130000  //us

#define T1_MAX  658
#define T1_MIN  250
#define T2_MAX  1124
#define T2_MIN  672
#define T3_MAX  1573
#define T3_MIN  1126
#define T6_MAX  3360
#define T6_MIN  2179

#define IR_RC6_HEAD_LEN             5
#define IR_RC6_CONT_START_BIT       IR_RC6_HEAD_LEN
#define IR_RC6_CONT_DATA_LEN        8
#define IR_RC6_INFO_START_BIT       (IR_RC6_CONT_START_BIT+IR_RC6_CONT_DATA_LEN)
#define IR_RC6_INFO_DATA_LEN        8
#define IR_RC6_TOTAL_LEN            (IR_RC6_HEAD_LEN+IR_RC6_CONT_DATA_LEN+IR_RC6_INFO_DATA_LEN)

#define KEYCODE_DUMMY 			0xff
// IR Format define
#define IRKEY_DUMY              0xFF
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER

#define IR_RP_TIMEOUT           irGetCnt(IR_TIMEOUT_CYC)

// IR Definition for Touch RC
#define IR_NO_TOUCH_SIGNAL_TIMEOUT_TIME  	(1000000)	// 1 sec - guess value
#define IR_LEADING_TIME_MID_TIME			(9746)
#define IR_PULSE_1_MID_TIME					(263)
#define IR_PULSE_2_MID_TIME					(IR_PULSE_1_MID_TIME*2)
#define IR_PULSE_3_MID_TIME					(IR_PULSE_1_MID_TIME*3)
#define IR_PULSE_4_MID_TIME					(IR_PULSE_1_MID_TIME*4)

#define IR_NO_TOUCH_SIGNAL_TIMEOUT			irGetCnt(IR_NO_TOUCH_SIGNAL_TIMEOUT_TIME)
#define IR_NO_SIGNAL_TIMOUT					irGetCnt(IR_NO_SIGNAL_TIMOUT_TIME)
#define IR_LEADING_TIME_MID					irGetCnt(IR_LEADING_TIME_MID_TIME)
#define IR_PULSE_1_MID						irGetCnt(IR_PULSE_1_MID_TIME)
#define IR_PULSE_2_MID						irGetCnt(IR_PULSE_2_MID_TIME)
#define IR_PULSE_3_MID						irGetCnt(IR_PULSE_3_MID_TIME)
#define IR_PULSE_4_MID						irGetCnt(IR_PULSE_4_MID_TIME)

#define IR_LEADING_TIME_LB					irGetMinCnt(IR_LEADING_TIME_MID_TIME,0.2)
#define IR_LEADING_TIME_HB					irGetMaxCnt(IR_LEADING_TIME_MID_TIME,0.2)
#define IR_PULSE_1_LB						irGetMinCnt(IR_PULSE_1_MID_TIME,0.3)
#define IR_PULSE_1_HB						irGetMaxCnt(IR_PULSE_1_MID_TIME,0.3)
#define IR_PULSE_2_LB						irGetMinCnt(IR_PULSE_2_MID_TIME,0.2)
#define IR_PULSE_2_HB						irGetMaxCnt(IR_PULSE_2_MID_TIME,0.2)
#define IR_PULSE_3_LB						irGetMinCnt(IR_PULSE_3_MID_TIME,0.2)
#define IR_PULSE_3_HB						irGetMaxCnt(IR_PULSE_3_MID_TIME,0.2)
#define IR_PULSE_4_LB						irGetMinCnt(IR_PULSE_4_MID_TIME,0.2)
#define IR_PULSE_4_HB						irGetMaxCnt(IR_PULSE_4_MID_TIME,0.2)

/*
// Not used in RC5/6
#define IR_HDC_UPB              irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB              irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB              irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB              irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB           irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB           irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB            irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB            irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB              irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB              irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB              irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB              irGetMinCnt(IR_LOGI_1_TIME, 0.2)
*/

//
// RCMM defines
//
//////////////////////////////////////////////////////////////////
#define RC_MODE 0x01
#define MOUSE_MODE 0x02
#define KEYBOARD_MODE 0x04
#define JOYSTICK_MODE 0x08
#define RC6_MODE_0 0x00
#define RC6_MODE_1A 0x01
#define RC6_MODE_2A 0x02
#define RC6_MODE_6ALOWPOWER 0x03
#define RC6_MODE_6AOEMCMD 0x04
#define RC6_MODE_6AOEMSTR 0x05
#define RC6_MODE_6B 0x06
#define RCMMBASIC_MODE 0x10
#define RCMMOEM_SHORTID_MODE 0x20
#define RCMMOEM_LONGID_MODE 0x40
#define RC6_MODE_FLAG 0x80 //short OEM command mode RC, and RC6+ Keyboard, mouse mode
#define RC6_HEADERLEN 8
#define RC6_MODE_0_DATALEN 16
#define RC6_MODE_0_FRAMELEN (RC6_HEADERLEN + RC6_MODE_0_DATALEN -1)
#define RC6_MODE_1A_DATALEN 32
#define RC6_MODE_1A_FRAMELEN (RC6_HEADERLEN + RC6_MODE_1A_DATALEN -1)
#define RC6_MODE_2A_DATALEN 32
#define RC6_MODE_2A_FRAMELEN (RC6_HEADERLEN + RC6_MODE_2A_DATALEN -1)
#define RC6_MODE_6ALOWPOWER_DATALEN 24
#define RC6_MODE_6ALOWPOWER_FRAMELEN (RC6_HEADERLEN + RC6_MODE_6ALOWPOWER_DATALEN -1)
#define RC6_MODE_6AOEMCMD_DATALEN 32
#define RC6_MODE_6AOEMCMD_FRAMELEN (RC6_HEADERLEN + RC6_MODE_6AOEMCMD_DATALEN -1)
#define RC6_MODE_6AOEMSTR_DATALEN 40
#define RC6_MODE_6AOEMSTR_FRAMELEN (RC6_HEADERLEN + RC6_MODE_6AOEMSTR_DATALEN -1)
#define RC6_MODE_6B_DATALEN 32
#define RC6_MODE_6B_FRAMELEN (RC6_HEADERLEN + RC6_MODE_6B_DATALEN -1)
/****************Timing frame for RCMM (us)*****************************
!* *
!* RCMM Timing Aspects: *
! -------------------------------- *
!* Standard 6 carrier periods ONTIME = 167 us nominal. *
!* *
!* A "0-0" sequence is defined as: *
!* 6 periods ONTIME and 10 periods OFFTIME *
!* *
!* A "0-1" sequence is defined as: *
!* 6 periods ONTIME and 16 periods OFFTIME *
!* *
!* A "1-0" sequence is defined as: *
!* 6 periods ONTIME and 22 periods OFFTIME *
!* *
!* A "1-1" sequence is defined as: *
!* 6 periods ONTIME and 28 periods OFFTIME *
!* *
!* Leader 15 carrier periods ONTIME = 417 us nominal. *
!* *
!* In the program comment we refer these pulse width-sequences as follows: *
!* *
!* Data ONTIME 6p. *
!* * ____
!* * | | |
!* * | | |
!* * _| |___________|
!*
!* < p6 >
!*
!* p6 = 167 us *
!* p6_max = 275 us *
!* p6_min = 80 us *
!* *
!* The 0-0 sequence timing *
!* *
!* * ____
!* * | | |
!* * | | |
!* * _| |_______________|
!*
!* < p6 >< p10 >
!* < p16 >
!* *
!* p16 (P6 + P10) = 444 us *
!* p16_max = 502 us *
!* p16_min = 386 us *
!* *
!* The 0-1 sequence timing *
!* *
!* * ____
!* * | | |
!* * | | |
!* * _| |______________________ |
!*
!* < p6 >< p16 >
!* < p22 >
!* *
!* p22 (P6 + P16) = 611 us *
!* p22_max = 669 us *
!* p22_min = 553 us *
!* *
!* The 1-0 sequence timing *
!* *
!* *
!* * ____
!* * | | |
!* * | | |
!* * _| |________________________ |
!*
!* < p6 >< p22 >
!* < p28 >
!*
!* p28 *
!* *
!* p28 (P6 + P22) = 778 us *
!* p28_max = 836 us *
!* p28_min = 720 us *
!*
************************************************/
#define P15L_MIN 330 //330
#define P15L_MAX 583 //583
//#define P25_MIN 636 //636
//#define P25_MAX 752 //752
#define P6_MIN 80 //80
#define P6_MAX 275 //275
//#define P16_MIN 386 //386
//#define P16_MAX 502 //502
//#define P22_MIN 553 //553
//#define P22_MAX 669 //669
//#define P28_MIN 720 //720
//#define P28_MAX 836 //836
//#define P34_MIN 886 //886
//#define P34_MAX 1002 //1002
/***************Timing frame for RC6 (us)********************************
* RC6 Timing Aspects: *
* *
* Data bit-time = 888 usec. *
* A "0" is defined as a 0 1 transition in the middle of a bit-time. *
* A "1" is defined as a 1 0 transition in the middle of a bit-time. *
* *
* In the program comment we refer these 2 pulse-widths as follows: *
* 1T = 444 usec, *
* 2T = 888 usec. *
* *
* When the pulses are 1T long *
* *
* _______
* |
* |_______
*
* |< >|< >|
* T1 T1
* *
* T1 = 444 us *
* t1_max = 658 us *
* t1_min = 250 us *
*
* When the pulses are 2T long *
* *
* _______________
* |
* |_______________
*
* |< >|< >|
* T2 T2
*
*
* T2 = 888 us *
* t2_max = 1124 us *
* t2_min = 672 us *
* *
* When the pulses are 3T long (when the trailer bit is received) *
* *
*
* ________________
* |
* |_______________
*
* |< >|< >|
* T3 T3
*
* T3 = 1333 us *
* t3_max = 1573 us *
* t3_min = 1126 us *
* *
*
*
*
* RC6 leader bit *
* *
* ________________
* |
* |________
*
* |< >|< >|
* T6 T2
* *
* T6 = 2664 us *
* t6_max = 3360 us *
* t6_min = 2179 us *
* *
* The signal free time of RC6 has a minimum value of t6_max *
* *
***********************************************************************************/
/*
#define T6_MIN 2178
#define T6_MAX 3360
#define T1_MIN 249
#define T1_MAX 658
#define T2_MIN 671
#define T2_MAX 1124
#define T3_MIN 1125
#define T3_MAX 1573
*/
#define TSEP_RC6M6A 400 //391.6ms
#define TREP_RC6M6A 110 //106.7ms
#define TREP_RC6PLUS 40 //40ms for RC6+ for keyboard and mouse
//#define RCMMOEM_KB_AUTOBREAK_TIME 350 //350ms
#define RCMMOEM_KB_AUTOBREAK_TIME 165 //165ms
#define RCMMOEM_RC_AUTOBREAK_TIME 165
#define RCMMBASIC_KB_AUTOBREAK_TIME 165 //165ms
#define RCMMBASIC_RC_AUTOBREAK_TIME 165
///////////////////////
#define P25_MIN 618
#define P25_MAX 658
#define P16_MIN 300
#define P16_MAX 507
#define P22_MIN 537
#define P22_MAX 627
#define P28_MIN 671
#define P28_MAX 791
#define P34_MIN 825
#define P34_MAX 1105
////////////////////////

#define IRDATA_FRAME_LEN    6
typedef struct IRDATA_FRAME
{
    unsigned char Mode;
    unsigned char Data[IRDATA_FRAME_LEN];
}IRDATA_FRAME;

#define RCDATA_BUFLEN 6


//-------------------------------------------------------------------------------------------
// Customer IR Specification parameter define (Please modify them by IR SPEC)
//-------------------------------------------------------------------------------------------
#define IR_INT_NP_EDGE_TRIG
#define IR_MODE_SEL             IR_TYPE_SWDECODE_MODE
// IR Header code define
#define IR_HEADER_CODE0         0x80    // Custom 0
#define IR_HEADER_CODE1         0x7F    // Custom 1

// IR Timing define
//#define IR_HEADER_CODE_TIME     9000    // us
#define IR_OFF_CODE_TIME        4500    // us
#define IR_OFF_CODE_RP_TIME     2500    // us
#define IR_LOGI_01H_TIME        560     // us
#define IR_LOGI_0_TIME          1120    // us
#define IR_LOGI_1_TIME          2240    // us
//#define IR_TIMEOUT_CYC          160000  // us


// IR Format define
#define IRKEY_DUMY              0xFF
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER

/*
typedef enum _IrCommandType
{
    IRKEY_TV_RADIO          = 0x0C,
    IRKEY_CHANNEL_LIST      = 0x10,
    IRKEY_CHANNEL_FAV_LIST  = 0x08,
    IRKEY_CHANNEL_RETURN    = 0x5C,
    IRKEY_CHANNEL_PLUS      = 0x1F,
    IRKEY_CHANNEL_MINUS     = 0x19,

    IRKEY_AUDIO             = 0x44,
    IRKEY_VOLUME_PLUS       = 0x16,
    IRKEY_VOLUME_MINUS      = 0x15,

    IRKEY_UP                = 0x52,
    IRKEY_POWER             = 0x46,
    IRKEY_EXIT              = 0x1B,
    IRKEY_MENU              = 0x07,
    IRKEY_DOWN              = 0x13,
    IRKEY_LEFT              = 0x06,
    IRKEY_SELECT            = 0x0F,
    IRKEY_RIGHT             = 0x1A,

    IRKEY_NUM_0             = 0x50,
    IRKEY_NUM_1             = 0x49,
    IRKEY_NUM_2             = 0x55,
    IRKEY_NUM_3             = 0x59,
    IRKEY_NUM_4             = 0x4D,
    IRKEY_NUM_5             = 0x51,
    IRKEY_NUM_6             = 0x5D,
    IRKEY_NUM_7             = 0x48,
    IRKEY_NUM_8             = 0x54,
    IRKEY_NUM_9             = 0x58,

    IRKEY_MUTE              = 0x5A,
    IRKEY_PAGE_UP           = 0x03,
    IRKEY_PAGE_DOWN         = 0x05,
    IRKEY_CLOCK             = 0x5F,

    IRKEY_INFO              = 0x14,
    IRKEY_RED               = 0x47,
    IRKEY_GREEN             = 0x4B,
    IRKEY_YELLOW            = 0x57,
    IRKEY_BLUE              = 0x5B,
    IRKEY_MTS               = 0x41,
    IRKEY_NINE_LATTICE      = IRKEY_DUMY,
    IRKEY_TTX               = 0x0A,
    IRKEY_CC                = 0x09,
    IRKEY_INPUT_SOURCE      = 0x04,
    IRKEY_CRADRD            = IRKEY_DUMY-1,
//    IRKEY_PICTURE           = 0x40,
    IRKEY_ZOOM              = 0x4C,
    IRKEY_DASH              = 0x4E,
    IRKEY_SLEEP             = 0x45,
    IRKEY_EPG               = 0x4A,
    IRKEY_PIP               = 0x40,

  	IRKEY_MIX               = 0x1C,
    IRKEY_INDEX             = 0x18,
    IRKEY_HOLD              = 0x00,
    IRKEY_PREVIOUS          = 0x0E,
    IRKEY_NEXT              = 0x12,
    IRKEY_BACKWARD          = 0x02,
    IRKEY_FORWARD           = 0x1E,
    IRKEY_PLAY              = 0x01,
    IRKEY_RECORD            = 0x0D,
    IRKEY_STOP              = 0x11,
    IRKEY_PAUSE             = 0x1D,

    IRKEY_SIZE              = 0x43,
    IRKEY_REVEAL            = 0x4F,
    IRKEY_SUBCODE           = 0x53,
}IrCommandType;
*/

//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// IR system parameter define for H/W setting (Please don't modify them)
//-------------------------------------------------------------------------------------------
// 90Mhz
#define IR_RP_TIMEOUT           irGetCnt(IR_TIMEOUT_CYC)
#define IR_HDC_UPB              irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB              irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB              irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB              irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB           irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB           irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB            irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB            irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB              irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB              irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB              irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB              irGetMinCnt(IR_LOGI_1_TIME, 0.2)
//
// End of RCMM defines
//

// End RC5/6
#endif // #if (IR_TYPE_SEL == IR_TYPE_EBONY)

extern U8 calc_crc4( U32 nData );
extern BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag);
extern void _MDrv_IR_ISR(uint32_t wTimeStamp);

#endif /* CHECKTOUCHRC_H_ */
