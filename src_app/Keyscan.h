
#ifndef _KEYSCAN_H
#define _KEYSCAN_H
//#include "mcu.h"

//define ADC Port
#define  Port0           0
#define  Port1           1
#define  Port2           2
#define  Port3           3


#define   BITS_PER_RC5              14		//	14 bits/rc5 code

//Front	Key	DC value
#define	FK_DCV_UP					0x01//0x02	//KEY_CHANNEL_PLUS
#define	FK_DCV_DOWN					0x02//0x10	//CHANNEL_DOWN
#define	FK_DCV_LEFT					0x04//0x01	//VOLUME_DOWN_
#define	FK_DCV_RIGHT				0x08//0x08	//KEY_VOLUME_PLUS;
#define	FK_DCV_SOFT_START			0x10//0x04	//SOFTSTART_K;
#define	FK_DCV_SOURCE				0x20//0x03	//KEY_AV_SOURCE;
#define	FK_DCV_ENTER				0x40//0x12	//KEY_MENU;
#define	FK_DCV_PIP				    0x80//0x12	//KEY_MENU;
#define	FK_DCV_FACTORY				0x42//0x09	//KEY_FACTORY;
#define	FK_DCV_UPDOWN				0x03//		//FK_DCV_UPDOWN;
//Key pin define

//
/* KeyCode */
#define KEY_NULL                    -1

#define	KEY_0						0xA0  //0x00 | A0
#define	KEY_1						0xA1  //0x01 | A0
#define	KEY_2						0xA2  //0x02 | A0
#define	KEY_3						0xA3  //0x03 | A0
#define	KEY_4						0xA4  //0x04 | A0
#define	KEY_5						0xA5  //0x05 | A0
#define	KEY_6						0xA6  //0x06 | A0
#define	KEY_7						0xA7  //0x07 | A0
#define	KEY_8						0xA8  //0x08 | A0
#define	KEY_9						0xA9  //0x09 | A0


#define	KEY_CHANNEL_PLUS			0x20
#define	KEY_CHANNEL_MINUS			0x21

#define	KEY_VOLUME_PLUS			    0x10
#define	KEY_VOLUME_MINUS			0x11

#define	KEY_AV_SOURCE				0x38
#define	KEY_PAD_SOURCE			    0x39

#define	KEY_SLEEP_TIMER			    0x26
#define	KEY_STANDBY				    0x0C
#define	KEY_MUTE					0x0D
#define	KEY_SMART_SOUND 			0xCB
#define	KEY_SMART_PICTURE 		    0xCA

#define	KEY_DUAL 					0x23
#define	KEY_PP 					    0x22 
#define	KEY_VIDEOFORMAT			    0x3E  //0xFE  //203E |D0
#define KEY_CC  		     		0x3A
#define	KEY_MENU 					0xD2  //0x12 | 0xD0

#define	KEY_UP 					    0xD0  //0x10 | 0xD0
#define	KEY_DOWN 					0xD1  //0x11 | 0xD0
#define	KEY_LEFT 					0xD5  //0x15 | 0xD0
#define	KEY_RIGHT 				    0xD6  //0x16 | 0xD0
//#define	KEY_OSD_LIST 				0xD7  //0x17 | 0xD0
#define	KEY_OK		 				0xD7  //0x17 | 0xD0
#define	KEY_FREEZE		 			0xDD  //0x1D | 0xD0
#define	KEY_PCTV 					0xDC  //0x0C | 0xD0
#define KEY_STATUS					0xDF  //0x0F | 0xD0

#define	KEY_TELETEXT_ON_OFF 		0x3C
#define	KEY_TELETEXT_ENLARGE 		0x2B
#define	KEY_TELETEXT_HOLD 			0x29
#define	KEY_TELETEXT_MIX 			0x2E
#define	KEY_TELETEXT_REVEAL 		0x2C
#define	KEY_TELETEXT_INDEX 			0x0F
#define	KEY_TELETEXT_R 			    0xFB  //0x2B | 0xD0
#define	KEY_TELETEXT_G 			    0xFC  //0x2C | 0xD0
#define	KEY_TELETEXT_Y 			    0xFD  //0x2D | 0xD0
#define	KEY_TELETEXT_C 			    0xFE  //0x2E | 0xD0
#define	KEY_TELETEXT_0				0x60
#define	KEY_TELETEXT_1				0x61
#define	KEY_TELETEXT_2				0x62
#define	KEY_TELETEXT_3				0x63
#define	KEY_TELETEXT_4				0x64
#define	KEY_TELETEXT_5				0x65
#define	KEY_TELETEXT_6				0x66
#define	KEY_TELETEXT_7				0x67
#define	KEY_TELETEXT_8				0x68
#define	KEY_TELETEXT_9				0x69
#define	KEY_TELETEXT_UP				0x6A
#define	KEY_TELETEXT_DOWN			0x6B

#define	KEY_FACTORY					0x50
#define KEY_SOFT_POWER				0x51
#define KEY_OSD_LOCK				0x52
#define	KEY_AUTO					0x53
#define	KEY_AGING					0x54
#define	KEY_UPDOWN					0x57



#define KEY_PIP_SIZE               0x55//0x2028
#define KEY_PIP_POSITION           0x56//0x2019
/***********************************************///RCKeyCode
#define RC_CHANNEL_K_0              0x3000
#define RC_CHANNEL_K_1              0x3001
#define RC_CHANNEL_K_2              0x3002
#define RC_CHANNEL_K_3              0x3003
#define RC_CHANNEL_K_4              0x3004
#define RC_CHANNEL_K_5              0x3005
#define RC_CHANNEL_K_6              0x3006
#define RC_CHANNEL_K_7              0x3007
#define RC_CHANNEL_K_8              0x3008
#define RC_CHANNEL_K_9              0x3009
									

#define RC_CHANNEL_PLUS_K           0x3020
#define RC_CHANNEL_MINUS_K          0x3021
                                    
#define RC_VOLUME_PLUS_K            0x3010
#define RC_VOLUME_MINUS_K           0x3011
								    
#define RC_MENU_K                   0x2012
#define RC_MENU_MAG_K               0x302E //magnavous function the same as 0x2012

#define RC_UP_K                     0x2010
#define RC_DOWN_K                   0x2011
#define RC_LEFT_K                   0x2015
#define RC_RIGHT_K                  0x2016
							        
//#define RC_PIP_SIZE_K               0x2018
#define RC_PIP_SIZE_K               0x2028
#define RC_AV_SOURE_K               0x3038
#define RC_PIP_POSITION_K           0x2019
#define RC_SLEEP_TIMER_K            0x3026
//#define RC_PROGRAM_LIST_K           0x2017
#define RC_OK_K           			0x2017
#define RC_STANDBY_K                0x300C
#define RC_MUTE_K                   0x300D    // not used
#define RC_SMART_SOUND_K            0x30CB
#define RC_SMART_PICTURE_K          0x30CA
#define RC_TELETEXT_R_K             0x202B
#define RC_TELETEXT_G_K             0x202C
#define RC_TELETEXT_Y_K             0x202D
#define RC_TELETEXT_C_K             0x202E
#define RC_TELETEXT_ON_OFF_K        0x303C
#define RC_ENLARGE_K                0x302B
#define RC_HOLD_K                   0x3029
#define RC_MIX_K                    0x302E
#define RC_REVEAL_K                 0x302C
#define RC_OSD_K                    0x300F
#define RC_DUAL_K                   0x3023
#define RC_PP_K                     0x3022
#define RC_PCTV_K                   0x200C
#define RC_CC_K  		     		0x303A
#define RC_VIDEOFORMAT_K  		    0x203E
#define RC_FREEZE_K         		0x201D

#define KEY_DEBOUCE_TIMES   (3)
//#define KEY_REPEAT_TIMES    (10)    /* unit: 10 ms */

/* variables */

/* functions */
GLOBAL void KeyScan(void);
GLOBAL void PcKeyProcess(void);
GLOBAL signed char GetKey(void);
GLOBAL Byte LSBToMSB(Byte);

#endif
