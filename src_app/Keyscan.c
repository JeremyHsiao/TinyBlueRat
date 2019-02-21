/*==========================================================================
|
|  Project Name : LCD TV RC controller
|
|  File Name	: KeyScan.c
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
|  This file is scan the front key value
|
|  Note
|
|  The KEIL C51 Compiler ( Version 7.07a ) is used.
|
==========================================================================*/

#include "chip.h"
#include "board.h"
#include "string.h"
#include "adc.h"
#include "Io.h"

#include "Define.h"

#define GLOBAL
#include "Keyscan.h"
#undef GLOBAL

#define GLOBAL extern
#include "Global.h"
#include "app_uart.h"
#include "Nvram_Access.h"
#include "Timer.h"
#include "Initial.h"
#include "Ir.h"
#include "IrAuto.h"
#include "IrMacro.h"
#include "Timeout.h"
#undef GLOBAL

#ifndef  LITE_VERSION
void KeyScan(void)
{
    static signed char  bPrevFntKey;
    static Byte         KeyRepeatCounter;
    static Byte         bDebounce_Cnt;
    Byte        bGetKeyFlag;
    Byte        bRepeatKeyFlag;
    signed char bFntKey;
    
    bGetKeyFlag     = FALSE;
    bRepeatKeyFlag  = FALSE;
    
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
            IR_TimeTBL(bPrevFntKey, TRUE);
            
            KeyRepeatCounter++;
            
        	printf("\r\n KeyScan() First key %d", bFntKey);

            return;
        }
        else if(++KeyRepeatCounter >= bKEY_repeat_times) /* repeat key */
        {
            KeyRepeatCounter = 1;

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
    }

    /* ready to send the real signal */
    if(bRepeatKeyFlag == TRUE)
    {
    	printf("\r\n KeyScan() bPrevFntKey = %d", bPrevFntKey&0xFF);

        IR_TimeTBL(0, FALSE);   /* follow the previous key */
/*
*/
    }
}

void PcKeyProcess(void)
{
    Byte    bRepeatKeyFlag;
    
    bRepeatKeyFlag  = FALSE;

    if(sIR_STATUS.bKeyPressed)
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
    }
    
    /* ready to send the real signal */
    if(bRepeatKeyFlag == TRUE)
    {
        IR_TimeTBL(0, FALSE);   /* follow the previous key */
    }
}

#ifdef FRC
////////////////////////////////////////////////////
//      PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0//output
//PD0   0   2   4   6   8   10  12  14
//PD2   1   3   5   7   9   11  13  15
signed char GetKey(void)
{
/*
    Byte i, j;
    Byte code aKeyData[8] =  { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7,
                               0xFB, 0xFD, 0xFE
                             };  //set one scan out pin low to check whether the scan in pin get low.
    Byte code aKeyCheck[2] = { _BIT0, _BIT2
                             };

    for(i = 0; i < 2; i++)
    {
		PortC |= 0xC0;
    	PortC &= aKeyData[i];
        
        for(j=0; j < 2; j++)
        {
            if((PortD&aKeyCheck[j]) == 0)   //low means key pressed PD0.
            {
                return (i*2 + j);
            }
        }
    }
*/
    return -1;
}
#else
////////////////////////////////////////////////////
//      PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0//output
//PD0   0   2   4   6   8   10  11  13
//PD1   1   3   5   7   9   11  12  14

//PD0   0   6   12  18  24  30  36   42
//PD2   1   7   13  19  25  31  37   43
//PD3   2   8   14  20  26  32  38   44
//PD4   3   9   15  21  27  33  39   45
//PD5   4   10  16  22  28  34  40   46
//PD6   5   11  17  23  29  35  41   47


signed char GetKey(void)
{
	uint32_t	adc_key_value;

	// To be modified
	adc_key_value = (ADCRead(ADC_CH0)>>3)&0xFF; // read key ADC -- requires minimal 2.44 us

	if(adc_key_value<0xF0)		// test -- shorted with GND
	{
		if ((adc_key_value>12)&&(adc_key_value<18))
				return 0;
		else if((adc_key_value>27)&&(adc_key_value<33))
				return 1;
		else if((adc_key_value>49)&&(adc_key_value<54))
				return 2;
		else if((adc_key_value>53)&&(adc_key_value<57))
				return 3;
		else if((adc_key_value>62)&&(adc_key_value<69))
				return 4;
		else if((adc_key_value>68)&&(adc_key_value<76))
				return 5;
		else if((adc_key_value>80)&&(adc_key_value<90))
				return 6;
		else return -1;

	}

	return -1;
}

#endif



#endif  /* LITE_VERSION */                

////////////////////////////////////////////////////////
Byte LSBToMSB(Byte Value)
{
    Byte bValue = 0;
    int i;

    for(i=0; i < 8; i++)
    {
        bValue = bValue + (((Value >> i) & 0x01) << (7-i));
    }

//    printf("\r\n Value=== %x", Value);
//    printf("\r\n bValue=== %x", bValue);

    return  bValue;

}


