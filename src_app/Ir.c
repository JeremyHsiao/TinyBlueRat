/*==========================================================================
|
|  Project Name : LCD TV RC controller
|
|  File Name	: Debug.C
|
|===========================================================================
|
|  Copyright TPV Technology Group /P-Harmony Monitors (Taiwan) Ltd.
|  All rights are reserved. Reproduction in full or in part is prohibited
|  without the prior written consent of the copyright owner.
|
|  COMPANY CONFIDENTIAL
|
|  Date 		Author(s)		Status & Comments
|---------------------------------------------------------------------------
|  2007/04/27	HW Hsu 			Draft
|       
|===========================================================================
|
|  Brief Description
|
|  This file is debug list and history.
|
|  Note
|
|  The KEIL C51 Compiler ( Version 7.07a ) is used.
|
==========================================================================*/

//#define _DEBUG_C_

#include "chip.h"
#include "board.h"
#include "string.h"
#include "Timer.h"
#include "watchdog.h"
#include "Io.h"
#include "gpio.h"
// System

// Common
#include "Define.h"
// Internal
#define  GLOBAL
#include "Ir.h"
#undef GLOBAL

// External
#define  GLOBAL extern
#include "global.h"
#include "app_uart.h"
#include "app_timer.h"
#include "version.h"
#include "Nvram_Access.h"
#include "Keyscan.h"
#include "IrAuto.h"
#include "IrMacro.h" 
#include "IrDA.h"
#include "timeout.h"
#undef GLOBAL

struct  _SNVRAM_IR_CODE sIR_RAM_TBL[IR_RAM_TBL_MAX];
uint32_t	IrCodeCounter;		// This variable seems unused now.

void IrRamInitial(void)
{
    memset(sIR_RAM_TBL, 0, sizeof(sIR_RAM_TBL[IR_RAM_TBL_MAX]));

    scTdiffer = Tdiff();

//    scCarrierdif = CarrierDif();
    scCarrierdif = 0;   //set fixed value for carrire frequency.

//    ClearWatchdogTimer();

    IrCodeDefault();
    IrTimerBase();
    IR_CarrierBase();
}

void IrCodeDefault(void)
{
    //for test 48 key code
    Byte i;
    Byte j;
    uint8_t		I2CRxd[8];

    const Byte aIrData[IR_RAM_TBL_MAX][8] =
    {   //Ty, mod, Add, cmd, rep, rev0, rs2, rsv2
        ////////////////////////////////RC6 table////////////////////////////
        {  6, 0, 0x30, 0x42,  0,  0,  0,  0},
        {  6, 0,    0, 0x0C,  0,  0,  0,  1},
        {  0, 0,    0,    0,  0,  0,  0,  2},
        {  0, 0,    0,    0,  0,  0,  0,  3},
        {  0, 0,    0,    0,  0,  0,  0,  4},
        {  6, 0,    0, 0x46,  0,  0,  0,  5},
        {  6, 0,    0, 0x67,  0,  0,  0,  6},
        {  6, 0,    0, 0x4E,  0,  0,  0,  7},
        {  6, 0,    0, 0xF8,  0,  0,  0,  8},
        {  6, 0, 0x30, 0x21,  0,  0,  0,  9},

        {  6, 0,    0, 0x38,  0,  0,  0,  10},
        {  6, 0, 0x30, 0x21,  0,  0,  0,  11},
        {  6, 0,    0, 0xD1,  0,  0,  0,  12},
        {  6, 0,    0, 0x54,  0,  0,  0,  13},
        {  6, 0,    0, 0x58,  0,  0,  0,  14},
        {  6, 0,    0, 0x5A,  0,  0,  0,  15},
        {  6, 0,    0, 0x59,  0,  0,  0,  16},
        {  6, 0,    0, 0x5B,  0,  0,  0,  17},
        {  0, 0,    0,    0,  0,  0,  0,  18},
        {  6, 0,    0, 0x5C,  0,  0,  0,  19},

        {  6, 0,    0, 0xF4,  0,  0,  0,  20},
        {  6, 0,    0, 0x47,  0,  0,  0,  21},
        {  6, 0,    0, 0xF3,  0,  0,  0,  22},
        {  6, 0,    0, 0x10,  0,  0,  0,  23},
        {  6, 0,    0, 0x11,  0,  0,  0,  24},
        {  6, 0,    0, 0x0D,  0,  0,  0,  25},
        {  6, 0,    0, 0x20,  0,  0,  0,  26},
        {  6, 0,    0, 0x21,  0,  0,  0,  27},
        {  6, 0,    0, 0x01,  0,  0,  0,  28},
        {  6, 0,    0, 0x02,  0,  0,  0,  29},

        {  6, 0,    0, 0x03,  0,  0,  0,  30},
        {  6, 0,    0, 0x04,  0,  0,  0,  31},
        {  6, 0,    3, 0x05,  0,  0,  0,  32},
        {  6, 0,    0, 0x06,  0,  0,  0,  33},
        {  6, 0,    0, 0x07,  0,  0,  0,  34},
        {  6, 0,    0, 0x08,  0,  0,  0,  35},
        {  6, 0,    0, 0x09,  0,  0,  0,  36},
        {  6, 0,    0, 0xD9,  0,  0,  0,  37},
        {  6, 0,    0, 0x00,  0,  0,  0,  38},
        {  6, 0,    0, 0x0A,  0,  0,  0,  39},


        {  6, 0,    0, 0x00,  0,  0,  0,  40},
        {  6, 0,    0, 0x85,  0,  0,  0,  41},
        {  6, 0,    0, 0xE3,  0,  0,  0,  42},
        {  6, 0,    0, 0x4E,  0,  0,  0,  43},
        {  6, 0,    0, 0x0F,  0,  0,  0,  44},
        {  0, 0,    0, 0xD2,  0,  0,  0,  45},
        {  6, 0,    0, 0xD3,  0,  0,  0,  46},
        {  5, 0,    0, 0xF5,  0,  0,  0,  47},
        {  0, 0,    0,    0,  0,  0,  0,  48}  //add for pc
    };

    ReadEEPROM(KEY_RAM_MARK_ADDR, I2CRxd, 1);
    if(I2CRxd[0] != KEY_RAM_MARK_VALUE)
    {
        for (i=0; i < IR_RAM_TBL_MAX; i++)
        {
            for(j=0; j < 8; j++)
            {
                StoreEEPROM(KEY_RAMSTAR_ADDR+i*8+j, aIrData[i][j]);
            }

            ClearWatchdogTimer();
        }
        StoreEEPROM(KEY_RAM_MARK_ADDR, KEY_RAM_MARK_VALUE);
    }

    for (i=0; i < IR_RAM_TBL_MAX; i++)
    {
        ReadEEPROM(KEY_RAMSTAR_ADDR+i*8, I2CRxd, 8);

        sIR_RAM_TBL[i].Type     = I2CRxd[0];
        sIR_RAM_TBL[i].Mode     = I2CRxd[1];
        sIR_RAM_TBL[i].Address  = I2CRxd[2];
        sIR_RAM_TBL[i].Cmd      = I2CRxd[3];
        sIR_RAM_TBL[i].Repeat   = I2CRxd[4];
        sIR_RAM_TBL[i].Rev0     = I2CRxd[5];
        sIR_RAM_TBL[i].Rev1     = I2CRxd[6];
        sIR_RAM_TBL[i].Rev2     = I2CRxd[7];

//        ClearWatchdogTimer();
    }
}

////////////////////////////////////////////////////
//RC5/RC6  35280hz<=36000 requence<=36720 :(+-2%) //////
//NEC
////////////////////////////////////////////////////
void IR_Carrier(uint32_t ulFre)
{
	PWM_period = (uint32_t) (PCLK_FREQUENCY/(ulFre));
	Chip_TIMER_SetMatch(LPC_TIMER32_1, MATCH_3, PWM_period);
	Chip_TIMER_Reset(LPC_TIMER32_1);
    // Reset Timer1 value to avoid missing reset-point due to frequency change
    // Especially when low->high frequency
}

void IR_CarrierBase(void)
{
    signed long int siDiff;

	Setup_IR_PWM_duty_cycle(33);
    switch(sIR_STATUS.IrType)
    {
        case RC5_INDEX:
        case RC6_INDEX:
        case RCMM_INDEX:
//        	Setup_IR_PWM_duty_cycle(33);
            siDiff = 36000;
            break;
        case MAT_INDEX:
            siDiff = 36000;
            break;
        case PANA_INDEX:
			siDiff = 36700;
			break;

        case NEC1_INDEX:
        case NEC2_INDEX:
        case RCA_INDEX:
        case SONY_INDEX:
        case SHARP_INDEX:
            siDiff = 37900;
            break;

        default:
            putstr("\r\n Error: IR_CarrierBase");
            return;

    }

    siDiff = siDiff + (signed long int) siDiff * scCarrierdif / 100;
    IR_Carrier(siDiff);
}

void IR_TimeTBL(signed char KeyIndex, Byte FirstKey)
{
    if(FirstKey)
    {
        if(sIR_RAM_TBL [KeyIndex].Type != sIR_STATUS.IrType)
        {
            sIR_STATUS.IrType = sIR_RAM_TBL [KeyIndex].Type;

            IR_CarrierBase();
            IrTimerBase();
        }
    }

    switch(sIR_STATUS.IrType)
    {
        case RC5_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_RC5(KeyIndex);
            }
            IR_TimeTBL_RC5();
            break;

        case RC6_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_RC6(KeyIndex);
            }
            IR_TimeTBL_RC6();
            break;

        case NEC1_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_NEC(KeyIndex);

                IR_TimeTBL_NEC();
            }
            else
            {
                IR_TimeTBL_NECRepeat();
            }
            break;

        case NEC2_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_NEC(KeyIndex);
            }
            IR_TimeTBL_NEC();
            break;

        case RCA_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_RCA(KeyIndex);
            }
            IR_TimeTBL_RCA();
            break;

        case SONY_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_SONY(KeyIndex);
            }
            IR_TimeTBL_SONY();
            break;

        case SHARP_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_SHARP(KeyIndex);
            }
            IR_TimeTBL_SHARP();
            break;

        case MAT_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_MAT(KeyIndex);
            }
            IR_TimeTBL_MAT();
            break;

        case PANA_INDEX:
            if(FirstKey)
            {
                IrCodeCodeMix_PANA(KeyIndex);
            }
            IR_TimeTBL_PANA();
            break;

        case RCMM_INDEX:
//            if(FirstKey)
//            {
//                IrCodeCodeMix_RCMM(KeyIndex);
//            }
//            IR_TimeTBL_RCMM();
            break;


        default:
            sIR_STATUS.IrType = RC_NULL;
			putstr("\r\n Error: IR_Type in the IR_TimeTBL()");
		return;
    }
}

void IR_TimeLevel( unsigned long int wTime, Byte bLevel)
{
//    wTimeBase += wTime;

    sRC_TIMETABLE_Output[bIrTimeIndexIn_Output].wTimeStamp = wTime;  // Transfer the time to real time
    sRC_TIMETABLE_Output[bIrTimeIndexIn_Output].bLevel = bLevel;
//    printf("%d : ", sRC_TIMETABLE_Output[bIrTimeIndexIn_Output].bLevel);
//    printf("%d \n", sRC_TIMETABLE_Output[bIrTimeIndexIn_Output].wTimeStamp);

    if(++bIrTimeIndexIn_Output >= IR_TBL_MAX)  bIrTimeIndexIn_Output = 0;
}

void IrCodeCodeMix_RC5(Byte bIndex)
{
    Byte bAddress;
    Byte bCmd;
    Byte bS2;

    bAddress    = sIR_RAM_TBL [bIndex].Address;
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }
// For DQE request, disable the toggle bit.
//    sIR_STATUS.bToggle          ^= 0x01;
    sIR_STATUS.wScriptStepDelay = (Word) (sIR_RAM_TBL [bIndex].Rev0 << 8) | sIR_RAM_TBL [bIndex].Rev1;

    //	printf("\r\n bIndex=== %x", bIndex);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);

    bS2     = bCmd / 64;  //cmd over 64 need one system bit S2 to support, let S2 low
    bCmd    %= 64;

    sIR_STATUS.IrKeyCode = _BIT12;

    if(bS2)
    {
        sIR_STATUS.IrKeyCode = 0;    //pull low
    }

    sIR_STATUS.IrKeyCode = sIR_STATUS.IrKeyCode | _BIT13 | ((Word) bAddress << 6) | bCmd;

//    printf("\r\n sIR_STATUS.IrKeyCode=== %x", sIR_STATUS.IrKeyCode);
}

void IrCodeCodeMix_RC6(Byte bIndex)
{
    Byte bMod;
    Byte bAddress;
    Byte bCmd;

    bMod        = sIR_RAM_TBL [bIndex].Mode;
    bAddress    = sIR_RAM_TBL [bIndex].Address;
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Mode             = bMod & 0x07;  //three bits.
    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }
// For DQE request, disable the toggle bit.
//    sIR_STATUS.bToggle          ^= 0x01;
    sIR_STATUS.wScriptStepDelay = (Word) (sIR_RAM_TBL [bIndex].Rev0 << 8) | sIR_RAM_TBL [bIndex].Rev1;
    sIR_STATUS.IrKeyCode = (Word) (bAddress << 8) | bCmd;

    //	printf("\r\n bIndex=== %x", bIndex);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Mode=== %x", bMod);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);
}

void IrCodeCodeMix_NEC(Byte bIndex)
{
    Byte bMod;
    Byte bAddress;
    Byte bCmd;
    Byte bS1;
    Byte bS2;
    Byte bS3;

    bMod        = sIR_RAM_TBL [bIndex].Mode;
    bAddress    = sIR_RAM_TBL [bIndex].Address;
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }

    sIR_STATUS.wScriptStepDelay = (Word)(sIR_RAM_TBL [bIndex].Rev0<<8) | sIR_RAM_TBL [bIndex].Rev1;

    //	printf("\r\n bIndex=== %x", bIndex);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Mode=== %x", bMod);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bRepeat=== %x", sIR_STATUS.Repeat);

    bS1 = LSBToMSB(bMod);
    bS2 = LSBToMSB(bAddress);
    bS3 = LSBToMSB(bCmd);

    sIR_STATUS.IrKeyCode    = (((bS1 & 0x00FF) << 8)| (bS2 & 0x00FF));
    sIR_STATUS.IrKeyCmdCode = (((bS3 & 0x00FF) << 8)|((~bS3) & 0x00FF));

    //	printf("\r\n sIR_STATUS.IrKeyCode=== %x", sIR_STATUS.IrKeyCode);
    //	printf("\r\n sIR_STATUS.IrKeyCmdCode === %x", sIR_STATUS.IrKeyCmdCode);
}

void IrCodeCodeMix_RCA(Byte bIndex)
{
    Byte bMod;
    Byte bAddress;
    Byte bCmd;
    Byte bS1;
    Byte bS2;
    Byte bS3;

    bMod        = sIR_RAM_TBL [bIndex].Mode;//00
    bAddress    = sIR_RAM_TBL [bIndex].Address;// 4 bits
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }

    sIR_STATUS.wScriptStepDelay = (Word)(sIR_RAM_TBL [bIndex].Rev0<<8) | sIR_RAM_TBL [bIndex].Rev1;

    //	printf("\r\n bIndex=== %x", bIndex);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Mode=== %x", bMod);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bRepeat=== %x", sIR_STATUS.Repeat);

 //   bS1 = LSBToMSB(bMod);
 //   bS2 = LSBToMSB(bAddress);
 //   bS3 = LSBToMSB(bCmd);

    sIR_STATUS.IrKeyCode    = (((bAddress & 0x000F) << 8)| (bCmd & 0x00FF));
    sIR_STATUS.IrKeyCmdCode = ((~sIR_STATUS.IrKeyCode) & 0x0FFF);

    //	printf("\r\n sIR_STATUS.IrKeyCode=== %x", sIR_STATUS.IrKeyCode);
    //	printf("\r\n sIR_STATUS.IrKeyCmdCode === %x", sIR_STATUS.IrKeyCmdCode);
}

void IrCodeCodeMix_SONY(Byte bIndex)
{
    Byte bMod;
    Byte bAddress;
    Byte bCmd;
    Byte bS1;
    Byte bS2;
    Byte bS3;

    bMod        = sIR_RAM_TBL [bIndex].Mode;
    bAddress    = sIR_RAM_TBL [bIndex].Address;
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }

    sIR_STATUS.wScriptStepDelay = (Word)(sIR_RAM_TBL [bIndex].Rev0<<8) | sIR_RAM_TBL [bIndex].Rev1;

    	//printf("\r\n bIndex=== %x", bIndex);
    	//printf("\r\n sIR_RAM_TBL [bIndex].Mode=== %x", bMod);
    	//printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
    	//printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);
    	//printf("\r\n sIR_RAM_TBL [bIndex].bRepeat=== %x", sIR_STATUS.Repeat);

    bS1 = LSBToMSB(bMod);
    bS2 = LSBToMSB(bAddress);
    bS3 = LSBToMSB(bCmd);

    //sIR_STATUS.IrKeyCode    = (((bS1 & 0x00FF) << 8)| (bS2 & 0x00F8));
    sIR_STATUS.IrKeyCode    = (((bS2 & 0x00F8) << 8));
    sIR_STATUS.IrKeyCmdCode = (((bS3 & 0x00FE) << 8));

    	//printf("\r\n sIR_STATUS.IrKeyCode=== %x", sIR_STATUS.IrKeyCode);
    	//printf("\r\n sIR_STATUS.IrKeyCmdCode === %x", sIR_STATUS.IrKeyCmdCode);
}


void IrCodeCodeMix_SHARP(Byte bIndex)
{
    Byte bMod;
    Byte bAddress;
    Byte bCmd;
    Byte bS1;
    Byte bS2;
    Byte bS3;

    bMod        = sIR_RAM_TBL [bIndex].Mode;
    bAddress    = sIR_RAM_TBL [bIndex].Address;
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }

    sIR_STATUS.wScriptStepDelay = (Word)(sIR_RAM_TBL [bIndex].Rev0<<8) | sIR_RAM_TBL [bIndex].Rev1;

    //	printf("\r\n bIndex=== %x", bIndex);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Mode=== %x", bMod);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bRepeat=== %x", sIR_STATUS.Repeat);

    bS1 = LSBToMSB(bMod);
    bS2 = LSBToMSB(bAddress);
    bS3 = LSBToMSB(bCmd);

    //sIR_STATUS.IrKeyCode    = (((bS1 & 0x00FF) << 8)| (bS2 & 0x00F8));
    sIR_STATUS.IrKeyCode    = (((bS2 & 0x00F8) << 8)| (bS3 & 0x00FF));
    sIR_STATUS.IrKeyCmdCode = (((bS2 & 0x00F8) << 8)|((~bS3) & 0x00FF));

    //	printf("\r\n sIR_STATUS.IrKeyCode=== %x", sIR_STATUS.IrKeyCode);
    //	printf("\r\n sIR_STATUS.IrKeyCmdCode === %x", sIR_STATUS.IrKeyCmdCode);
}

void IrCodeCodeMix_MAT(Byte bIndex)
{
    Byte bMod;
    Byte bAddress;
    Byte bCmd;
    Byte bS1;
    Byte bS2;
    Byte bS3;

    bMod        = sIR_RAM_TBL [bIndex].Mode;
    bAddress    = sIR_RAM_TBL [bIndex].Address;
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }

    sIR_STATUS.wScriptStepDelay = (Word)(sIR_RAM_TBL [bIndex].Rev0<<8) | sIR_RAM_TBL [bIndex].Rev1;

    //	printf("\r\n bIndex=== %x", bIndex);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Mode=== %x", bMod);
    //	printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);
    //	printf("\r\n sIR_RAM_TBL [bIndex].bRepeat=== %x", sIR_STATUS.Repeat);

    bS1 = LSBToMSB(bMod);       /* Hi- Address */
    bS2 = LSBToMSB(bAddress);   /* Ho- Address */
    bS3 = LSBToMSB(bCmd);       /* key code */

#if 1
    sIR_STATUS.IrKeyCode    = ((((bS1 & 0x00FC) >> 2) << 6) | (bS3 & 0x00FC) >> 2);
    sIR_STATUS.IrKeyCmdCode = ((((bS2 & 0x00FC) >> 2) << 6) | ((~bS3) & 0x00FC) >> 2);
#else
    sIR_STATUS.IrKeyCode    = (((bS1 & 0x00FF) << 8)| (bS2 & 0x00FF));
    sIR_STATUS.IrKeyCmdCode = (((bS3 & 0x00FF) << 8)|((~bS3) & 0x00FF));
#endif

    //	printf("\r\n sIR_STATUS.IrKeyCode=== %x", sIR_STATUS.IrKeyCode);
    //	printf("\r\n sIR_STATUS.IrKeyCmdCode === %x", sIR_STATUS.IrKeyCmdCode);
}

void IrCodeCodeMix_PANA(Byte bIndex)
{
    Byte bMod;
    Byte bAddress;
    Byte bCmd;
    Byte bS1;
    Byte bS2;
    Byte bS3;
	Byte bS4;

    bMod        = sIR_RAM_TBL [bIndex].Mode;
    bAddress    = sIR_RAM_TBL [bIndex].Address;
    bCmd        = sIR_RAM_TBL [bIndex].Cmd;

    sIR_STATUS.Repeat           = sIR_RAM_TBL [bIndex].Repeat;

    sIR_STATUS.RepeatCountDown  = sIR_STATUS.Repeat;
    if(sIR_STATUS.RepeatCountDown)
    {
        sIR_STATUS.RepeatCountDown--;   /* first key has run*/
    }

    sIR_STATUS.wScriptStepDelay = (Word)(sIR_RAM_TBL [bIndex].Rev0<<8) | sIR_RAM_TBL [bIndex].Rev1;

//    printf("\r\n bIndex=== %x", bIndex);
//    printf("\r\n sIR_RAM_TBL [bIndex].Mode=== %x", bMod);
//    printf("\r\n sIR_RAM_TBL [bIndex].Address=== %x", bAddress);
//    printf("\r\n sIR_RAM_TBL [bIndex].bCmd=== %x", bCmd);
//    printf("\r\n sIR_RAM_TBL [bIndex].bRepeat=== %x", sIR_STATUS.Repeat);

    bS4 = 0x00;					    // Parity
//    bS1 = LSBToMSB(bMod<<4);		// System 4 bits
    bS1 = LSBToMSB(bMod);		// System 4 bits
    bS2 = LSBToMSB(bAddress);		// Equipment 8 bits
    bS3 = LSBToMSB(bCmd);			// Command Code 8bits
	bS4 = bS1;
	bS4 ^= bS2;
	bS4 ^= bS3;

    sIR_STATUS.IrKeyCode    = (((bS1 & 0x00FF) << 8)| (bS2 & 0x00FF));
    sIR_STATUS.IrKeyCmdCode = (((bS3 & 0x00FF) << 8)|((bS4) & 0x00FF));

//    printf("\r\n sIR_STATUS.IrKeyCode=== %x", sIR_STATUS.IrKeyCode);
//    printf("\r\n sIR_STATUS.IrKeyCmdCode === %x", sIR_STATUS.IrKeyCmdCode);
}

void IrCodeCodeMix_RCMM(Byte bIndex)
{

}

void IR_TimeTBL_RC5 (void)
{
    Byte j;
    WORD wtempIrKeycode;
    Byte last_level;

    //2012 menu key
    //10        0000       ; 0001        0010
    //0110      10101010   ; 1010101001  10100110 //add another 00 for ending.

	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter + CONTER_OFFSET_RC5;

    wtempIrKeycode = sIR_STATUS.IrKeyCode;

    if(sIR_STATUS.bToggle)
    {
        wtempIrKeycode |= _BIT11;
    }
    else
    {
        wtempIrKeycode &= (~_BIT11);
    }

    for(j = 0; j < 14; j++)
    {
        if(wtempIrKeycode & _BIT13)
        {
            IR_TimeLevel(889, 0);
            IR_TimeLevel(889, 1);

//            last_level = 0;
        }
        else
        {
            IR_TimeLevel(889, 1);
            IR_TimeLevel(889, 0);

//            last_level = 1;
        }

        wtempIrKeycode <<= 1;
    }

    // let the IR_out low
//    if(last_level)
//    {
//        IR_TimeLevel(889, 1);
//    }

 //   IR_TimeLevel(889, 0);		//HW

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();

}

void IR_TimeTBL_RC6 (void)
{
    Byte i, bRC6_Mode_Temp;
    WORD wtempIrKeycode;
    Byte last_level;

	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter + CONTER_OFFSET_RC6;

    wtempIrKeycode = sIR_STATUS.IrKeyCode;
    bRC6_Mode_Temp = sIR_STATUS.Mode  ;

    //MS=>6T:1 , 2T:0
//    IR_TimeLevel(0, 1);
    IR_TimeLevel(2666, 1);
    IR_TimeLevel(889, 0);
    //SB=>1T:1 , 1T:0
    IR_TimeLevel(444, 1);
    IR_TimeLevel(444, 0);
// Field 3 bits
    for(i = 0; i < 3; i++)
    {
        if(bRC6_Mode_Temp & _BIT2)
        {
            IR_TimeLevel(444, 1);
            IR_TimeLevel(444, 0);
        }
        else
        {
            IR_TimeLevel(444, 0);
            IR_TimeLevel(444, 1);
        }
        bRC6_Mode_Temp <<= 1;
    }

    //TR=>2T:0 , 2T:1
    if(sIR_STATUS.bToggle)
    {
        IR_TimeLevel(889, 1);
        IR_TimeLevel(889, 0);
    }
    else
    {
        IR_TimeLevel(889, 0);
        IR_TimeLevel(889, 1);
    }

    if (wtempIrKeycode & _BIT15)
    {
        IR_TimeLevel(444, 1);
        IR_TimeLevel(444, 0);
    }
    else
    {
        IR_TimeLevel(444, 0);
        IR_TimeLevel(444, 1);
    }

    wtempIrKeycode <<= 1;

    for(i = 0; i < 15; i++)
    {
        if (wtempIrKeycode & _BIT15)
        {
            IR_TimeLevel(444, 1);
            IR_TimeLevel(444, 0);

            last_level = 0;
        }
        else
        {
            IR_TimeLevel(444, 0);
            IR_TimeLevel(444, 1);

            last_level = 1;
        }
        wtempIrKeycode <<= 1;
    }

    // let the IR_out low
//    if(last_level)
//    {
//        IR_TimeLevel(444, 1);
//    }
//    IR_TimeLevel(444, 0);		//HW

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}

void IR_TimeTBL_NEC(void)
{
    int i;

	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter + CONTER_OFFSET_NEC;

//    IR_TimeLevel(0, 1);

    IR_TimeLevel(9000, 1);  // 160

    IR_TimeLevel(4500, 0);	//80

    for(i = 0; i < 32; i++)
    {
        IR_TimeLevel(560, 1);	// 10

        if (i < 16)
        {
            if((sIR_STATUS.IrKeyCode << i) & _BIT15)
            {
                IR_TimeLevel(1690, 0);	// 30
            }
            else
            {
                IR_TimeLevel(560, 0);	// 10
            }
        }
        else
        {
            if((sIR_STATUS.IrKeyCmdCode << (i-16)) & _BIT15)
            {
                IR_TimeLevel(1690, 0);	// 30
            }
            else
            {
                IR_TimeLevel(560, 0);	// 10
            }
        }
    }

    IR_TimeLevel(560, 1);	// 10

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}

void IR_TimeTBL_RCA(void)
{
    int i;

	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter + CONTER_OFFSET_RCA;

//    IR_TimeLevel(0, 1);

    IR_TimeLevel(4000, 1);  // 80

    IR_TimeLevel(4000, 0);	//80

    for(i = 0; i < 24; i++)
    {
        IR_TimeLevel(500, 1);	// 10

        if (i < 12)
        {
            if((sIR_STATUS.IrKeyCode << i) & _BIT11)
            {
                IR_TimeLevel(2000, 0);	// 30
            }
            else
            {
                IR_TimeLevel(1000, 0);	// 10
            }
        }
        else
        {
            if((sIR_STATUS.IrKeyCmdCode << (i-12)) & _BIT11)
            {
                IR_TimeLevel(2000, 0);	// 30
            }
            else
            {
                IR_TimeLevel(1000, 0);	// 10
            }
        }
    }

    IR_TimeLevel(500, 1);	// 10

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}


void IR_TimeTBL_NECRepeat(void)
{
	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter+CONTER_OFFSET_NEC;

//    IR_TimeLevel(0, 1);

    IR_TimeLevel(9000, 1);	// 160

    IR_TimeLevel(2250, 0);	// 40

    IR_TimeLevel(560, 1);	//

    IR_TimeLevel(560, 0);		//HW

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}

void IR_TimeTBL_SONY(void)
{
    int i;

	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter + CONTER_OFFSET_NEC;

//    IR_TimeLevel(0, 1);

    IR_TimeLevel(2400, 1);		// 40

    IR_TimeLevel(600, 0);

    for(i = 0; i < 12; i++)
    {
        if (i < 7)
        {
            if((sIR_STATUS.IrKeyCmdCode << i) & _BIT15)
            {
                IR_TimeLevel(1200, 1);
            }
            else
            {
                IR_TimeLevel(600, 1);
            }
            IR_TimeLevel(600, 0);
        }
        else if ((i >= 7) && (i < 11))
        {
            if((sIR_STATUS.IrKeyCode << (i-7)) & _BIT15)
            {
                IR_TimeLevel(1200, 1);
            }
            else
            {
                IR_TimeLevel(600, 1);
            }
            IR_TimeLevel(600, 0);
        }
        else
        {
            if((sIR_STATUS.IrKeyCode << (i-7)) & _BIT15)
            {
                IR_TimeLevel(1200, 1);
            }
            else
            {
                IR_TimeLevel(600, 1);
            }
        }
    }


//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}



void IR_TimeTBL_SHARP(void)
{
    int i;

	IR_LED_OUT_HIGH;

//    wTimeBase = IrCodeCounter + CONTER_OFFSET_SHARP;

	//5BIT SYSTEM + 8BIT CMD

//    IR_TimeLevel(0, 1);

    for(i = 0; i < 13; i++)
    {
        IR_TimeLevel(320, 1);

        if (i < 5)
        {
            if(sIR_STATUS.IrKeyCode  & _BIT15)
            {
                IR_TimeLevel(1680, 0);//1680/40 = 42
            }
            else
            {
               IR_TimeLevel(680, 0);//680/40 = 17
            }

			sIR_STATUS.IrKeyCode <<= 1;
        }
        else //if ((i >= 5) && (i < 13))
        {
            if(sIR_STATUS.IrKeyCode & _BIT12)
            {
                IR_TimeLevel(1680, 0);
            }
            else
            {
                IR_TimeLevel(680, 0);
            }
			sIR_STATUS.IrKeyCode <<= 1;
        }
    }

	//2BIT EXT (1, 0)
	IR_TimeLevel(320, 1);
	IR_TimeLevel(1680, 0);
	IR_TimeLevel(320, 1);
	IR_TimeLevel(680, 0);

    //1BIT 40 MS INTERVAL
    IR_TimeLevel(320, 1);
    IR_TimeLevel(40000, 0);//40000/40 = 1000

	//5BIT SYSTEM +8BIT CMD
    for(i = 0; i < 13; i++)
    {
        IR_TimeLevel(320, 1);
        if (i < 5)
        {
            if(sIR_STATUS.IrKeyCmdCode & _BIT15)
            {
                IR_TimeLevel(1680, 0);//1680/40 = 42
            }
            else
            {
                IR_TimeLevel(680, 0);//680/40 = 17
            }
			sIR_STATUS.IrKeyCmdCode <<= 1;
        }
        else //if ((i >= 5) && (i < 13))	Invert data
        {
            if(sIR_STATUS.IrKeyCmdCode  & _BIT12)
            {
                IR_TimeLevel(1680, 0);
            }
            else
            {
                IR_TimeLevel(680, 0);
            }
            sIR_STATUS.IrKeyCmdCode <<= 1;
        }
    }

	//2BIT EXT (0, 1)
	IR_TimeLevel(320, 1);
	IR_TimeLevel(680, 0);
	IR_TimeLevel(320, 1);
	IR_TimeLevel(1680, 0);

    IR_TimeLevel(320, 1);
//    IR_TimeLevel(320, 0);		//HW

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}

void IR_TimeTBL_MAT(void)
{
    int i;

	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter + CONTER_OFFSET_MAT;

//    IR_TimeLevel(0, 1);

    IR_TimeLevel(3520, 1);

    IR_TimeLevel(3520, 0);

    for(i = 0; i < 24; i++)
    {
        IR_TimeLevel(880, 1);

        if (i < 12)
        {
            if((sIR_STATUS.IrKeyCode << i) & _BIT11)
            {
                IR_TimeLevel(2640, 0);
            }
            else
            {
                IR_TimeLevel(880, 0);
            }
        }
        else
        {
            if((sIR_STATUS.IrKeyCmdCode << (i-12)) & _BIT11)
            {
                IR_TimeLevel(2640, 0);
            }
            else
            {
                IR_TimeLevel(880, 0);
            }
        }
    }

    IR_TimeLevel(880, 1);
//    IR_TimeLevel(880, 0);		//HW

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }

//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}
/////////////////////////////////////////////////////////////////////
/////RC5 T time: 844.444us<-----889us------->933.333us  :(+-5%)////////
/////RC6 T time: 422.222us<-----449us------->466.667us  :(+-5%)////////
/////NEC
////////////////////////////////////////////////////

void IR_TimeTBL_PANA(void)			//HW
{
    int i,wPANA_MC;

	wPANA_MC = 0x4004;

	IR_LED_OUT_HIGH;

    wTimeBase = IrCodeCounter + CONTER_OFFSET_PANA;

//    IR_TimeLevel(100, 0);

    IR_TimeLevel(3490, 1);		// 80

    IR_TimeLevel(1745, 0);		// 40

    for(i = 0; i < 48; i++)
    {
        IR_TimeLevel(436, 1);	//

        if (i < 16 )
        {
			if((wPANA_MC << i) & _BIT15)
	        {
	            IR_TimeLevel(1309, 0);		// 30
	        }
	        else
	        {
	            IR_TimeLevel(436, 0);		// 10
	        }
		}
        else if (i < 32)
        {
	        if((sIR_STATUS.IrKeyCode << (i-16)) & _BIT15)
	        {
	            IR_TimeLevel(1309, 0);	// 30
	        }
	        else
	        {
	            IR_TimeLevel(436, 0);	// 10
			}
		}
		else
		{
			 if((sIR_STATUS.IrKeyCmdCode << (i-32)) & _BIT15)
	         {
	            IR_TimeLevel(1309, 0);	// 30
	         }
	         else
	         {
	            IR_TimeLevel(436, 0);	// 10
	         }
		}
    }

    IR_TimeLevel(436, 1);		// 10

//    while(bIrTimeIndexIn != bIrTimeIndexOut)
//    {
//        ClearWatchdogTimer();
//    }
//
//	IR_LED_OUT_LOW;
	IR_Transmit_Buffer_StartSend();
}

void IR_TimeTBL_RCMM(void)			//HW
{

}

void IrTimerBase(void)
{
//    Word        wTHOtemp;
//    Word        wTtimebase;
//    signed long slDiff;
//
    switch(sIR_STATUS.IrType)
    {
        case RC5_INDEX:
//            slDiff          = TH0_89US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_89US + (signed int) slDiff;
//
//            wTtimebase      = SAM_TIME_89US;
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase RC5 \r\n");
            break;

        case RC6_INDEX:
//            slDiff          = TH0_44US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_44US + (signed int) slDiff;
//
//            wTtimebase      = SAM_TIME_44US;
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase RC6 \r\n");
            break;

        case NEC1_INDEX:
        case NEC2_INDEX:
//            slDiff          = TH0_56US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_56US + (signed int) slDiff;
//
//            wTtimebase      = 10;			// SAM_TIME_56US; HW change
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase NEC \r\n");
            break;

        case RCA_INDEX:
//            slDiff          = TH0_56US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_56US + (signed int) slDiff;
//
//            wTtimebase      = 10;			// SAM_TIME_56US; HW change
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase RCA \r\n");
            break;

        case SONY_INDEX:
//            slDiff          = TH0_60US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_60US + (signed int) slDiff;
//
//            wTtimebase      = SAM_TIME_60US;
            bKEY_repeat_times = 5;
 //           putstr("\r\n IrTimerBase SONY \r\n");
            break;

        case SHARP_INDEX:
//            slDiff          = TH0_40US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_40US + (signed int) slDiff;
//
//            wTtimebase      = SAM_TIME_40US;
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase SHARP \r\n");
            break;

        case MAT_INDEX:
//            slDiff          = TH0_44US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_44US + (signed int) slDiff;
//
//            wTtimebase      = SAM_TIME_44US;
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase Matsushita \r\n");
            break;

	    case PANA_INDEX:
//            slDiff          = TH0_44US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_44US + (signed int) slDiff;
//
//            wTtimebase      = 1;			// SAM_TIME_44US; HW Change
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase PANA \r\n");
            break;

	    case RCMM_INDEX:
//            slDiff          = TH0_44US;
//            slDiff          = (slDiff * scTdiffer) / 100;
//            wTHOtemp        = TH0_44US + (signed int) slDiff;
//
//            wTtimebase      = 1;			// SAM_TIME_44US; HW Change
            bKEY_repeat_times = 10;
//            putstr("\r\n IrTimerBase RCMM \r\n");
            break;

        default:
            putstr("\r\n Error: IrTimerBase \r\n");
            return;

    }
//    TimeUnit = wTtimebase;
//
//    wTimer10ms      = 10000 / wTtimebase;
//    SystemTimer10ms = 10000 / wTtimebase;;
//
//    EA = 0;
//    wTL = (65536-wTHOtemp) % 256;
//    wTH = (65536-wTHOtemp) / 256;
//    EA = 1;

//    printf("\r\n wTHOtemp = %d", wTHOtemp);
//    printf("\r\n scTdiffer = %x", scTdiffer);
}


signed char Tdiff(void)
{
    signed char temp;
    uint8_t I2CRxd[1];

    ReadEEPROM(DEVIATION_ADDR, I2CRxd, 1);
    temp = I2CRxd[0];

    return temp;
}

signed char CarrierDif(void)
{
    signed char temp;
//    ReadEEPROM(CARRIER_ADDR, 1);
//    temp = I2CRxd[0];

    temp = 0;   //fixed.
    return temp;
}

void Initial_ir_dec (void)
{
    Reset_ir_dec();
}

void Reset_ir_dec (void)
{
	Byte i;
    memset(&tg_ir_rc5x_data, 0, sizeof(tg_ir_rc5x_data));
    memset(&tg_ir_rc6_data, 0, sizeof(tg_ir_rc6_data));
    memset(&tg_ir_NEC_data, 0, sizeof(tg_ir_NEC_data));
    memset(&tg_ir_SONY_data, 0, sizeof(tg_ir_SONY_data));
    memset(&tg_ir_SHARP_data, 0, sizeof(tg_ir_SHARP_data));
    memset(&tg_ir_MAT_data, 0, sizeof(tg_ir_MAT_data));
    memset(&tg_ir_PANA_data, 0, sizeof(tg_ir_PANA_data));
    memset(&tg_ir_RCMM_data, 0, sizeof(tg_ir_RCMM_data));
    memset(&tg_ir_RCA_data, 0, sizeof(tg_ir_RCA_data));

    ClearIrDecDataFlag = 0;
//IrDA
//	for(i=0;i<IRDA_TBL_MAX;i++)
//		sIrDA_Accumlate[i][0]=0;
////    memset(&sIrDA_Accumlate, 0, sizeof(sIrDA_Accumlate));
////    ClearIrDAFlag = 0;
//    UARTputstr("IR carry frq reset\n");
}

#ifndef  LITE_VERSION
int abs(int value)
{
    return ((value < 0) ? -value : value);
}

BOOL Get_ir_dec_rc6_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_rc6_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_rc6_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Get_ir_dec_rc5x_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_rc5x_data.uc_main_stage == IR_RC5X_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_rc5x_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_rc5x_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Get_ir_dec_NEC_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_NEC_data.uc_main_stage == IR_NEC_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_NEC_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_NEC_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Get_ir_dec_RCA_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_RCA_data.uc_main_stage == IR_RCA_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_RCA_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_RCA_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Get_ir_dec_SONY_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_SONY_data.uc_main_stage == IR_SONY_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_SONY_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_SONY_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}
BOOL Get_ir_dec_SHARP_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_SHARP_data.uc_main_stage == IR_SHARP_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_SHARP_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_SHARP_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Get_ir_dec_Matsushita_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_MAT_data.uc_main_stage == IR_MAT_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_MAT_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_MAT_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Get_ir_dec_PANA_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_PANA_data.uc_main_stage == IR_PANA_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_PANA_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_PANA_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Get_ir_dec_RCMM_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info)
{
    if(tg_ir_RCMM_data.uc_main_stage == IR_RCMM_STAGE_FINISH)
    {
        *p_ui2_mode_addr = tg_ir_RCMM_data.ui2_mode_addr;
        *p_ui2_ctrl_info = tg_ir_RCMM_data.ui2_ctrl_info;

        Reset_ir_dec();

        return TRUE;
    }

    return FALSE;
}

BOOL Ir_dec_rc6_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;
/*
    if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
    {
        i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
    }
    else
    {
        i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
    }
*/
    i2_diff_time = wTimeStamp;

    switch(tg_ir_rc6_data.uc_main_stage)
    {
        case IR_RC6_STAGE_LS:
            if(tg_ir_rc6_data.uc_sub_stage == 0 &&
               (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_6T) < RC6_6T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 1 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_SB;
                }
            }
            break;

        case IR_RC6_STAGE_SB:
            if(tg_ir_rc6_data.uc_sub_stage == 0 &&
               (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                /* Logic*/
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_MB2;
                }
            }
            break;

        case IR_RC6_STAGE_MB2:
            if(tg_ir_rc6_data.uc_sub_stage == 0 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                tg_ir_rc6_data.ui2_mode_addr = 0;

                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    /* wait high level */
                    tg_ir_rc6_data.uc_sub_stage = 1;
                }
                else if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_MB1;

                    tg_ir_rc6_data.ui2_mode_addr &= ~RC6_MB2_BIT;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 1 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_MB1;

                    tg_ir_rc6_data.ui2_mode_addr |= RC6_MB2_BIT;
                }
            }
            break;

        case IR_RC6_STAGE_MB1:
        case IR_RC6_STAGE_MB0:
            /* MB2 == 1 or MB1 == 1*/
            if(tg_ir_rc6_data.uc_sub_stage == 1 &&
               (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    /* wait high */
                    tg_ir_rc6_data.uc_sub_stage = 2;
                }
                else if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;

                    if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB1)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_MB0;
                        tg_ir_rc6_data.ui2_mode_addr &= ~RC6_MB1_BIT;
                    }
                    else if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB0)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_TR;
                        tg_ir_rc6_data.ui2_mode_addr &= ~RC6_MB0_BIT;
                    }
                }
            }
            /* MB2 == 0 or MB1 == 0 */
            else if(tg_ir_rc6_data.uc_sub_stage == 0 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    /* wait low */
                    tg_ir_rc6_data.uc_sub_stage = 3;
                }
                else if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;

                    if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB1)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_MB0;
                        tg_ir_rc6_data.ui2_mode_addr |= RC6_MB1_BIT;
                    }
                    else if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB0)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_TR;
                        tg_ir_rc6_data.ui2_mode_addr |= RC6_MB0_BIT;
                    }
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 2 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;

                    if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB1)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_MB0;
                        tg_ir_rc6_data.ui2_mode_addr |= RC6_MB1_BIT;
                    }
                    else if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB0)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_TR;
                        tg_ir_rc6_data.ui2_mode_addr |= RC6_MB0_BIT;
                    }
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 3 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;

                    if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB1)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_MB0;
                        tg_ir_rc6_data.ui2_mode_addr &= ~RC6_MB1_BIT;
                    }
                    else if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_MB0)
                    {
                        tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_TR;
                        tg_ir_rc6_data.ui2_mode_addr &= ~RC6_MB0_BIT;
                    }
                }
            }
            break;

        case IR_RC6_STAGE_TR:
            /* reset control and info word */
            tg_ir_rc6_data.ui2_ctrl_info = 0;

            /* MB0 == 1 */
            if(tg_ir_rc6_data.uc_sub_stage == 1 &&
               (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    /* wait high */
                    tg_ir_rc6_data.uc_sub_stage = 2;
                }
                else if(abs(i2_diff_time-RC6_3T) < RC6_3T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL7;

                    tg_ir_rc6_data.ui2_mode_addr &= ~RC6_MODE_TOGGLE_BIT;
                }
            }
            /* MB0 == 0 */
            else if(tg_ir_rc6_data.uc_sub_stage == 0 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    /* wait low */
                    tg_ir_rc6_data.uc_sub_stage = 3;
                }
                else if(abs(i2_diff_time-RC6_3T) < RC6_3T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL7;

                    tg_ir_rc6_data.ui2_mode_addr |= RC6_MODE_TOGGLE_BIT;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 2 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL7;

                    tg_ir_rc6_data.ui2_mode_addr |= RC6_MODE_TOGGLE_BIT;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 3 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL7;

                    tg_ir_rc6_data.ui2_mode_addr &= ~RC6_MODE_TOGGLE_BIT;
                }
            }
            break;

        case IR_RC6_STAGE_CTRL7:
            /* TOGGLE == 1 */
            if(tg_ir_rc6_data.uc_sub_stage == 1 &&
               (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    /* wait high */
                    tg_ir_rc6_data.uc_sub_stage = 2;
                }
                else if(abs(i2_diff_time-RC6_3T) < RC6_3T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL6;

                    tg_ir_rc6_data.ui2_ctrl_info &= ~RC6_CTRL7_BIT;
                }
            }
            /* TOGGLE == 0 */
            else if(tg_ir_rc6_data.uc_sub_stage == 0 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    /* wait low */
                    tg_ir_rc6_data.uc_sub_stage = 3;
                }
                else if(abs(i2_diff_time-RC6_3T) < RC6_3T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL6;

                    tg_ir_rc6_data.ui2_ctrl_info |= RC6_CTRL7_BIT;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 2 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL6;

                    tg_ir_rc6_data.ui2_ctrl_info |= RC6_CTRL7_BIT;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 3 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = IR_RC6_STAGE_CTRL6;

                    tg_ir_rc6_data.ui2_ctrl_info &= ~RC6_CTRL7_BIT;
                }
            }
            break;

        case IR_RC6_STAGE_CTRL6:
        case IR_RC6_STAGE_CTRL5:
        case IR_RC6_STAGE_CTRL4:
        case IR_RC6_STAGE_CTRL3:
        case IR_RC6_STAGE_CTRL2:
        case IR_RC6_STAGE_CTRL1:
        case IR_RC6_STAGE_CTRL0:
        case IR_RC6_STAGE_INFO7:
        case IR_RC6_STAGE_INFO6:
        case IR_RC6_STAGE_INFO5:
        case IR_RC6_STAGE_INFO4:
        case IR_RC6_STAGE_INFO3:
        case IR_RC6_STAGE_INFO2:
        case IR_RC6_STAGE_INFO1:
        case IR_RC6_STAGE_INFO0:
            /* set initial condition */
            uc_next_main_stage = tg_ir_rc6_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (RC6_CTRL_MSB_BIT-(tg_ir_rc6_data.uc_main_stage-IR_RC6_STAGE_CTRL7)));

            /* Previous Level == 1 */
            if(tg_ir_rc6_data.uc_sub_stage == 1 &&
               (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    /* wait high */
                    tg_ir_rc6_data.uc_sub_stage = 2;
                }
                else if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = uc_next_main_stage;

                    tg_ir_rc6_data.ui2_ctrl_info &= ~ui2_set_bit;
                }
            }
            /* Previous Level == 0 */
            else if(tg_ir_rc6_data.uc_sub_stage == 0 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    /* wait low */
                    tg_ir_rc6_data.uc_sub_stage = 3;
                }
                else if(abs(i2_diff_time-RC6_2T) < RC6_2T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;
                    tg_ir_rc6_data.uc_main_stage = uc_next_main_stage;

                    tg_ir_rc6_data.ui2_ctrl_info |= ui2_set_bit;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 2 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 1;
                    tg_ir_rc6_data.uc_main_stage = uc_next_main_stage;

                    tg_ir_rc6_data.ui2_ctrl_info |= ui2_set_bit;
                }
            }
            else if(tg_ir_rc6_data.uc_sub_stage == 3 &&
                    (bLevel & RC6_LEVEL_BIT) == RC6_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC6_1T) < RC6_1T_TOL)
                {
                    tg_ir_rc6_data.uc_sub_stage = 0;
                    tg_ir_rc6_data.uc_main_stage = uc_next_main_stage;

                    tg_ir_rc6_data.ui2_ctrl_info &= ~ui2_set_bit;
                }
            }
            break;

        case IR_RC6_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_rc6_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_rc6_data.uc_main_stage != IR_RC6_STAGE_LS)
    {
        printf("\r\n stage = %d", tg_ir_rc6_data.uc_main_stage);
        printf(" %X",tg_ir_rc6_data.ui2_mode_addr);
        printf("%X",tg_ir_rc6_data.ui2_ctrl_info);
    }
#endif

    if(tg_ir_rc6_data.uc_main_stage == IR_RC6_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Ir_dec_rc5x_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

 /*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
 */
    i2_diff_time = wTimeStamp;

    switch(tg_ir_rc5x_data.uc_main_stage)
    {
        case IR_RC5X_STAGE_S1:
            if(tg_ir_rc5x_data.uc_sub_stage == 0 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                tg_ir_rc5x_data.uc_main_stage = IR_RC5X_STAGE_S2;
            }
            break;

        case IR_RC5X_STAGE_S2:
            /* Previous Level == 1 */
            if(tg_ir_rc5x_data.uc_sub_stage == 0 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    /* wait low */
                    tg_ir_rc5x_data.uc_sub_stage = 1;
                }
                else if(abs(i2_diff_time-RC5X_2T) < RC5X_2T_TOL)
                {
                    tg_ir_rc5x_data.ui2_ctrl_info = 0;
                    tg_ir_rc5x_data.ui2_ctrl_info |= RC5X_COMM_BIT6;    /* inverted */

                    tg_ir_rc5x_data.uc_main_stage = IR_RC5X_STAGE_T;
                }
            }
            else if(tg_ir_rc5x_data.uc_sub_stage == 1 &&
                    (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    tg_ir_rc5x_data.ui2_ctrl_info = 0;
                    tg_ir_rc5x_data.ui2_ctrl_info &= ~RC5X_COMM_BIT6;   /* inverted */

                    tg_ir_rc5x_data.uc_main_stage = IR_RC5X_STAGE_T;
                }
            }
            break;

        case IR_RC5X_STAGE_T:
            tg_ir_rc5x_data.ui2_mode_addr = 0;

            /* Previous Level == 1 */
            if(tg_ir_rc5x_data.uc_sub_stage == 1 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    /* wait low */
                    tg_ir_rc5x_data.uc_sub_stage = 2;
                }
                else if(abs(i2_diff_time-RC5X_2T) < RC5X_2T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 0;

                    tg_ir_rc5x_data.ui2_mode_addr &= ~RC5X_MODE_TOGGLE_BIT;

                    tg_ir_rc5x_data.uc_main_stage = IR_RC5X_STAGE_ADDR4;
                }
            }
            /* Previous Level == 0 */
            else if(tg_ir_rc5x_data.uc_sub_stage == 0 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    /* wait high */
                    tg_ir_rc5x_data.uc_sub_stage = 3;
                }
                else if(abs(i2_diff_time-RC5X_2T) < RC5X_2T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 1;

                    tg_ir_rc5x_data.ui2_mode_addr |= RC5X_MODE_TOGGLE_BIT;

                    tg_ir_rc5x_data.uc_main_stage = IR_RC5X_STAGE_ADDR4;
                }
            }
            else if(tg_ir_rc5x_data.uc_sub_stage == 2 &&
                    (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 1;

                    tg_ir_rc5x_data.ui2_mode_addr |= RC5X_MODE_TOGGLE_BIT;

                    tg_ir_rc5x_data.uc_main_stage = IR_RC5X_STAGE_ADDR4;
                }
            }
            else if(tg_ir_rc5x_data.uc_sub_stage == 3 &&
                    (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 0;

                    tg_ir_rc5x_data.ui2_mode_addr &= ~RC5X_MODE_TOGGLE_BIT;

                    tg_ir_rc5x_data.uc_main_stage = IR_RC5X_STAGE_ADDR4;
                }
            }
            break;

        case IR_RC5X_STAGE_ADDR4:
        case IR_RC5X_STAGE_ADDR3:
        case IR_RC5X_STAGE_ADDR2:
        case IR_RC5X_STAGE_ADDR1:
        case IR_RC5X_STAGE_ADDR0:
            /* set initial condition */
            uc_next_main_stage = tg_ir_rc5x_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (RC5X_ADDR_MSB_BIT-(tg_ir_rc5x_data.uc_main_stage-IR_RC5X_STAGE_ADDR4)));

            /* Previous Level == 1 */
            if(tg_ir_rc5x_data.uc_sub_stage == 1 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    /* wait low */
                    tg_ir_rc5x_data.uc_sub_stage = 2;
                }
                else if(abs(i2_diff_time-RC5X_2T) < RC5X_2T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 0;

                    tg_ir_rc5x_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            /* Previous Level == 0 */
            else if(tg_ir_rc5x_data.uc_sub_stage == 0 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    /* wait high */
                    tg_ir_rc5x_data.uc_sub_stage = 3;
                }
                else if(abs(i2_diff_time-RC5X_2T) < RC5X_2T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 1;

                    tg_ir_rc5x_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            else if(tg_ir_rc5x_data.uc_sub_stage == 2 &&
                    (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 1;

                    tg_ir_rc5x_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            else if(tg_ir_rc5x_data.uc_sub_stage == 3 &&
                    (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 0;

                    tg_ir_rc5x_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_RC5X_STAGE_COMM5:
        case IR_RC5X_STAGE_COMM4:
        case IR_RC5X_STAGE_COMM3:
        case IR_RC5X_STAGE_COMM2:
        case IR_RC5X_STAGE_COMM1:
        case IR_RC5X_STAGE_COMM0:
            /* set initial condition */
            uc_next_main_stage = tg_ir_rc5x_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (RC5X_COMM_MSB_BIT-(tg_ir_rc5x_data.uc_main_stage-IR_RC5X_STAGE_COMM5)));

            /* Previous Level == 1 */
            if(tg_ir_rc5x_data.uc_sub_stage == 1 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    /* wait low */
                    tg_ir_rc5x_data.uc_sub_stage = 2;
                }
                else if(abs(i2_diff_time-RC5X_2T) < RC5X_2T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 0;

                    tg_ir_rc5x_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            /* Previous Level == 0 */
            else if(tg_ir_rc5x_data.uc_sub_stage == 0 &&
               (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    /* wait high */
                    tg_ir_rc5x_data.uc_sub_stage = 3;
                }
                else if(abs(i2_diff_time-RC5X_2T) < RC5X_2T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 1;

                    tg_ir_rc5x_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            else if(tg_ir_rc5x_data.uc_sub_stage == 2 &&
                    (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 1;

                    tg_ir_rc5x_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            else if(tg_ir_rc5x_data.uc_sub_stage == 3 &&
                    (bLevel & RC5X_LEVEL_BIT) == RC5X_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RC5X_1T) < RC5X_1T_TOL)
                {
                    tg_ir_rc5x_data.uc_sub_stage = 0;

                    tg_ir_rc5x_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_rc5x_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_RC5X_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_rc5x_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_rc5x_data.uc_main_stage != IR_RC5X_STAGE_S1)
    {
        printf("\r\n tg_ir_rc5x_data.uc_main_stage = %d", tg_ir_rc5x_data.uc_main_stage);
    }
#endif

    if(tg_ir_rc5x_data.uc_main_stage == IR_RC5X_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Ir_dec_NEC_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

/*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
*/
    i2_diff_time = wTimeStamp;

    switch(tg_ir_NEC_data.uc_main_stage)
    {
        case IR_NEC_STAGE_LP:
            if(tg_ir_NEC_data.uc_sub_stage == 0 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-NEC_16T) < NEC_16T_TOL)
                {
                    tg_ir_NEC_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_NEC_data.uc_sub_stage == 1 &&
                    (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_LOW)
            {
                if(abs(i2_diff_time-NEC_8T) < NEC_8T_TOL)
                {
                    tg_ir_NEC_data.uc_sub_stage = 0;
                    tg_ir_NEC_data.uc_main_stage = IR_NEC_STAGE_ADDR0;
                }
            }
            break;

        case IR_NEC_STAGE_ADDR0:
        case IR_NEC_STAGE_ADDR1:
        case IR_NEC_STAGE_ADDR2:
        case IR_NEC_STAGE_ADDR3:
        case IR_NEC_STAGE_ADDR4:
        case IR_NEC_STAGE_ADDR5:
        case IR_NEC_STAGE_ADDR6:
        case IR_NEC_STAGE_ADDR7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_NEC_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_NEC_data.uc_main_stage-IR_NEC_STAGE_ADDR0+8));

            if(tg_ir_NEC_data.uc_sub_stage == 0 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    tg_ir_NEC_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_NEC_data.uc_sub_stage == 1 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_LOW)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;

                }
                else if(abs(i2_diff_time-NEC_3T) < NEC_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_NEC_STAGE_C_ADDR0:
        case IR_NEC_STAGE_C_ADDR1:
        case IR_NEC_STAGE_C_ADDR2:
        case IR_NEC_STAGE_C_ADDR3:
        case IR_NEC_STAGE_C_ADDR4:
        case IR_NEC_STAGE_C_ADDR5:
        case IR_NEC_STAGE_C_ADDR6:
        case IR_NEC_STAGE_C_ADDR7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_NEC_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_NEC_data.uc_main_stage-IR_NEC_STAGE_C_ADDR0));

            if(tg_ir_NEC_data.uc_sub_stage == 0 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    tg_ir_NEC_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_NEC_data.uc_sub_stage == 1 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_LOW)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-NEC_3T) < NEC_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_NEC_STAGE_COMM0:
        case IR_NEC_STAGE_COMM1:
        case IR_NEC_STAGE_COMM2:
        case IR_NEC_STAGE_COMM3:
        case IR_NEC_STAGE_COMM4:
        case IR_NEC_STAGE_COMM5:
        case IR_NEC_STAGE_COMM6:
        case IR_NEC_STAGE_COMM7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_NEC_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_NEC_data.uc_main_stage-IR_NEC_STAGE_COMM0+8));

            if(tg_ir_NEC_data.uc_sub_stage == 0 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    tg_ir_NEC_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_NEC_data.uc_sub_stage == 1 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_LOW)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-NEC_3T) < NEC_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_NEC_STAGE_C_COMM0:
        case IR_NEC_STAGE_C_COMM1:
        case IR_NEC_STAGE_C_COMM2:
        case IR_NEC_STAGE_C_COMM3:
        case IR_NEC_STAGE_C_COMM4:
        case IR_NEC_STAGE_C_COMM5:
        case IR_NEC_STAGE_C_COMM6:
        case IR_NEC_STAGE_C_COMM7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_NEC_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_NEC_data.uc_main_stage-IR_NEC_STAGE_C_COMM0));

            if(tg_ir_NEC_data.uc_sub_stage == 0 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    tg_ir_NEC_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_NEC_data.uc_sub_stage == 1 &&
               (bLevel & NEC_LEVEL_BIT) == NEC_LEVEL_LOW)
            {
                if(abs(i2_diff_time-NEC_1T) < NEC_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-NEC_3T) < NEC_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_NEC_data.uc_sub_stage = 0;

                    tg_ir_NEC_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_NEC_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_NEC_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_NEC_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_NEC_data.uc_main_stage != IR_NEC_STAGE_LP)
    {
        printf("\r\n tg_ir_NEC_data.uc_main_stage = %d", tg_ir_NEC_data.uc_main_stage);
    }
#endif

    if(tg_ir_NEC_data.uc_main_stage == IR_NEC_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Ir_dec_RCA_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

/*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
*/
    i2_diff_time = wTimeStamp;

    switch(tg_ir_RCA_data.uc_main_stage)
    {
        case IR_RCA_STAGE_LP:
            if(tg_ir_RCA_data.uc_sub_stage == 0 &&
               (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RCA_8T) < RCA_8T_TOL)
                {
                    tg_ir_RCA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_RCA_data.uc_sub_stage == 1 &&
                    (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RCA_8T) < RCA_8T_TOL)
                {
                    tg_ir_RCA_data.uc_sub_stage = 0;
                    tg_ir_RCA_data.uc_main_stage = IR_RCA_STAGE_ADDR0;
                }
            }
            break;

        case IR_RCA_STAGE_ADDR0:
        case IR_RCA_STAGE_ADDR1:
        case IR_RCA_STAGE_ADDR2:
        case IR_RCA_STAGE_ADDR3:
        case IR_RCA_STAGE_ADDR4:
        case IR_RCA_STAGE_ADDR5:
        case IR_RCA_STAGE_ADDR6:
        case IR_RCA_STAGE_ADDR7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_RCA_data.uc_main_stage + 1;
            ui2_set_bit = (0x8000 >> (tg_ir_RCA_data.uc_main_stage-IR_RCA_STAGE_ADDR0));

            if(tg_ir_RCA_data.uc_sub_stage == 0 &&
               (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RCA_1T) < RCA_1T_TOL)
                {
                    tg_ir_RCA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_RCA_data.uc_sub_stage == 1 &&
               (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RCA_2T) < RCA_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_RCA_data.uc_sub_stage = 0;

                    tg_ir_RCA_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_RCA_data.uc_main_stage = uc_next_main_stage;

                }
                else if(abs(i2_diff_time-RCA_4T) < RCA_4T_TOL)
                {
                    /* level 1 */
                    tg_ir_RCA_data.uc_sub_stage = 0;

                    tg_ir_RCA_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_RCA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_RCA_STAGE_C_ADDR0:
        case IR_RCA_STAGE_C_ADDR1:
        case IR_RCA_STAGE_C_ADDR2:
        case IR_RCA_STAGE_C_ADDR3:
        case IR_RCA_STAGE_C_ADDR4:
        case IR_RCA_STAGE_C_ADDR5:
        case IR_RCA_STAGE_C_ADDR6:
        case IR_RCA_STAGE_C_ADDR7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_RCA_data.uc_main_stage + 1;
            ui2_set_bit = (0x80 >> (tg_ir_RCA_data.uc_main_stage-IR_RCA_STAGE_C_ADDR0));

            if(tg_ir_RCA_data.uc_sub_stage == 0 &&
               (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RCA_1T) < RCA_1T_TOL)
                {
                    tg_ir_RCA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_RCA_data.uc_sub_stage == 1 &&
               (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RCA_2T) < RCA_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_RCA_data.uc_sub_stage = 0;

                    tg_ir_RCA_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_RCA_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCA_4T) < RCA_4T_TOL)
                {
                    /* level 1 */
                    tg_ir_RCA_data.uc_sub_stage = 0;

                    tg_ir_RCA_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_RCA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_RCA_STAGE_COMM0:
        case IR_RCA_STAGE_COMM1:
        case IR_RCA_STAGE_COMM2:
        case IR_RCA_STAGE_COMM3:
        case IR_RCA_STAGE_COMM4:
        case IR_RCA_STAGE_COMM5:
        case IR_RCA_STAGE_COMM6:
        case IR_RCA_STAGE_COMM7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_RCA_data.uc_main_stage + 1;
            ui2_set_bit = (0x8000 >> (tg_ir_RCA_data.uc_main_stage-IR_RCA_STAGE_COMM0));

            if(tg_ir_RCA_data.uc_sub_stage == 0 &&
               (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RCA_1T) < RCA_1T_TOL)
                {
                    tg_ir_RCA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_RCA_data.uc_sub_stage == 1 &&
               (bLevel & RCA_LEVEL_BIT) == RCA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RCA_2T) < RCA_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_RCA_data.uc_sub_stage = 0;

                    tg_ir_RCA_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_RCA_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCA_4T) < RCA_4T_TOL)
                {
                    /* level 1 */
                    tg_ir_RCA_data.uc_sub_stage = 0;

                    tg_ir_RCA_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_RCA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;


        case IR_RCA_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_RCA_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_RCA_data.uc_main_stage != IR_RCA_STAGE_LP)
    {
        printf("\r\n tg_ir_RCA_data.uc_main_stage = %d", tg_ir_RCA_data.uc_main_stage);
    }
#endif

    if(tg_ir_RCA_data.uc_main_stage == IR_RCA_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Ir_dec_SONY_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

    /*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
    */
    i2_diff_time = wTimeStamp;

    switch(tg_ir_SONY_data.uc_main_stage)
    {
        case IR_SONY_STAGE_LP:
            if(tg_ir_SONY_data.uc_sub_stage == 0 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SONY_4T) < SONY_4T_TOL)
                {
                    tg_ir_SONY_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SONY_data.uc_sub_stage == 1 &&
                    (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    tg_ir_SONY_data.uc_sub_stage = 0;
                    tg_ir_SONY_data.uc_main_stage = IR_SONY_STAGE_COMM0;
                }
            }
            break;

        case IR_SONY_STAGE_COMM0:
        case IR_SONY_STAGE_COMM1:
        case IR_SONY_STAGE_COMM2:
        case IR_SONY_STAGE_COMM3:
        case IR_SONY_STAGE_COMM4:
        case IR_SONY_STAGE_COMM5:
        case IR_SONY_STAGE_COMM6:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SONY_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_SONY_data.uc_main_stage-IR_SONY_STAGE_COMM0));

            if(tg_ir_SONY_data.uc_sub_stage == 0 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    tg_ir_SONY_data.uc_sub_stage = 1;
                }
                else if(abs(i2_diff_time-SONY_2T) < SONY_2T_TOL)
                {
                    tg_ir_SONY_data.uc_sub_stage = 2;
                }
            }
            else if(tg_ir_SONY_data.uc_sub_stage == 1 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_SONY_data.uc_sub_stage = 0;

                    tg_ir_SONY_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_SONY_data.uc_main_stage = uc_next_main_stage;
                }
            }
            else if(tg_ir_SONY_data.uc_sub_stage == 2 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    /* level 1 */
                    tg_ir_SONY_data.uc_sub_stage = 0;

                    tg_ir_SONY_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_SONY_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_SONY_STAGE_ADDR0:
        case IR_SONY_STAGE_ADDR1:
        case IR_SONY_STAGE_ADDR2:
        case IR_SONY_STAGE_ADDR3:
//        case IR_SONY_STAGE_ADDR4:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SONY_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_SONY_data.uc_main_stage-IR_SONY_STAGE_ADDR0));

            if(tg_ir_SONY_data.uc_sub_stage == 0 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    tg_ir_SONY_data.uc_sub_stage = 1;
                }
                else if(abs(i2_diff_time-SONY_2T) < SONY_2T_TOL)
                {
                    tg_ir_SONY_data.uc_sub_stage = 2;
                }
            }
            else if(tg_ir_SONY_data.uc_sub_stage == 1 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_SONY_data.uc_sub_stage = 0;

                    tg_ir_SONY_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_SONY_data.uc_main_stage = uc_next_main_stage;

                }
            }
            else if(tg_ir_SONY_data.uc_sub_stage == 2 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    /* level 1 */
                    tg_ir_SONY_data.uc_sub_stage = 0;

                    tg_ir_SONY_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_SONY_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_SONY_STAGE_ADDR4:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SONY_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_SONY_data.uc_main_stage-IR_SONY_STAGE_ADDR0));

            if(tg_ir_SONY_data.uc_sub_stage == 0 &&
               (bLevel & SONY_LEVEL_BIT) == SONY_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SONY_1T) < SONY_1T_TOL)
                {
                    tg_ir_SONY_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_SONY_data.uc_main_stage = uc_next_main_stage;
                 }
                else if(abs(i2_diff_time-SONY_2T) < SONY_2T_TOL)
                {
                    tg_ir_SONY_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_SONY_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;
        case IR_SONY_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_SONY_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_SONY_data.uc_main_stage != IR_SONY_STAGE_LP)
    {
        printf("\r\n stage = %d", tg_ir_SONY_data.uc_main_stage);
    }
#endif

    if(tg_ir_SONY_data.uc_main_stage == IR_SONY_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



BOOL Ir_dec_SHARP_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

    /*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
    */
    i2_diff_time = wTimeStamp;



    switch(tg_ir_SHARP_data.uc_main_stage)
    {


        case IR_SHARP_STAGE_ADDR0:
            /* set initial condition */
            ui2_set_bit = (1 << (tg_ir_SHARP_data.uc_main_stage-IR_SHARP_STAGE_ADDR0+8));

            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_2T) < SHARP_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    tg_ir_SHARP_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = IR_SHARP_STAGE_ADDR1;

                }
				else if(abs(i2_diff_time-SHARP_6T) < SHARP_6T_TOL)
                {
                    // level 1
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    tg_ir_SHARP_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = IR_SHARP_STAGE_ADDR1;
                    //putstr("1");
                }
            }
            break;
        case IR_SHARP_STAGE_ADDR1:
        case IR_SHARP_STAGE_ADDR2:
        case IR_SHARP_STAGE_ADDR3:
        case IR_SHARP_STAGE_ADDR4:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SHARP_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_SHARP_data.uc_main_stage-IR_SHARP_STAGE_ADDR0+8));

            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_2T) < SHARP_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    tg_ir_SHARP_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;

                }
                else if(abs(i2_diff_time-SHARP_6T) < SHARP_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    tg_ir_SHARP_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_SHARP_STAGE_COM0:
        case IR_SHARP_STAGE_COM1:
        case IR_SHARP_STAGE_COM2:
        case IR_SHARP_STAGE_COM3:
        case IR_SHARP_STAGE_COM4:
        case IR_SHARP_STAGE_COM5:
        case IR_SHARP_STAGE_DEF0:
        case IR_SHARP_STAGE_DEF1:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SHARP_data.uc_main_stage + 1;

        	ui2_set_bit = (1 << (tg_ir_SHARP_data.uc_main_stage-IR_SHARP_STAGE_COM0+8));

            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_2T) < SHARP_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                	tg_ir_SHARP_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-SHARP_6T) < SHARP_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                	tg_ir_SHARP_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;
        case IR_SHARP_STAGE_EXT0:
        case IR_SHARP_STAGE_EXT1:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SHARP_data.uc_main_stage + 1;

            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_2T) < SHARP_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    if(tg_ir_SHARP_data.uc_main_stage == IR_SHARP_STAGE_EXT1)
                    {
                        tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                    }
                }
                else if(abs(i2_diff_time-SHARP_6T) < SHARP_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    if(tg_ir_SHARP_data.uc_main_stage == IR_SHARP_STAGE_EXT0)
                    {
                        tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                    }
                }
            }
            break;
        case IR_SHARP_STAGE_I_STP0:
            /* set STEP GAP condition */
            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_125T) < SHARP_125T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;
                    tg_ir_SHARP_data.uc_main_stage = IR_SHARP_STAGE_ADDR0A;
                }
            }
            break;
        case IR_SHARP_STAGE_ADDR0A:
        case IR_SHARP_STAGE_ADDR1A:
        case IR_SHARP_STAGE_ADDR2A:
        case IR_SHARP_STAGE_ADDR3A:
        case IR_SHARP_STAGE_ADDR4A:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SHARP_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_SHARP_data.uc_main_stage-IR_SHARP_STAGE_ADDR0A));

            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_2T) < SHARP_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    tg_ir_SHARP_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;

                }
                else if(abs(i2_diff_time-SHARP_6T) < SHARP_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    tg_ir_SHARP_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;
        case IR_SHARP_STAGE_C_COMM0:
        case IR_SHARP_STAGE_C_COMM1:
        case IR_SHARP_STAGE_C_COMM2:
        case IR_SHARP_STAGE_C_COMM3:
        case IR_SHARP_STAGE_C_COMM4:
        case IR_SHARP_STAGE_C_COMM5:
        case IR_SHARP_STAGE_C_DEF0:
        case IR_SHARP_STAGE_C_DEF1:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SHARP_data.uc_main_stage + 1;

        	ui2_set_bit = (1 << (tg_ir_SHARP_data.uc_main_stage-IR_SHARP_STAGE_C_COMM0));

            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_2T) < SHARP_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                	tg_ir_SHARP_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-SHARP_6T) < SHARP_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                	tg_ir_SHARP_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;
        case IR_SHARP_STAGE_C_EXT0:
        case IR_SHARP_STAGE_C_EXT1:
            /* set initial condition */
            uc_next_main_stage = tg_ir_SHARP_data.uc_main_stage + 1;

            if(tg_ir_SHARP_data.uc_sub_stage == 0 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-SHARP_1T) < SHARP_1T_TOL)
                {
                    tg_ir_SHARP_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_SHARP_data.uc_sub_stage == 1 &&
               (bLevel & SHARP_LEVEL_BIT) == SHARP_LEVEL_LOW)
            {
                if(abs(i2_diff_time-SHARP_2T) < SHARP_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    if(tg_ir_SHARP_data.uc_main_stage == IR_SHARP_STAGE_C_EXT0)
                    {
                        tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                    }
                }
                else if(abs(i2_diff_time-SHARP_6T) < SHARP_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_SHARP_data.uc_sub_stage = 0;

                    if(tg_ir_SHARP_data.uc_main_stage == IR_SHARP_STAGE_C_EXT1)
                    {
                        tg_ir_SHARP_data.uc_main_stage = uc_next_main_stage;
                    }
                }
            }
            break;

        case IR_SHARP_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_SHARP_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_SHARP_data.uc_main_stage != IR_SHARP_STAGE_FINISH)
    {
        printf("s %d, \a", tg_ir_SHARP_data.uc_main_stage);
        printf("    %d \n",wTimeStamp);
    }
#endif

    if(tg_ir_SHARP_data.uc_main_stage == IR_SHARP_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Ir_dec_Matsushita_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

    /*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
    */
    i2_diff_time = wTimeStamp;

    switch(tg_ir_MAT_data.uc_main_stage)
    {
        case IR_MAT_STAGE_LP:
            if(tg_ir_MAT_data.uc_sub_stage == 0 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-MAT_8T) < MAT_8T_TOL)
                {
                    tg_ir_MAT_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_MAT_data.uc_sub_stage == 1 &&
                    (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_LOW)
            {
                if(abs(i2_diff_time-MAT_8T) < MAT_8T_TOL)
                {
                    tg_ir_MAT_data.uc_sub_stage = 0;
                    tg_ir_MAT_data.uc_main_stage = IR_MAT_STAGE_ADDR0;
                }
            }
            break;

        case IR_MAT_STAGE_ADDR0:
        case IR_MAT_STAGE_ADDR1:
        case IR_MAT_STAGE_ADDR2:
        case IR_MAT_STAGE_ADDR3:
        case IR_MAT_STAGE_ADDR4:
        case IR_MAT_STAGE_ADDR5:
            /* set initial condition */
            uc_next_main_stage = tg_ir_MAT_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_MAT_data.uc_main_stage-IR_MAT_STAGE_ADDR0+8));

            if(tg_ir_MAT_data.uc_sub_stage == 0 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    tg_ir_MAT_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_MAT_data.uc_sub_stage == 1 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_LOW)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;

                }
                else if(abs(i2_diff_time-MAT_6T) < MAT_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_MAT_STAGE_COMM0:
        case IR_MAT_STAGE_COMM1:
        case IR_MAT_STAGE_COMM2:
        case IR_MAT_STAGE_COMM3:
        case IR_MAT_STAGE_COMM4:
        case IR_MAT_STAGE_COMM5:
            /* set initial condition */
            uc_next_main_stage = tg_ir_MAT_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_MAT_data.uc_main_stage-IR_MAT_STAGE_COMM0+8));

            if(tg_ir_MAT_data.uc_sub_stage == 0 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    tg_ir_MAT_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_MAT_data.uc_sub_stage == 1 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_LOW)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-MAT_6T) < MAT_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_MAT_STAGE_C_ADDR0:
        case IR_MAT_STAGE_C_ADDR1:
        case IR_MAT_STAGE_C_ADDR2:
        case IR_MAT_STAGE_C_ADDR3:
        case IR_MAT_STAGE_C_ADDR4:
        case IR_MAT_STAGE_C_ADDR5:
            /* set initial condition */
            uc_next_main_stage = tg_ir_MAT_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_MAT_data.uc_main_stage-IR_MAT_STAGE_C_ADDR0));

            if(tg_ir_MAT_data.uc_sub_stage == 0 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    tg_ir_MAT_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_MAT_data.uc_sub_stage == 1 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_LOW)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-MAT_6T) < MAT_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_MAT_STAGE_C_COMM0:
        case IR_MAT_STAGE_C_COMM1:
        case IR_MAT_STAGE_C_COMM2:
        case IR_MAT_STAGE_C_COMM3:
        case IR_MAT_STAGE_C_COMM4:
        case IR_MAT_STAGE_C_COMM5:
            /* set initial condition */
            uc_next_main_stage = tg_ir_MAT_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_MAT_data.uc_main_stage-IR_MAT_STAGE_C_COMM0));

            if(tg_ir_MAT_data.uc_sub_stage == 0 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    tg_ir_MAT_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_MAT_data.uc_sub_stage == 1 &&
               (bLevel & MAT_LEVEL_BIT) == MAT_LEVEL_LOW)
            {
                if(abs(i2_diff_time-MAT_2T) < MAT_2T_TOL)
                {
                    /* level 0 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-MAT_6T) < MAT_6T_TOL)
                {
                    /* level 1 */
                    tg_ir_MAT_data.uc_sub_stage = 0;

                    tg_ir_MAT_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_MAT_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_MAT_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_MAT_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_MAT_data.uc_main_stage != IR_MAT_STAGE_LP)
    {
        printf("\r\n tg_ir_MAT_data.uc_main_stage = %d", tg_ir_MAT_data.uc_main_stage);
    }
#endif

    if(tg_ir_MAT_data.uc_main_stage == IR_MAT_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}




BOOL Ir_dec_PANA_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

    /*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
    */
    i2_diff_time = wTimeStamp;

    switch(tg_ir_PANA_data.uc_main_stage)
    {
        case IR_PANA_STAGE_LP:
            if(tg_ir_PANA_data.uc_sub_stage == 0 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-PANA_8T) < PANA_8T_TOL)
                {
                    tg_ir_PANA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_PANA_data.uc_sub_stage == 1 &&
                    (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-PANA_4T) < PANA_4T_TOL)
                {
                    tg_ir_PANA_data.uc_sub_stage = 0;
                    tg_ir_PANA_data.uc_main_stage = IR_PANA_STAGE_MC0;
                }
            }
            break;

        case IR_PANA_STAGE_MC0:
        case IR_PANA_STAGE_MC1:
        case IR_PANA_STAGE_MC2:
        case IR_PANA_STAGE_MC3:
        case IR_PANA_STAGE_MC4:
        case IR_PANA_STAGE_MC5:
        case IR_PANA_STAGE_MC6:
        case IR_PANA_STAGE_MC7:
        case IR_PANA_STAGE_MC8:
        case IR_PANA_STAGE_MC9:
        case IR_PANA_STAGE_MCA:
        case IR_PANA_STAGE_MCB:
        case IR_PANA_STAGE_MCC:
        case IR_PANA_STAGE_MCD:
        case IR_PANA_STAGE_MCE:
        case IR_PANA_STAGE_MCF:

            /* set initial condition */
            uc_next_main_stage = tg_ir_PANA_data.uc_main_stage + 1;
//            ui2_set_bit = (1 << (tg_ir_PANA_data.uc_main_stage-IR_PANA_STAGE_MC0));

            if(tg_ir_PANA_data.uc_sub_stage == 0 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    tg_ir_PANA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_PANA_data.uc_sub_stage == 1 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

//                    tg_ir_PANA_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;

                }
                else if(abs(i2_diff_time-PANA_3T) < PANA_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

//                    tg_ir_PANA_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_PANA_STAGE_MCP0:
        case IR_PANA_STAGE_MCP1:
        case IR_PANA_STAGE_MCP2:
        case IR_PANA_STAGE_MCP3:
        case IR_PANA_STAGE_SYS0:
        case IR_PANA_STAGE_SYS1:
        case IR_PANA_STAGE_SYS2:
        case IR_PANA_STAGE_SYS3:

            /* set initial condition */
            uc_next_main_stage = tg_ir_PANA_data.uc_main_stage + 1;
            ui2_set_bit = (1 << ((tg_ir_PANA_data.uc_main_stage-IR_PANA_STAGE_MCP0)+8) );

            if(tg_ir_PANA_data.uc_sub_stage == 0 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    tg_ir_PANA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_PANA_data.uc_sub_stage == 1 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;

                }
                else if(abs(i2_diff_time-PANA_3T) < PANA_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_PANA_STAGE_EQUIP0:
        case IR_PANA_STAGE_EQUIP1:
        case IR_PANA_STAGE_EQUIP2:
        case IR_PANA_STAGE_EQUIP3:
        case IR_PANA_STAGE_EQUIP4:
        case IR_PANA_STAGE_EQUIP5:
        case IR_PANA_STAGE_EQUIP6:
        case IR_PANA_STAGE_EQUIP7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_PANA_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_PANA_data.uc_main_stage-IR_PANA_STAGE_EQUIP0));

            if(tg_ir_PANA_data.uc_sub_stage == 0 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    tg_ir_PANA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_PANA_data.uc_sub_stage == 1 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_mode_addr &= ~ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-PANA_3T) < PANA_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_mode_addr |= ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_PANA_STAGE_COMM0:
        case IR_PANA_STAGE_COMM1:
        case IR_PANA_STAGE_COMM2:
        case IR_PANA_STAGE_COMM3:
        case IR_PANA_STAGE_COMM4:
        case IR_PANA_STAGE_COMM5:
        case IR_PANA_STAGE_COMM6:
        case IR_PANA_STAGE_COMM7:

            /* set initial condition */
            uc_next_main_stage = tg_ir_PANA_data.uc_main_stage + 1;
            ui2_set_bit = (1 << ((tg_ir_PANA_data.uc_main_stage-IR_PANA_STAGE_COMM0)+8) );

            if(tg_ir_PANA_data.uc_sub_stage == 0 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    tg_ir_PANA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_PANA_data.uc_sub_stage == 1 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-PANA_3T) < PANA_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_PANA_STAGE_PARITY0:
        case IR_PANA_STAGE_PARITY1:
        case IR_PANA_STAGE_PARITY2:
        case IR_PANA_STAGE_PARITY3:
        case IR_PANA_STAGE_PARITY4:
        case IR_PANA_STAGE_PARITY5:
        case IR_PANA_STAGE_PARITY6:
        case IR_PANA_STAGE_PARITY7:

            /* set initial condition */
            uc_next_main_stage = tg_ir_PANA_data.uc_main_stage + 1;
            ui2_set_bit = (1 << (tg_ir_PANA_data.uc_main_stage-IR_PANA_STAGE_PARITY0));

            if(tg_ir_PANA_data.uc_sub_stage == 0 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    tg_ir_PANA_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_PANA_data.uc_sub_stage == 1 &&
               (bLevel & PANA_LEVEL_BIT) == PANA_LEVEL_LOW)
            {
                if(abs(i2_diff_time-PANA_1T) < PANA_1T_TOL)
                {
                    /* level 0 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_ctrl_info &= ~ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-PANA_3T) < PANA_3T_TOL)
                {
                    /* level 1 */
                    tg_ir_PANA_data.uc_sub_stage = 0;

                    tg_ir_PANA_data.ui2_ctrl_info |= ui2_set_bit;

                    tg_ir_PANA_data.uc_main_stage = uc_next_main_stage;
                }
            }
            break;

        case IR_PANA_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_PANA_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_PANA_data.uc_main_stage != IR_PANA_STAGE_LP)
    {
        printf("tg_ir_PANA_data.uc_main_stage = %d\a", tg_ir_PANA_data.uc_main_stage);
        printf("    %d\r\n",wTimeStamp);
    }
#endif

    if(tg_ir_PANA_data.uc_main_stage == IR_PANA_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Ir_dec_RCMM_process (BYTE bLevel, WORD wTimeStamp)
{
    int   i2_diff_time;
    BYTE  uc_next_main_stage;
    WORD  ui2_set_bit;

    /*
        if(wTimeStamp > tg_ir_rc6_data.ui2_old_time_tick)
        {
            i2_diff_time = wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
        else
        {
            i2_diff_time = 0x10000 + wTimeStamp - tg_ir_rc6_data.ui2_old_time_tick;
        }
    */
    i2_diff_time = wTimeStamp;
    if (wTimeStamp > RCMM_GAP)
    {
    	tg_ir_RCMM_data.uc_main_stage = 0;
    }

    switch(tg_ir_RCMM_data.uc_main_stage)
    {
        case IR_RCMM_STAGE_LP:
            if(tg_ir_RCMM_data.uc_sub_stage == 0 &&
               (bLevel & RCMM_LEVEL_BIT) == RCMM_LEVEL_HIGH)
            {
                if((abs(i2_diff_time-RCMM_20T) < RCMM_HEADER_TOL)||(abs(i2_diff_time-RCMM_15T) < RCMM_HEADER_TOL))
                {
                    tg_ir_RCMM_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_RCMM_data.uc_sub_stage == 1 &&
                    (bLevel & RCMM_LEVEL_BIT) == RCMM_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RCMM_20T) < RCMM_HEADER_TOL)
                {
                    tg_ir_RCMM_data.uc_sub_stage = 0;
                    tg_ir_RCMM_data.uc_main_stage = IR_RCMM_STAGE_MC0;
                    tg_ir_RCMM_data.ui2_mode_addr = 0;
                    tg_ir_RCMM_data.ui2_ctrl_info = 0;
                    RCMM_Key = 0;
                }
                else if(abs(i2_diff_time-RCMM_10T) < RCMM_HEADER_TOL)
                {
                    tg_ir_RCMM_data.uc_sub_stage = 0;
                    tg_ir_RCMM_data.uc_main_stage = IR_RCMM_STAGE_MC0;
                    tg_ir_RCMM_data.ui2_mode_addr = 0;
                    tg_ir_RCMM_data.ui2_ctrl_info = 0;
                    RCMM_Key = 1;
                }
            }
            break;

        case IR_RCMM_STAGE_MC0:
        case IR_RCMM_STAGE_MC1:
        case IR_RCMM_STAGE_MC2:
        case IR_RCMM_STAGE_MC3:
        case IR_RCMM_STAGE_MC4:
        case IR_RCMM_STAGE_MC5:
        case IR_RCMM_STAGE_MC6:
        case IR_RCMM_STAGE_MC7:
            /* set initial condition */
            uc_next_main_stage = tg_ir_RCMM_data.uc_main_stage + 1;
//            ui2_set_bit = (1 << (tg_ir_RCMM_data.uc_main_stage-IR_RCMM_STAGE_MC0));

            if(tg_ir_RCMM_data.uc_sub_stage == 0 &&
               (bLevel & RCMM_LEVEL_BIT) == RCMM_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RCMM_6T) < RCMM_DATA_H_TOL)
                {
                    tg_ir_RCMM_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_RCMM_data.uc_sub_stage == 1 &&
               (bLevel & RCMM_LEVEL_BIT) == RCMM_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RCMM_10T) < RCMM_00_TOL)		//00  8.89
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_mode_addr = tg_ir_RCMM_data.ui2_mode_addr << 2;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCMM_16T) < RCMM_01_TOL)		//01  12.22
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_mode_addr = tg_ir_RCMM_data.ui2_mode_addr << 2;
                    tg_ir_RCMM_data.ui2_mode_addr |= 0x01;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCMM_22T) < RCMM_10_TOL)		//10   15.55
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_mode_addr = tg_ir_RCMM_data.ui2_mode_addr << 2;
                    tg_ir_RCMM_data.ui2_mode_addr |= 0x02;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCMM_28T) < RCMM_11_TOL)		//11	18.89
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_mode_addr = tg_ir_RCMM_data.ui2_mode_addr << 2;
                    tg_ir_RCMM_data.ui2_mode_addr |= 0x03;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }

            }
            break;

        case IR_RCMM_STAGE_MC8:
        case IR_RCMM_STAGE_MC9:
        case IR_RCMM_STAGE_MCA:
        case IR_RCMM_STAGE_MCB:
        case IR_RCMM_STAGE_MCC:
        case IR_RCMM_STAGE_MCD:
        case IR_RCMM_STAGE_MCE:
        case IR_RCMM_STAGE_MCF:
            /* set initial condition */
            uc_next_main_stage = tg_ir_RCMM_data.uc_main_stage + 1;
//            ui2_set_bit = (1 << (tg_ir_RCMM_data.uc_main_stage-IR_RCMM_STAGE_MC8));

            if(tg_ir_RCMM_data.uc_sub_stage == 0 &&
               (bLevel & RCMM_LEVEL_BIT) == RCMM_LEVEL_HIGH)
            {
                if(abs(i2_diff_time-RCMM_6T) < RCMM_DATA_H_TOL)
                {
                    tg_ir_RCMM_data.uc_sub_stage = 1;
                }
            }
            else if(tg_ir_RCMM_data.uc_sub_stage == 1 &&
               (bLevel & RCMM_LEVEL_BIT) == RCMM_LEVEL_LOW)
            {
                if(abs(i2_diff_time-RCMM_10T) < RCMM_00_TOL)		//00  8.89
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_ctrl_info = tg_ir_RCMM_data.ui2_ctrl_info << 2;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCMM_16T) < RCMM_01_TOL)		//01  12.22
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_ctrl_info = tg_ir_RCMM_data.ui2_ctrl_info << 2;
                    tg_ir_RCMM_data.ui2_ctrl_info |= 0x01;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCMM_22T) < RCMM_10_TOL)		//10   15.55
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_ctrl_info = tg_ir_RCMM_data.ui2_ctrl_info << 2;
                    tg_ir_RCMM_data.ui2_ctrl_info |= 0x02;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }
                else if(abs(i2_diff_time-RCMM_28T) < RCMM_11_TOL)		//11	18.89
                {
                    /* level 1 */
                    tg_ir_RCMM_data.uc_sub_stage = 0;

                    tg_ir_RCMM_data.ui2_ctrl_info = tg_ir_RCMM_data.ui2_ctrl_info << 2;
                    tg_ir_RCMM_data.ui2_ctrl_info |= 0x03;

                    tg_ir_RCMM_data.uc_main_stage = uc_next_main_stage;
                }

            }
            break;


        case IR_RCMM_STAGE_FINISH:
            break;

        default:
            break;
    }

    tg_ir_RCMM_data.ui2_old_time_tick = wTimeStamp;

#if 0
    if(tg_ir_RCMM_data.uc_main_stage != IR_RCMM_STAGE_LP)
    {
        //UARTputhex(tg_ir_RCMM_data.uc_main_stage);
		//putchr('0'+bLevel);
		//printf(":%d\r\n", (wTimeStamp));
    }
#endif

    if(tg_ir_RCMM_data.uc_main_stage == IR_RCMM_STAGE_FINISH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


#endif  /* LITE_VERSION */

//void ReplayWave(void)
//{
//	Word wValue,wNext;
//
//	IR_LED_OUT_HIGH;
//	IR_OUT_LOW;
//        while (bIrTimeIndexIn_Output != bIrTimeIndexOut_Output)
//        {
//			wValue = sRC_TIMETABLE_Output[bIrTimeIndexOut_Output].wTimeStamp;
//        	if(sRC_TIMETABLE_Output[bIrTimeIndexOut_Output].bLevel)
//            {
//              	IR_OUT_LOW;
//            }
//            else
//            {
//               	IR_OUT_HIGH;
//            }
//            if(++bIrTimeIndexOut_Output >= IR_TBL_MAX) bIrTimeIndexOut_Output = 0;
//			wNext = sRC_TIMETABLE_Output[bIrTimeIndexOut_Output].wTimeStamp;
//			if(wNext < wValue) wNext += 0x10000;
//			Delayus(wNext-wValue);
//        }
//	IR_OUT_LOW;
//	IR_LED_OUT_LOW;
//}

void ScanIR(Byte bLevel, Word wTimeStamp )
{
    WORD  		ui2_mode_addr,ui2_ctrl_info, bParity;
    uint32_t 	Longdata;


	if(Ir_dec_rc6_process(bLevel, wTimeStamp))
	      {
	        if(Get_ir_dec_rc6_code(&ui2_mode_addr, &ui2_ctrl_info))
	            {
	                ShowIRCode("RC6",ui2_mode_addr,ui2_ctrl_info);
	            }
	      }

	 if(Ir_dec_NEC_process(bLevel, wTimeStamp))
	     {
	         if(Get_ir_dec_NEC_code(&ui2_mode_addr, &ui2_ctrl_info))
	              {
						bParity = (ui2_ctrl_info>>8)+ (ui2_ctrl_info & 0xFF);
						if (bParity == 0xFF)
						{
							ShowIRCode("NEC",ui2_mode_addr,ui2_ctrl_info);
						}
	              }
	      }

	 if(Ir_dec_RCA_process(bLevel, wTimeStamp))
	     {
	         if(Get_ir_dec_RCA_code(&ui2_mode_addr, &ui2_ctrl_info))
	         {
	        	 bParity = ui2_mode_addr & 0x000F;
	        	 bParity = (bParity << 8) + (ui2_ctrl_info >> 8);
	        	 bParity += (ui2_mode_addr >> 4);
	        	 if (bParity == 0xFFF)
	              	  {
							ShowIRCode("RCA",ui2_mode_addr,ui2_ctrl_info); //MSB
	              	  }
	         }
	      }

	 if(Ir_dec_rc5x_process(bLevel, wTimeStamp))
     {
         if(Get_ir_dec_rc5x_code(&ui2_mode_addr, &ui2_ctrl_info))
         {
        	 ShowIRCode("RC5",ui2_mode_addr,ui2_ctrl_info);
         }
     }

     if(Ir_dec_SONY_process(bLevel, wTimeStamp))
     {
         if(Get_ir_dec_SONY_code(&ui2_mode_addr, &ui2_ctrl_info))
         {
        	 ShowIRCode("SNY",ui2_mode_addr,ui2_ctrl_info);
         }
     }

     if(Ir_dec_SHARP_process(bLevel, wTimeStamp))
     {
         if(Get_ir_dec_SHARP_code(&ui2_mode_addr, &ui2_ctrl_info))
         {
				bParity = (ui2_ctrl_info>>8)+ (ui2_ctrl_info & 0xFF);
				if (bParity == 0xFF)
				{
					ShowIRCode("SHA",ui2_mode_addr,ui2_ctrl_info);
				}
         }
     }

     if(Ir_dec_Matsushita_process(bLevel, wTimeStamp))
     {
         if(Get_ir_dec_Matsushita_code(&ui2_mode_addr, &ui2_ctrl_info))
         {
				bParity = (ui2_ctrl_info>>8)+ (ui2_ctrl_info & 0xFF);
				if (bParity == 0x3F)
				{
					ShowIRCode("MAT",ui2_mode_addr,ui2_ctrl_info);
				}
         }
     }

     if(Ir_dec_PANA_process(bLevel, wTimeStamp))
      {
          if(Get_ir_dec_PANA_code(&ui2_mode_addr, &ui2_ctrl_info))
          {
				bParity = 0x00;
				bParity ^= ui2_mode_addr>>8;
				bParity ^= ui2_mode_addr &0xFF;
				bParity ^= ui2_ctrl_info>>8;
				if (bParity == (ui2_ctrl_info & 0xFF))
				{
					ShowIRCode("PAN",ui2_mode_addr,ui2_ctrl_info);
				}
          }
      }

     if(Ir_dec_RCMM_process(bLevel, wTimeStamp))
      {
          if(Get_ir_dec_RCMM_code(&ui2_mode_addr, &ui2_ctrl_info))
          {
//        	  ShowIRCode("RCM",ui2_mode_addr,ui2_ctrl_info);
        	  Longdata = ui2_mode_addr;
        	  Longdata <<= 16;
        	  Longdata |= ui2_ctrl_info;
        	  if (RCMM_Key)
        	  {
        		  ShowIRCode("RCM",ui2_mode_addr,ui2_ctrl_info);
        	  }
        	  else
        	  {
        		  if (Longdata != Old_Longdata)
        		  {
        			  if (ui2_mode_addr&0x10) UARTputstr("LL");			//Left key
        			  UARTprintf("%d:", ((Longdata>>10)&0x3FF));
        			  UARTprintf("%d\r\n", (Longdata &0x3FF));
        			  Old_Longdata = Longdata;
        	  	  }
        		  else
        			  UARTputstr(".");
        	  }
//        	  Toggle_LED();
          }
      }
     RestartClearIrDecDataTimer();
}

void ShowIRCode(char *IR_Code, WORD mode_addr, WORD ctrl_info)
{
	ShowIRFrq(FreqAverage);

	UARTputstr("$IRK");
	UARTputchar(11);
	UARTputstr(IR_Code);

//	                    printf("%x \a", (ui2_mode_addr>>8));
    UARTprintf("%4X", mode_addr);
//	                    printf("%x \a", (ui2_ctrl_info>>8));
    UARTprintf("%4X\r\n", ctrl_info);
//                      UARTputstr("\r\n");
}

void ShowIRFrq(WORD IR_frq)
{
	UARTputstr("$IRF");
	UARTputchar(11);
	UARTputstr("FRQ");

//	                    printf("%x \a", (ui2_mode_addr>>8));
    UARTprintf("%4X", IR_frq);
//	                    printf("%x \a", (ui2_ctrl_info>>8));
    UARTprintf("%4X\r\n", IR_frq);
//                      UARTputstr("\r\n");
}
