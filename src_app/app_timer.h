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
#ifndef _APP_TIMER_H_
#define _APP_TIMER_H_

//
//
//

//need timer0 mode 2 8bits autoreload
#define SAM_TIME_89US   (89)                        /* unit: us */
#define SAM_TIME_44US   (44)                        /* unit: us */
#define SAM_TIME_56US   (56)                        /* unit: us */
#define SAM_TIME_20US   (20)                        /* unit: us */
#define SAM_TIME_10US   (10)                        /* unit: us */
#define SAM_TIME_60US   (60)                        /* unit: us */
#define SAM_TIME_32US   (32)                        /* unit: us */
#define SAM_TIME_64US   (64)                        /* unit: us */
#define SAM_TIME_40US   (40)                        /* unit: us */

//
//
//

/* The test is either MAT_OUT or CAP_IN. Default is MAT_OUT. */
#define TIMER_MATCH		0

#define EMC0	4
#define EMC1	6
#define EMC2	8
#define EMC3	10

#define INT_MATCH0		(1<<0)
#define INT_MATCH1		(1<<1)
#define INT_MATCH2		(1<<2)
#define INT_MATCH3		(1<<3)
#define INT_CAPTURE0	(1<<4)

//#ifndef TIME_INTERVAL
//#define TIME_INTERVAL	(SystemCoreClock /1000 - 1)
////#define TIME_INTERVAL	(SystemAHBFrequency/100 - 1)
///* depending on the SystemFrequency and SystemAHBFrequency setting,
//if SystemFrequency = 60Mhz, SystemAHBFrequency = 1/4 SystemAHBFrequency,
//10mSec = 150.000-1 counts */
//#endif
//
//#define CONST_1MHz						(1000000)
//#define CONST_1KHz						(1000)
//#define CONST_1Hz						(1)
//
//#define PCLK_FREQUENCY					(48*CONST_1MHz)										// 48MHz
//#define PRESCALER_Hz_CNT(Hz_value)		((PCLK_FREQUENCY)/(Hz_value))
//#define PRESCALER_1US_CNT				(PRESCALER_Hz_CNT(CONST_1MHz))
//#define PRESCALER_10US_CNT				(PRESCALER_Hz_CNT((CONST_1KHz*100)))
//#define PRESCALER_1MS_CNT				(PRESCALER_Hz_CNT(CONST_1KHz))
//
//// Timer 0 is 1us-based
//#define TIMER0_PRESCALER				(PRESCALER_1US_CNT)
//#define TIMER0_1uS_CNT					((PCLK_FREQUENCY/TIMER0_PRESCALER)/CONST_1MHz)		// Valid only when (PCLK_FREQUENCY/TIMER0_PRESCALER) >= CONST_1MHz
//#define TIMER0_1mS_CNT					((PCLK_FREQUENCY/TIMER0_PRESCALER)/CONST_1KHz)		// Valid only when (PCLK_FREQUENCY/TIMER0_PRESCALER) >= CONST_1KHz
//#define TIMER0_1S_CNT					((PCLK_FREQUENCY/TIMER0_PRESCALER)/CONST_1Hz)		// Valid only when (PCLK_FREQUENCY/TIMER0_PRESCALER) >= CONST_1Hz
//
//// Timer 1 is PCLK-based
//#define TIMER1_PRESCALER				(1)
//#define TIMER1_1uS_CNT					((PCLK_FREQUENCY/TIMER1_PRESCALER)/CONST_1MHz)		// Valid only when (PCLK_FREQUENCY/TIMER1_PRESCALER) >= CONST_1MHz
//#define TIMER1_1mS_CNT					((PCLK_FREQUENCY/TIMER1_PRESCALER)/CONST_1KHz)		// Valid only when (PCLK_FREQUENCY/TIMER1_PRESCALER) >= CONST_1KHz
//#define TIMER1_1S_CNT					((PCLK_FREQUENCY/TIMER1_PRESCALER)/CONST_1Hz)		// Valid only when (PCLK_FREQUENCY/TIMER1_PRESCALER) >= CONST_1Hz
//
//// Timer16B0 CAP0 is 10us-based
//#define TIMER16B0_PRESCALER				(PRESCALER_10US_CNT)
//#define TIMER16B0_10uS_CNT				((PCLK_FREQUENCY/TIMER16B0_PRESCALER)/(CONST_1KHz*100))		// Valid only when (PCLK_FREQUENCY/TIMER16B0_PRESCALER) >= CONST_1MHz
//#define TIMER16B0_1mS_CNT				((PCLK_FREQUENCY/TIMER16B0_PRESCALER)/CONST_1KHz)			// Valid only when (PCLK_FREQUENCY/TIMER16B0_PRESCALER) >= CONST_1KHz
//
//// Timer16B1 CAP0 is PCLK-based
//#define TIMER16B1_PRESCALER				(1)
//#define TIMER16B1_1uS_CNT				((PCLK_FREQUENCY/TIMER16B1_PRESCALER)/(CONST_1MHz))			// Valid only when (PCLK_FREQUENCY/TIMER16B1_PRESCALER) >= CONST_1MHz
//#define TIMER16B1_1mS_CNT				((PCLK_FREQUENCY/TIMER16B1_PRESCALER)/(CONST_1KHz))			// Valid only when (PCLK_FREQUENCY/TIMER16B1_PRESCALER) >= CONST_1KHz

//void TIMER32_0_IRQHandler(void);
//void TIMER32_1_IRQHandler(void);
//void enable_timer32(uint8_t timer_num);
//void disable_timer32(uint8_t timer_num);
//void reset_timer32(uint8_t timer_num);
//void init_timer32(uint8_t timer_num, uint32_t timerInterval);
//void init_timer32PWM(uint8_t timer_num, uint32_t period, uint8_t match_enable);
//void setMatch_timer32PWM (uint8_t timer_num, uint8_t match_nr, uint32_t value);
//void enable_timer16(uint8_t timer_num);
//void init_timer16(uint8_t timer_num);
void Setup_IR_PWM_Pulse_by_IR_Level(uint32_t bLevel);
uint32_t Check_PWM_Pulse_Width(uint32_t temp_width);
void Setup_IR_PWM_duty_cycle(uint32_t duty_cycle);

extern void IR_Transmit_Buffer_Init(void);
extern void IR_Transmit_Buffer_StartSend(void);
extern uint8_t IR_Transmit_Buffer_Push(uint32_t temp_level, uint32_t temp_width );
extern uint8_t IR_Transmit_Buffer_Pop(uint32_t *temp_level, uint32_t *temp_width );

extern volatile uint32_t		PWM_period;
extern volatile uint32_t 		IR_Transmitter_Running;
extern volatile uint32_t		PWM_duty_cycle;

///
///
///

//extern Word wTimer10ms;
//extern Word data  wTL;
//extern Word data  wTH;
//

extern Word data wSystemClock;
extern Byte data bTS;

extern void Delayus(uint32_t uint32_Value);
#define 	Delay1ms(wValue)		Delayus((((uint32_t)(wValue))*1000))

///
///
///

#endif /* end _APP_TIMER_H_ */
/*****************************************************************************
**                            End Of File
******************************************************************************/
