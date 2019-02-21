/*==========================================================================
|
|  Project Name : LCD TV RC controller
|
|  File Name	: IrMarco.C
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
// System

// Common
#include "Define.h"

// Internal
#define  GLOBAL
#include "IrMacro.h"
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
#include "Irauto.h"
#include "Timeout.h"
#undef GLOBAL

#ifndef  LITE_VERSION
void IrMacoKeyScan(void)
{
    static signed char  bPrevFntKey;
    static Byte         KeyRepeatCounter;
    static Byte         bDebounce_Cnt;
    Byte        bGetKeyFlag;
    signed char bFntKey;
    
    bGetKeyFlag     = FALSE;

    if (!KeyScanTimer)
    {
        KeyScanTimer = wTimer10ms;
        
        bFntKey = GetKey();
        
        if (bFntKey == -1)  /* Get null key */
        {
            bDebounce_Cnt       = 0;
            KeyRepeatCounter    = 0;
        }
        else    /* Get right key */
        {
            if (bFntKey == bPrevFntKey)
            {
                if(++bDebounce_Cnt >= KEY_DEBOUCE_TIMES)    /* debounce */
                {
                    bDebounce_Cnt = KEY_DEBOUCE_TIMES;
                    bGetKeyFlag = TRUE;
                }
            }
        }
        
        bPrevFntKey = bFntKey;
    }

    if(bGetKeyFlag == TRUE) /* run each 10 ms */
    {
        if(KeyRepeatCounter == 0)   /* first pressed */
        {
            KeyRepeatCounter = 1;   /* no repeat key */
            
            IrMacroRAMIni(bPrevFntKey);
            
            StartMacro();

            StartMacroKeyTimer();
            
            putstr("\r\n IrMacoKeyScan() First key");
        }
    }
}
#endif  /* LITE_VERSION */ 

void IrMacroRAMIni(Byte bIndex)
{
    Byte 		i;
    uint8_t		I2CRxd[8];

    if(bIndex > IR_MACRO_KEY_MAX)
    {
        bIndex = 0;
    }
        
    for (i = 0; i < IR_MACRO_TBL_MAX; i++)  // IR_MACRO_TBL_MAX
    {
        ReadEEPROM(MACRO_0_ADDR + bIndex * MACRO_NVRAM_SIZE + i*8, I2CRxd, 8);
        
        sIR_RAM_TBL[i].Type     = I2CRxd[0];
        sIR_RAM_TBL[i].Mode     = I2CRxd[1];
        sIR_RAM_TBL[i].Address  = I2CRxd[2];
        sIR_RAM_TBL[i].Cmd      = I2CRxd[3];
        sIR_RAM_TBL[i].Repeat   = I2CRxd[4];
        sIR_RAM_TBL[i].Rev0     = I2CRxd[5];    /* Delay Hi */    
        sIR_RAM_TBL[i].Rev1     = I2CRxd[6];    /* Delay Lo */    
        sIR_RAM_TBL[i].Rev2     = I2CRxd[7];
        
//        ClearWatchdogTimer();
    }
}

void StartMacro(void)
{
    sIR_STATUS.bMarcoRun = 1;
}

void StopMacro(void)
{
    sIR_STATUS.bMarcoRun = 0;
}

void Macrotest(void)
{
#if 0
    Byte i,j,k;
    Byte code  aTemp[IR_MACRO_TBL_MAX][8]={//Ty,mod,Add,cmd,rep,rev0,rs2,rsv2
        ////////////////////////////////RC6 table////////////////////////////
        { 6, 0,    0, 0x54,  3,  0,  2,  0},
        { 6, 0,    0, 0x00,  1,  0,  2,  1},
        { 6, 0,    0, 0x06,  1,  0,  2,  2},
        { 6, 0,    0, 0x02,  1,  0,  2,  3},
        { 6, 0,    0, 0x05,  1,  0,  2,  4},
        { 6, 0,    0, 0x09,  1,  0,  2,  5},
        { 6, 0,    0, 0x06,  1,  0,  2,  6},
        { 0, 0,    0,   82,  1,  0,  2,  7},
        { 0, 0,    0, 0x20,  1,  0,  2,  8},
        { 0, 0,    0, 0x21,  1,  0,  2,  9},
        { 0, 0,    0, 0x20,  1,  0,  2,  10},
        { 0, 0,    0, 0x21,  1,  0,  2,  11},
        { 0, 0,    0, 0x20,  1,  0,  2,  12},
        { 0, 0,    0, 0x21,  1,  0,  2,  13},
        { 0, 0,    0, 0x20,  1,  0,  2,  14},
        { 0, 0,    0, 0x5A,  1,  0,  2,  15}
		};


        for (k=0;k<20;k++)
        {
            for (i=0; i < IR_MACRO_TBL_MAX; i++)
            {
                for(j=0; j < 8; j++)
                StoreEEPROM(MACRO_0_ADDR+k*MACRO_NVRAM_SIZE+i*8+j, aTemp[i][j]);

                ClearWatchdogTimer();

            }
       }
#endif
}

#ifndef  LITE_VERSION
void Ir_MacroProc(void)
{
    Byte    bRepeatKeyFlag;

    if(sIR_STATUS.bMarcoRun == 0)
    {
        return;
    }
    
    switch(sIR_STATUS.IrType)   /* Check IR type */
    {
        case RC5_INDEX:
        case RC6_INDEX:
        case NEC1_INDEX:
        case NEC2_INDEX:
        case SONY_INDEX:            
        case SHARP_INDEX:            
        case MAT_INDEX:            
        case PANA_INDEX:
        case RCMM_INDEX:
        case RCA_INDEX:
            break;

        default:
            StopMacro();
//            putstr("\r\n Error: Ir_MacroProc() IrType \r\n");
            return;
    }
    
    bRepeatKeyFlag  = FALSE;
    
    if(bTime_100ms_Flag)
    {
        bTime_100ms_Flag = 0;

        if(++sIR_STATUS.wScriptTime >= sIR_STATUS.wScriptStepDelay)
        {
            sIR_STATUS.wScriptTime = 0;

            if( ++sIR_STATUS.bScriptNum < IR_MACRO_TBL_MAX )   /* key step delay */
            {
//                printf("\r\n  Ir_MacroProc() sIR_STATUS.IrType = %x", sIR_STATUS.IrType);
                
                IR_TimeTBL(sIR_STATUS.bScriptNum, TRUE);      /* next script */
                bRepeatTimeFlag = 0;
                
//                printf("\r\n  Ir_MacroProc() bScriptNum = %x", sIR_STATUS.bScriptNum);
            }
            else
            {
                StopMacro();
            }
        }
    }
    
    if(sIR_STATUS.bScriptNum < IR_MACRO_TBL_MAX)
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
            printf("\r\n  Ir_MacroProc() Rpt: bScriptNum = %x", sIR_STATUS.bScriptNum);
            
            IR_TimeTBL(0, FALSE);   /* follow the previous key */
        }
    }
}
#endif  /* LITE_VERSION */                
