/****************************************************************************
 *   $Id:: timer32.h 3635 2010-06-02 00:31:46Z usb00423                     $
 *   Project: NXP LPC11xx software example
 *
 *   Description:
 *     This file contains definition and prototype for 32-bit timer 
 *     configuration.
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
#ifndef __TIMER_H
#define __TIMER_H

#define CONST_1MHz						(1000000)
#define CONST_1KHz						(1000)
#define CONST_1Hz						(1)

#define PCLK_FREQUENCY					(48*CONST_1MHz)										// 48MHz
#define PRESCALER_Hz_CNT(Hz_value)		((PCLK_FREQUENCY)/(Hz_value))
#define PRESCALER_1US_CNT				(PRESCALER_Hz_CNT(CONST_1MHz))
#define PRESCALER_10US_CNT				(PRESCALER_Hz_CNT((CONST_1KHz*100)))
#define PRESCALER_1MS_CNT				(PRESCALER_Hz_CNT(CONST_1KHz))

// Timer 0 is 1us-based
#define TIMER0_PRESCALER				(PRESCALER_1US_CNT)
#define TIMER0_1uS_CNT					((PCLK_FREQUENCY/TIMER0_PRESCALER)/CONST_1MHz)		// Valid only when (PCLK_FREQUENCY/TIMER0_PRESCALER) >= CONST_1MHz
#define TIMER0_1mS_CNT					((PCLK_FREQUENCY/TIMER0_PRESCALER)/CONST_1KHz)		// Valid only when (PCLK_FREQUENCY/TIMER0_PRESCALER) >= CONST_1KHz
#define TIMER0_1S_CNT					((PCLK_FREQUENCY/TIMER0_PRESCALER)/CONST_1Hz)		// Valid only when (PCLK_FREQUENCY/TIMER0_PRESCALER) >= CONST_1Hz

// Timer 1 is PCLK-based
#define TIMER1_PRESCALER				(1)
#define TIMER1_1uS_CNT					((PCLK_FREQUENCY/TIMER1_PRESCALER)/CONST_1MHz)		// Valid only when (PCLK_FREQUENCY/TIMER1_PRESCALER) >= CONST_1MHz
#define TIMER1_1mS_CNT					((PCLK_FREQUENCY/TIMER1_PRESCALER)/CONST_1KHz)		// Valid only when (PCLK_FREQUENCY/TIMER1_PRESCALER) >= CONST_1KHz
#define TIMER1_1S_CNT					((PCLK_FREQUENCY/TIMER1_PRESCALER)/CONST_1Hz)		// Valid only when (PCLK_FREQUENCY/TIMER1_PRESCALER) >= CONST_1Hz

// Timer16B0 CAP0 is 10us-based
#define TIMER16B0_PRESCALER				(PRESCALER_10US_CNT)
#define TIMER16B0_10uS_CNT				((PCLK_FREQUENCY/TIMER16B0_PRESCALER)/(CONST_1KHz*100))		// Valid only when (PCLK_FREQUENCY/TIMER16B0_PRESCALER) >= CONST_1MHz
#define TIMER16B0_1mS_CNT				((PCLK_FREQUENCY/TIMER16B0_PRESCALER)/CONST_1KHz)			// Valid only when (PCLK_FREQUENCY/TIMER16B0_PRESCALER) >= CONST_1KHz

// Timer16B1 CAP0 is PCLK-based
#define TIMER16B1_PRESCALER				(1)
#define TIMER16B1_1uS_CNT				((PCLK_FREQUENCY/TIMER16B1_PRESCALER)/(CONST_1MHz))			// Valid only when (PCLK_FREQUENCY/TIMER16B1_PRESCALER) >= CONST_1MHz
#define TIMER16B1_1mS_CNT				((PCLK_FREQUENCY/TIMER16B1_PRESCALER)/(CONST_1KHz))			// Valid only when (PCLK_FREQUENCY/TIMER16B1_PRESCALER) >= CONST_1KHz

extern volatile uint32_t		LastCaptureTime_IR;
extern volatile uint32_t		LastCaptureTime_CEC;
extern volatile uint32_t		LastCaptureTime_HSync;
extern volatile	uint32_t		PWM_period;
extern volatile	uint32_t		PWM_duty_cycle;
extern          Bool 			SW_Timer_Timeout;
extern volatile uint32_t 		SystemTimer;
extern void Timer_Init(void);
extern void Delayus(uint32_t delay_us_value);
extern void SetMyTimer_us(uint32_t delay_us_value);
extern Bool CheckMyTimer_us_TimeOut(void);
extern void ClearMyTimer_us();

//
//
//

//
//extern Word wTimer10ms;
//extern Word data  wTL;
//extern Word data  wTH;
//
//extern void ClearWatchdogTimer(void);
//
//extern Word data IrCodeCounter;
//extern Word data wSystemClock;
//extern Byte data bTS;
//
//extern void Delayus(uint32_t uint32_Value);
//#define 	Delay1ms(wValue)		Delayus((((uint32_t)(wValue))*1000))

///
///
///

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
