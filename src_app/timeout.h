#ifndef _TIMEOUT_H
#define _TIMEOUT_H

GLOBAL void RCKeyChannelNumberTimeOut(void);
GLOBAL void StoreLifeTimerEvery30Min(void);
GLOBAL void OSDTimeOutDefalutSetting(void);
GLOBAL void OSDTimeOutDefalutSettingHDTV(void);
GLOBAL void TurboRotaryTimeOut(void);
GLOBAL void CheckTunerOSDFrameLock(void);
GLOBAL void GetVchipRating(void);
typedef struct
{
	int X;
	int Y;
}IM_POSITION_Type;

/* variables */
GLOBAL Byte bRepeatTimeFlag;
GLOBAL Byte bTime_100ms_Flag;

/* functions */
GLOBAL void TimeOutCtrl(void);

GLOBAL void RestartClearIrDecDataTimer(void);
    
GLOBAL void StartRepeatKeyTimer(void);
GLOBAL void StopRepeatKeyTimer(void);

GLOBAL void RestartAutoScriptTimer(void);
GLOBAL void StartMacroKeyTimer(void);

#endif
