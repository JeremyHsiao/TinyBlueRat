/****************************************************************************
 *   $Id:: adc.c 9359 2012-04-19 22:30:24Z nxp41306                         $
 *   Project: NXP LPC11Axx ADC example
 *
 *   Description:
 *     This file contains ADC code example which include ADC 
 *     initialization, ADC interrupt handler, and APIs for ADC
 *     reading.
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
 * Permission to use, copy, modify, and distribute this software and its 
 * documentation is hereby granted, under NXP Semiconductors' 
 * relevant copyright in the software, without fee, provided that it 
 * is used in conjunction with NXP Semiconductors microcontrollers.  This 
 * copyright, permission, and disclaimer notice must appear in all copies of 
 * this code.
****************************************************************************/
#include "chip.h"
#include "board.h"
#include "string.h"
#include "adc.h"
#include "Io.h"

#define _LPC_ADC_ID LPC_ADC
static ADC_Clock_Setup_T ADCSetup;
/*****************************************************************************
** Function name:		ADCInit
**
** Descriptions:		initialize ADC channel
**
** parameters:			ADC clock rate
** Returned value:		None
** 
*****************************************************************************/
void ADCInit(void)
{
	Board_ADC_Init();

	/*ADC Init */
	Chip_ADC_Init(_LPC_ADC_ID, &ADCSetup);
	Chip_IOCON_PinMuxSet(LPC_IOCON, ADC_KEY0_GPIO_PORT_NUM, ADC_KEY0_GPIO_BIT_NUM, PIO0_11_AD0_FILT);
	Chip_ADC_Channel_Enable_Cmd(_LPC_ADC_ID, ADC_CH0, ENABLE);
}

/*****************************************************************************
** Function name:		ADCRead
**
** Descriptions:		Read ADC channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCRead(CHIP_ADC_CHANNEL_T Ch_No)
{
	uint16_t dataADC;

	/* Start A/D conversion */
	Chip_ADC_Set_StartMode(_LPC_ADC_ID, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

	/* Waiting for A/D conversion complete */
	while (Chip_ADC_Read_Status(_LPC_ADC_ID, Ch_No, ADC_DR_DONE_STAT) != SET) {}

	/* Read ADC value */
	Chip_ADC_Read_Value(_LPC_ADC_ID, Ch_No, &dataADC);

	return dataADC;

}
/*********************************************************************************
**                            End Of File
*********************************************************************************/
