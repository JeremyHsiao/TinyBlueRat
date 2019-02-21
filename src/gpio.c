/****************************************************************************
 *   $Id:: gpio.c 4068 2010-07-30 02:25:13Z usb00423                        $
 *   Project: NXP LPC11xx GPIO example
 *
 *   Description:
 *     This file contains GPIO code example which include GPIO
 *     initialization, GPIO interrupt handler, and related APIs for
 *     GPIO access.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "chip.h"
#include "board.h"
#include "string.h"
#include "gpio.h"
#include "Io.h"
#include "LPC11U37_401.h"

#ifdef _MY_UNIT_TEST_
extern void IrDA_Int_Handler(void) {}
#else
extern void IrDA_Int_Handler(void);
#endif // _MY_UNIT_TEST_

uint32_t	interrput_rising_edge_cnt = 0, interrput_falling_edge_cnt = 0, interrput_level_cnt = 0;

void FLEX_INT0_IRQHandler(void)
{
	LPC_GPIO_T			*GPIO_PT = LPC_GPIO_PORT;		// Don't care variable here
	const uint8_t		Bit_NO = 0, Mode_NO = 0;		// Don't care variable here
	const uint8_t		PORT_NO = 0;

	if ( Chip_GPIO_IntGetStatus(GPIO_PT, PORT_NO, Bit_NO, Mode_NO) )
	{
		if (IP_GPIOPININT_CheckRisingEdgeIntEnable(LPC_GPIO_PIN_INT, PORT_NO) && IP_GPIOPININT_GetRisingEdgeDetect(LPC_GPIO_PIN_INT,PORT_NO) )
		{
			// Rising edge interrupt
			interrput_rising_edge_cnt++;
			Chip_GPIO_IntClear(GPIO_PT, PORT_NO, Bit_NO);
		}
		else if (IP_GPIOPININT_CheckFallingEdgeIntEnable(LPC_GPIO_PIN_INT, PORT_NO) && IP_GPIOPININT_GetFallingEdgeDetect(LPC_GPIO_PIN_INT,PORT_NO) )
		{
			// Falling edge interrupt
			IrDA_Int_Handler();
			IP_GPIOPININT_ClearFallingEdgeDetect(LPC_GPIO_PIN_INT, PORT_NO);
			interrput_falling_edge_cnt++;
			Chip_GPIO_IntClear(GPIO_PT, PORT_NO, Bit_NO);
		}
		else
		{
			// Level-triggered interrupt enters here.
			// No need to Chip_GPIO_IntClear()
			interrput_level_cnt++;
		}
	}

	return;
}

/*****************************************************************************
** Function name:		GPIOInit
**
** Descriptions:		Initialize GPIO, install the
**						GPIO interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the VIC table
**						is full and GPIO interrupt handler can be
**						installed.
**
*****************************************************************************/
void GPIOInit( void )
{
	/* Initialize GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	// Enable GPIO Pin interrupt clock domain
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PINT);

	// Enable GPIO Group interrupt #0 clock domain
	//Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GROUP0INT);
	// Enable GPIO Group interrupt #1 clock domain
	//Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GROUP1INT);
}

void SetGPIOPinInterrupt( uint32_t PinIntNo, uint32_t portNum, uint32_t bitPosi, uint32_t edge_trigger, uint32_t event )
{
	LPC_GPIO_T			*GPIO_PT=LPC_GPIO_PORT;		// Don't care variable here
	const uint8_t		Bit_NO=0;					// Don't care variable here

	Chip_SYSCTL_SetPinInterrupt(PinIntNo, portNum, bitPosi);
	if ( edge_trigger != 0 )
	{
		// This bit is rising edge
		if ( event & 0x01 )
		{
			Chip_GPIO_IntCmd(GPIO_PT, PinIntNo, Bit_NO, GPIOPININT_RISING_EDGE);
		}
		// This bit is falling edge
		if ( event & 0x02 )
		{
			Chip_GPIO_IntCmd(GPIO_PT, PinIntNo, Bit_NO, GPIOPININT_FALLING_EDGE);
		}
	}
	else	// level trigger
	{
		// Level Trigger
		if ( event != 0 )
		{
			// Level, High
			Chip_GPIO_IntCmd(GPIO_PT, PinIntNo, Bit_NO, GPIOPININT_ACTIVE_HIGH_LEVEL);
		}
		else
		{
			// Level, Low
			Chip_GPIO_IntCmd(GPIO_PT, PinIntNo, Bit_NO, GPIOPININT_ACTIVE_LOW_LEVEL);
		}
	}
}

inline void Set_LED(uint32_t LED_On_Off)
{
	if(LED_On_Off==0)
	{
		Board_LED_Set(0, false);
	}
	else
	{
		Board_LED_Set(0, true);
	}
}


inline void Toggle_LED(void)
{
	if(Board_LED_Test(0)!=false)
	{
		Board_LED_Set(0, false);
	}
	else
	{
		Board_LED_Set(0, true);
	}
}


/******************************************************************************
**                            End Of File
******************************************************************************/
