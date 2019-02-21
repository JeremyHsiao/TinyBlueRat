/****************************************************************************
 *   $Id:: adc.h 9359 2012-04-19 22:30:24Z nxp41306                         $
 *   Project: NXP LPC11xx ADC example
 *
 *   Description:
 *     This file contains ADC code header definition.
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

#ifndef __ADC_H 
#define __ADC_H

extern void ADCInit(void);
extern uint32_t ADCRead(CHIP_ADC_CHANNEL_T Ch_No);
#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
