/*==========================================================================
|
|  Project Name : LCD TV RC controller
|
|  File Name	: TimeOut.C
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
|  This file is timeout service.
|
|  Note
|
|  The KEIL C51 Compiler ( Version 7.07a ) is used.
|
==========================================================================*/

#include "board.h"

// System


// Common



// Internal
#include "Define.h"
#define GLOBAL
#include "Timeout.h"
#undef GLOBAL

// External
#define GLOBAL extern
#include "Global.h"
#include "app_uart.h"
#include "Io.h"
#include "Timer.h"
#include "Initial.h"
#include "Ir.h"
#include "Keyscan.h"
#include "IrAuto.h"
#include "IrMacro.h" 
#undef GLOBAL


void TimeOutCtrl(void)
{
    if(SystemTimer >= 10)   /* 10 ms */
    {
        SystemTimer = 0;
        SystemTimer10msCnt++;
        KeyScanTimer = 0;
        IRResetTimerCnt++;
        IRDataResetTimerCnt++;
//        SystemTimer10msCnt1++;
        /* set flag */
//        IR_LED_OUT_TOGGLE;
    }
    
//    if(SystemTimer10msCnt1 >= 8)         /* */ /*sharp with 40ms gap*/
//    {
//        SystemTimer10msCnt1 = 0;
//
//        /* set flag */
//        ClearIrDecDataFlag = 1;
//    }
    
    if(SystemTimer10msCnt >= 10)         /* 100 ms */
    {
        SystemTimer10msCnt = 0;
        SystemTimer10msCnt1 = 0;

        SystemTimer100msCnt++;
        
        /* set flag */
        bResetRxdCmdFlag    = 1;
        bRepeatTimeFlag     = 1;
        bTime_100ms_Flag    = 1;
//        ClearIrDAFlag = 1;
    }
    
    if(SystemTimer100msCnt > 4)        /* 500 ms */
    {
        SystemTimer100msCnt = 0;
        ClearIrDecDataFlag = 1;
        /* set flag */
//        bModeLEDflag = 1;
//		bToggleflag++;
    }
}

#ifndef LITE_VERSION
void RestartClearIrDecDataTimer(void)
{
    SystemTimer10msCnt1 = 0;     /* reset timer */

    ClearIrDecDataFlag  = 0;
//    ClearIrDAFlag = 0;		// For IrDA
    IRDataResetTimerCnt = 0x00;
}
#endif  /* LITE_VERSION */                

void StartRepeatKeyTimer(void)
{
    sIR_STATUS.bKeyPressed = 1;
    
    SystemTimer10msCnt = 0;     /* reset timer */
    
    bRepeatTimeFlag = 0;
}

void StopRepeatKeyTimer(void)
{
    sIR_STATUS.bKeyPressed = 0;
}

void RestartAutoScriptTimer(void)
{
    sIR_STATUS.bScriptNum   = 0;
    sIR_STATUS.wScriptTime  = 0;
    
    IR_TimeTBL(0, TRUE);        /* first script */
    
    SystemTimer10msCnt  = 0;    /* reset timer */
    
    bRepeatTimeFlag     = 0;
    bTime_100ms_Flag    = 0;
}

void StartMacroKeyTimer(void)
{
    sIR_STATUS.bScriptNum   = 0;
    sIR_STATUS.wScriptTime  = 0;
    
    IR_TimeTBL(0, TRUE);        /* first script */
    
    SystemTimer10msCnt  = 0;    /* reset timer */
    
    bRepeatTimeFlag     = 0;
    bTime_100ms_Flag    = 0;
}

