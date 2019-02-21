
#include "Define.h"

#ifndef _IRAUTO_H_
#define _IRAUTO_H_

/* constats & definition */
typedef struct _IR_SRCIPT
{
	Byte   bType;
  	Word   wGrpDelayTime;
  	Byte   bNums;		    
}sIR_SCRIPT_HEADER;

/* variables */
GLOBAL sIR_SCRIPT_HEADER sSCIPT_HEADER; 
GLOBAL Byte bModeLEDflag;
GLOBAL Byte bToggleflag;
GLOBAL Byte	bFunctionMode;

/* functions */
GLOBAL void CheckMode (void);
GLOBAL void ScriptVarIni(void);
GLOBAL void Scripttest(void);
GLOBAL void ScriptRamIni(void);
GLOBAL void IR_AutoScripProc(void);

GLOBAL void Ir_Mode_Ini(Byte bFunMode);
GLOBAL void Initial_Mode_LED(void);
GLOBAL void Set_Mode_LED(void);
GLOBAL void Mode_LED_Proc(void);

#endif
