/*==========================================================================
|
|  Project Name : LCD TV RC controller
|
|  File Name	: Initial.C
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
|  This file includes all initial value.
|
|  Note
|
|  The KEIL C51 Compiler ( Version 7.07a ) is used.
|
==========================================================================*/

#include "chip.h"
#include "board.h"
#include "string.h"
#include "watchdog.h"

// Common
#include "Define.h"

// Internal
#define  GLOBAL
#include "Initial.h"
#undef GLOBAL


// External
#define  GLOBAL extern
#include "Uart.h"
#include "Debug.h"
#include "Timer.h"   
#include "Global.h" 
#include "Io.h"
#include "Keyscan.h"
#include "Uart.h"
#include "Ir.h"
#include "IrAuto.h"
#include "Irmacro.h"
#include "app_timer.h"
#include "app_uart.h"
#undef GLOBAL


void CPU_IR_Encoder_Setting(void)
{
    IR_Transmit_Buffer_Init();
 //   fIR_Dec_Mode    = FALSE;
	IrRamInitial();
	bToggleflag = 0;
}

void CPU_IR_Decoder_Setting(void)
{
	bIrTimeIndexIn  = 0;
	bIrTimeIndexOut = 0;
	bIndexRC= (Byte *) sRC_TIMETABLE;
    fIR_Dec_Mode    = TRUE;

//    SystemTimer10ms = 10000 / SAM_TIME_20US;
    SystemTimer10ms = 1;	// System is 11.469 ms
    Initial_ir_dec();
}

void CPU_IR_Learning_Setting(void)
{
	bIrTimeIndexIn  = 0;
	bIrTimeIndexOut = 0;
	bIndexRC= (Byte *) sRC_TIMETABLE;
    fIR_Dec_Mode    = TRUE;

    IRRear = 0;
    IRRear_previous = 0;
    Request_RC_RawData_Index = 0;
    Current_RC_RawData_index = 0;
    Wait_for_URD_Data = FALSE;

    //    SystemTimer10ms = 10000 / SAM_TIME_20US;
    SystemTimer10ms = 1;	// System is 11.469 ms
    Initial_ir_dec();
}

void CPU_IR_Simu_Setting(void)
{
   	bTail = 0x00;
	bIrTimeIndexIn  = 0;
	bIrTimeIndexOut = 0;
	bIndexRC= (Byte *) sRC_TIMETABLE;
    fIR_Dec_Mode    = TRUE;

//    SystemTimer10ms = 10000 / SAM_TIME_20US;
    SystemTimer10ms = 1;	// System is 11.469 ms
	IR_Carrier(38000);		
//    Initial_ir_dec();
}


///////////////////////////////////////////////////////////////////////////////
// <Function>: InitVariable
//
// <Description>: Initialize global variable.
///////////////////////////////////////////////////////////////////////////////
void InitVariable(void)
{
//    Word    wTHOtemp;
    
    SystemTimer         = 0;
    SystemTimer10msCnt  = 0;
//    SystemTimer10msCnt1 = 0;
    SystemTimer100msCnt = 0;
    
//	wTHOtemp        = TH0_89US;
//	wTimer10ms      = 10000 / SAM_TIME_89US;
    SystemTimer10ms = 10000 / SAM_TIME_89US;;
  
	bIrTimeIndexIn  = 0;
	bIrTimeIndexOut = 0;
    
	RCRepeatCounter = 0;

	sIR_STATUS.IrType           = RC6_INDEX;
	sIR_STATUS.bToggle          = 0;
    sIR_STATUS.bKeyPressed      = 0;
    
	KeyScanTimer                = wTimer10ms;

	scTdiffer       = 0;
	scCarrierdif    = 0;
	wTimeBase       = 0;

    fIR_Dec_Mode    = FALSE;

    bFunctionMode   = IR_PC_SRIPT_MODE;

    Initial_Mode_LED();
    
    bResetRxdCmdFlag    = 0;
}

void Hardware_Initial(void)
{
	ClearWatchdogTimer();
}

void PowerOnInit(void)
{
	Hardware_Initial();
	InitVariable();         // Initialize global variable
}	

