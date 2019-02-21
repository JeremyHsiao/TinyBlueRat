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

#include "board.h"
#include "chip.h"
#include  "string.h"
#include  "gpio.h"

// System

// Common
#include "Define.h"

// Internal
#define  GLOBAL
#include "IrAuto.h"
#undef GLOBAL

// External
#define  GLOBAL extern
#include "global.h"
#include "app_uart.h"
#include "version.h"
#include "Io.h"
#include "Nvram_Access.h"
#include "Keyscan.h"
#include "Ir.h"
#include "Initial.h"
#include "IrMacro.h"
#include "Timeout.h"
#undef GLOBAL


void CheckMode(void)
{
#ifdef FRC
    bFunctionMode = IR_MARCO_MODE;
#else
/*
    Byte i;
    Byte code aKeyCheck[5] = {_BIT4, _BIT5, _BIT6, _BIT7, _BIT3};
    bFunctionMode = IR_PC_SRIPT_MODE;

    for (i=0; i < 5; i++)
    {
        if((PortA&aKeyCheck[i]) == 0)
        {
            bFunctionMode = _BIT0 << i;  
            break;
        }
    }
*/
/*	DQE RC
	Byte bPA;
	bPA = PortA & 0xF8;
	
	if (bPA == 0xE8) bFunctionMode = IR_ENC_MODE;
	else if (bPA == 0xD8)  bFunctionMode = IR_DEC_MODE ;
	else if (bPA == 0xB8)  bFunctionMode = IR_AUTO_MODE ;
	else if (bPA == 0x78)  bFunctionMode = IR_MARCO_MODE ;
//	else if (bPA == 0xF0)  bFunctionMode = IR_LEARNING_MODE ;
//	else if (bPA == 0xE0)  bFunctionMode = IR_SIMU_MODE ;
	else  bFunctionMode = IR_PC_SRIPT_MODE ;
*/  
    uint8_t		I2CRxd[1];
    ReadEEPROM(MODE_ADDR, I2CRxd, 1);
	bFunctionMode = I2CRxd[0];
//    bFunctionMode = IR_ENC_MODE;		// For DQE
#endif    
    UARTprintf ("\r\n bFunctionMode = %x ", bFunctionMode);
}

void ScriptVarIni(void)
{
#if 0
    StoreEEPROM(SCRP_TYPE_ADDR, 0x10);
    StoreEEPROM(SCRP_DELAY_H_ADDR, 0x00);
    StoreEEPROM(SCRP_DELAY_L_ADDR, 90);
    StoreEEPROM(SCRP_NUMS_ADDR, 5);
#endif

    /* Type, Group-Delay, Numbers */

    uint8_t		I2CRxd[4];

    ReadEEPROM(SCRP_TYPE_ADDR, I2CRxd, 4);
    
    sSCIPT_HEADER.bType         = I2CRxd[0];
    sSCIPT_HEADER.wGrpDelayTime = (unsigned int) (I2CRxd[1] << 8) | I2CRxd[2];
    sSCIPT_HEADER.bNums         = I2CRxd[3];
    
//    printf("\r\n sSCIPT_HEADER.bType = %x", sSCIPT_HEADER.bType);
//    printf("\r\n sSCIPT_HEADER.wGrpDelayTime = %x", sSCIPT_HEADER.wGrpDelayTime);
//    printf("\r\n sSCIPT_HEADER.bNums = %x", sSCIPT_HEADER.bNums);
}

void Scripttest(void)
{
#if 0 
    Byte i,j;
    Byte code testA[5][4]={
                            {14,10,0x00,10}, //menu      
                            {27,2,0x00,10}, //      
                            {28,3,0,10},       
                            {20,3,0,10},
       					    {21,1,0,10},
                           };


    for(i=0; i < 5; i++)
        for (j=0; j < 4; j++)
        StoreEEPROM(SCRP_KEYSTART_ADDR+i*4+j, testA[i][j]);
#endif

}


void ScriptRamIni(void)
{
    Byte bIRKeyindex;
    Byte bIRRepeat;
    Byte bIRKeyDelayHi;
    Byte bIRKeyDelayLo;
    Byte i;
    uint8_t		I2CRxd[8];

    for(i = 0; i < sSCIPT_HEADER.bNums; i++)
    {
        /* Key-Index, repeat, Step-Delay */
        
        ReadEEPROM(SCRP_KEYSTART_ADDR + i*4, I2CRxd, 4);

        bIRKeyindex = I2CRxd[0] - 1;
        bIRKeyindex %= IR_RAM_TBL_MAX;

        bIRRepeat       = I2CRxd[1];
        bIRKeyDelayHi   = I2CRxd[2];
        bIRKeyDelayLo   = I2CRxd[3];
        
        ReadEEPROM(KEY_RAMSTAR_ADDR + bIRKeyindex*8, I2CRxd, 8);
        sIR_RAM_TBL[i].Type     = I2CRxd[0];
        sIR_RAM_TBL[i].Mode     = I2CRxd[1];
        sIR_RAM_TBL[i].Address  = I2CRxd[2];
        sIR_RAM_TBL[i].Cmd      = I2CRxd[3];
        sIR_RAM_TBL[i].Repeat   = bIRRepeat;
        sIR_RAM_TBL[i].Rev0     = bIRKeyDelayHi;
        sIR_RAM_TBL[i].Rev1     = bIRKeyDelayLo;
        sIR_RAM_TBL[i].Rev2     = 0;

//        printf("\r\n  sIR_RAM_TBL[i].Rev0 = %x", sIR_RAM_TBL[i].Rev0);
//        printf("\r\n  sIR_RAM_TBL[i].Rev1 = %x", sIR_RAM_TBL[i].Rev1);

//        ClearWatchdogTimer();
    }
}

void IR_AutoScripProc(void)
{
    Byte    bRepeatKeyFlag;
    
    bRepeatKeyFlag  = FALSE;
    
    if(bTime_100ms_Flag)
    {
        bTime_100ms_Flag = 0;
        
        if(sIR_STATUS.bScriptNum < sSCIPT_HEADER.bNums) /* key step delay */
        {
            if(++sIR_STATUS.wScriptTime >= sIR_STATUS.wScriptStepDelay)
            {
                sIR_STATUS.wScriptTime = 0;

                if(++sIR_STATUS.bScriptNum < sSCIPT_HEADER.bNums)
                {
                    IR_TimeTBL(sIR_STATUS.bScriptNum, TRUE);      /* next script */
                    
                    bRepeatTimeFlag = 0;
                    
//                    printf("\r\n  IR_AutoScripProc() bScriptNum = %x", sIR_STATUS.bScriptNum);
                }
            }
        }
        else    /* group delay */
        {
            if(++sIR_STATUS.wScriptTime >= sSCIPT_HEADER.wGrpDelayTime)
            {
                sIR_STATUS.wScriptTime = 0;

                RestartAutoScriptTimer();   /* next script */
                
//                printf("\r\n  IR_AutoScripProc() bScriptNum = %x", sIR_STATUS.bScriptNum);
            }
        }
    }
    
    if(sIR_STATUS.bScriptNum < sSCIPT_HEADER.bNums)
    {
        if(bRepeatTimeFlag)
        {
            bRepeatTimeFlag = 0;
            
            if(sIR_STATUS.Repeat == 0)      /* infinitely */
            {
                bRepeatKeyFlag = TRUE;
            }
            else if(sIR_STATUS.RepeatCountDown)
            {
                sIR_STATUS.RepeatCountDown--;
                
                bRepeatKeyFlag = TRUE;
            }
        }
        
        /* ready to send the real signal */
        if(bRepeatKeyFlag == TRUE)
        {
//            printf("\r\n  IR_AutoScripProc() Rpt: bScriptNum = %x", sIR_STATUS.bScriptNum);
            
            IR_TimeTBL(0, FALSE);   /* follow the previous key */
        }
    }
}

void Ir_Mode_Ini(Byte bFunMode)
{
    switch(bFunMode)
    {
        case IR_PC_SRIPT_MODE:
		case IR_ENC_MODE:
		
            CPU_IR_Encoder_Setting();
            
//            ClearWatchdogTimer();
            break;
            
        case IR_DEC_MODE:
            CPU_IR_Decoder_Setting();    
            break;
            
        case IR_AUTO_MODE:
            CPU_IR_Encoder_Setting(); 
            
            ScriptVarIni();
            Scripttest();
  		    ScriptRamIni();

            RestartAutoScriptTimer();
            break;
            
        case IR_MARCO_MODE:
            CPU_IR_Encoder_Setting(); 
            
            StopMacro();
            Macrotest();
            break;
           
        case IR_LEARNING_MODE:
            CPU_IR_Learning_Setting();
            break;

        case IR_SIMU_MODE:
            CPU_IR_Simu_Setting();
            break;

		case IR_SELFTEST_MODE:
//            CPU_IR_Decoder_Setting();
//			IR_Carrier(36000);	
			break;
           
        default:
			putstr("\r\n not correct mode in Ir_Mode_Ini");
            return;     
    }

//    Set_Mode_LED();
}

void Initial_Mode_LED(void)
{
	bModeLEDFlashTimes  = 0;
}

void Set_Mode_LED(void)
{
    Byte bMode;
    
    if(!bFunctionMode)  /* prevent the NULL mode */
    {
        return;
    }
    
    bMode = bFunctionMode;
    
    bModeLEDFlashTimes = 0;
    do
    {
        bModeLEDFlashTimes++;
        
        if((bMode&0x01) == 0x01)
        {
            break;
        }
        
        bMode >>= 1;
    }while(1);
    
    bModeLEDFlashTimes  <<= 1;
    bModeLEDflag        = 0;
    
    IR_LED_OUT_LOW;
}

/*
void Mode_LED_Proc(void)
{
    if(bModeLEDFlashTimes)
    {
        if(bModeLEDflag)
        {
            bModeLEDFlashTimes--;
            
            bModeLEDflag = 0;
            
            IR_LED_OUT_TOGGLE;
        }
    }
}
*/
