/*
 * @brief Blinky example using timers and sysTick
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include <stdio.h>
#include "LPC11U37_401.h"
#include "Timer.h"

/** @defgroup EXAMPLES_PERIPH_11XX_BLINKY LPC11xx Simple blinky example
 * @ingroup EXAMPLES_PERIPH_11XX
 * <b>Example description</b><br>
 * The Blinky example is known as the "Hello, World!" of embedded programming. This
 * example is a slight variation of the blinky theme in that the sysTick
 * interrupt turns on the LED at 10Hz, while timer 1 turns off the LED at 11Hz.
 * The LED on/off state should be slightly out of sync.<br>
 *
 * <b>Special connection requirements</b><br>
 * There are no special connection requirements for this example.<br>
 *
 * <b>Build procedures:</b><br>
 * @ref LPCOPEN_11XX_BUILDPROCS_XPRESSO<br>
 * @ref LPCOPEN_11XX_BUILDPROCS_KEIL<br>
 * @ref LPCOPEN_11XX_BUILDPROCS_IAR<br>
 *
 * <b>Supported boards and board setup:</b><br>
 * @ref LPCOPEN_11XX_BOARD_XPRESSO_11U14<br>
 * @ref LPCOPEN_11XX_BOARD_XPRESSO_11C24<br>
 *
 * <b>Submitting LPCOpen issues:</b><br>
 * @ref LPCOPEN_COMMUNITY
 * @{
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

//#define TICKRATE_HZ1 (10) /* 10 ticks per second */
//#define TICKRATE_HZ2 (11) /* 11 ticks per second */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

uint8_t toggle_bit = 0;
uint8_t show_message_off, show_message_on;


/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
//void SysTick_Handler(void)
//{
//	Board_LED_Set(0, false);
//	show_message_off=1;
//}

/**
 * @brief	Handle interrupt from 32-bit timer
 * @return	Nothing
 */
//inline void TEST_TIMER_Match_IRQHandler(void)
//{
//	const char *Str_Off = "123\r\n";
//	const char *Str_On  = "987\r\n";
//
//	if(toggle_bit!=0)
//	{
//		toggle_bit=0;
//		Board_LED_Set(0, false);
//		//show_message_off=1;
//		//UARTputstr("123\r\n");
//		VirtualSerial_MultiByteToHost(Str_Off,strlen(Str_Off));
//	}
//	else
//	{
//		toggle_bit=1;
//		Board_LED_Set(0, true);
//		//show_message_on=1;
//		//UARTputstr("987\r\n");
//		VirtualSerial_MultiByteToHost(Str_On,strlen(Str_On));
//	}
//}

/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main_blinky(void)
{
	uint32_t	temp_tc, temp_mr3;

	/* Enable timer 1 clock */
	Chip_TIMER_Init(LPC_TIMER32_0);
	Chip_TIMER_PrescaleSet(LPC_TIMER32_0, TIMER0_PRESCALER); // Prescaler set to 0

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER32_0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, MATCH_3);
	// Load Timer0_MATCH first match value (based on TC) and enable interrupt here
	temp_tc = Chip_TIMER_ReadCount(LPC_TIMER32_0);
	temp_mr3 = (500 * TIMER0_1mS_CNT) + temp_tc;
	Chip_TIMER_SetMatch(LPC_TIMER32_0, MATCH_3, temp_mr3);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER32_0, MATCH_3);
	Chip_TIMER_Enable(LPC_TIMER32_0);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
	NVIC_EnableIRQ(TIMER_32_0_IRQn);

	/* LEDs toggle in interrupt handlers */
//	while (1) {
//		__WFI();
//	}

	return 1;
}

/**
 * @}
 */
