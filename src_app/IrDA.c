/*
 * IrDA.c
 *
 *  Created on: 2013/1/7
 *      Author: Jeremy.Hsiao
 */

#include "chip.h"
#include "board.h"
#include "string.h"
#include "Timer.h"
#include "gpio.h"

// Common
#include "Define.h"
#include "Common.h"
//#include  "string.h"
// Internal
#define  GLOBAL
#include "IrDA.h"
#undef GLOBAL

// External
#define  GLOBAL extern
#include "global.h"
#include "Ir.h"
#include "app_uart.h"
#include "version.h"
#include "Io.h"
#include "Nvram_Access.h"
#include "Keyscan.h"
#include "IrAuto.h"
#include "IrMacro.h"
#undef GLOBAL

// Global variable here
uint16_t bIrDATimeIndexIn;
uint16_t bIrDATimeIndexOut;
uint32_t sIrDA_TIMETABLE[IRDA_TBL_MAX];
uint32_t IrDA_LastCapture;
// end

#ifdef _MY_UNIT_TEST_
#else
/*****************************************************************************
** Function name:		FLEX_INT0_IRQHandler
**
** Descriptions:		Use one GPIO pin as interrupt source
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void IrDA_Int_Handler(void)
{
	uint32_t 		current_tc, temp_width;

	current_tc = Chip_TIMER_ReadCount(LPC_TIMER16_1);
	temp_width = TIMER_DIFF16(current_tc,IrDA_LastCapture);
	IRDA_Pulse_Push(temp_width);
	IrDA_LastCapture = current_tc;
	return;
}
#endif // #ifdef _MY_UNIT_TEST_

uint8_t IRDA_Pulse_Push(uint32_t temp_width )
{
	uint32_t	temp;

	// Check if buffer is overflow after push
	temp = INC_INDEX16(bIrDATimeIndexIn,IRDA_TBL_MAX); 		// simulate push for buffer input index
	if ( temp == bIrDATimeIndexOut )						// detect buffer overflow after push (simulation)
	{
		return FALSE;
	}
	// end of check

	sIrDA_TIMETABLE[bIrDATimeIndexIn] = temp_width;
	bIrDATimeIndexIn = temp;
	return TRUE;
}

uint8_t IRDA_Pulse_Pop(uint32_t *temp_width )
{
	if(bIrDATimeIndexOut != bIrDATimeIndexIn)
	{
		*temp_width = sIrDA_TIMETABLE[bIrDATimeIndexOut] ;
		bIrDATimeIndexOut = INC_INDEX16(bIrDATimeIndexOut,IRDA_TBL_MAX);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void IrDA_init(void)
{
	const uint8_t	INT_NO = 0;
	LPC_GPIO_T		*GPIO_NA=LPC_GPIO_PORT;		// Don't care variable here
	const uint8_t	Bit_NA=0;					// Don't care variable here

	// Debouncing
	// Chip_IOCON_PinMuxSet(LPC_IOCON, IRDA_RX_GPIO_PORT_NUM, IRDA_RX_GPIO_BIT_NUM, (PIO1_15_FUNC_PIO1_15 | PIO1_15_DEFAULT | PIO1_15_HYSTERESIS_ENABLE));
	Chip_IOCON_PinMuxSet(LPC_IOCON, IRDA_RX_GPIO_PORT_NUM, IRDA_RX_GPIO_BIT_NUM, (PIO1_15_FUNC_PIO1_15 | PIO1_15_DEFAULT));
	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, IRDA_RX_GPIO_PORT_NUM, IRDA_RX_GPIO_BIT_NUM, false);		// set false as input pin
	IrDA_LastCapture = Chip_TIMER_ReadCount(LPC_TIMER16_1);
	SetGPIOPinInterrupt( INT_NO, IRDA_RX_GPIO_PORT_NUM, IRDA_RX_GPIO_BIT_NUM, 1, 0x02 );			// Use Pin interrupt #0, edge triggered on falling edges; also enabling interrupt
	Chip_GPIO_IntClear(GPIO_NA, INT_NO, Bit_NA);
	/* Enable the GPIO Pin Interrupt #0 */
	NVIC_EnableIRQ(PIN_INT0_IRQn);
}

void Find_Carry_Frequency(uint32_t temp_width)
{
	Byte i;
	uint32_t fAverage,w32Temp,wChecksum=0;

	for(i=0;i<IRDA_TBL_MAX;i++)
	{
		if (sIrDA_Accumlate[i][1])
			fAverage = sIrDA_Accumlate[i][0]/(sIrDA_Accumlate[i][1]);

//		if (abs(temp_width - sIrDA_Accumlate[i][0])<200)
		if (abs(temp_width - fAverage)<200)
		{
			sIrDA_Accumlate[i][1]++;
			sIrDA_Accumlate[i][0]+=temp_width;
			if((sIrDA_Accumlate[i][1]==40) || (sIrDA_Accumlate[i][1]==100))
			{
				FreqAverage = fAverage;
//				printf("IR carry frq is %d Hz\r\n", temp_width);

				if (bFunctionMode == IR_LEARNING_MODE)
				{
	                PushIRData(0x02,fAverage);
//	    			IR_LED_OUT_TOGGLE;			//debug
				}
				else
				{
//					printf("IR carry frq is %d Hz\r\n", fAverage);
//					ShowIRFrq(fAverage);
				}
//				for(i=0;i<IRDA_TBL_MAX;i++)
//					sIrDA_Accumlate[i][0]=0;
			}
			break;
		}
		else
		{
			if (sIrDA_Accumlate[i][0]==0x00)
			{
				sIrDA_Accumlate[i][0] = temp_width;
				sIrDA_Accumlate[i][1] = 1;
				break;
			}
		}
	}
//	ClearIrDAFlag = 0;
	IRDataResetTimerCnt = 0x00;
}
