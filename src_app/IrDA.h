/*
 * IrDA.h
 *
 *  Created on: 2013/1/22
 *      Author: Jeremy.Hsiao
 */

#ifndef IRDA_H_
#define IRDA_H_

#define IRDA_TBL_MAX	(32)
uint32_t sIrDA_Accumlate[IRDA_TBL_MAX][2];

extern uint8_t 	IRDA_Pulse_Push(uint32_t temp_width );
extern uint8_t 	IRDA_Pulse_Pop(uint32_t *temp_width );
extern uint32_t IrDA_LastCapture;
extern void IrDA_init(void);

GLOBAL void Find_Carry_Frequency(uint32_t temp_width);

#endif /* IRDA_H_ */
