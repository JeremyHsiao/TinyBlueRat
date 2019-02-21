/*
 * input.h
 *
 *  Created on: 2014/2/11
 *      Author: jeremy.hsiao
 */

#ifndef INPUT_H_
#define INPUT_H_

// Simulation of input_report
#define REL_X		0
#define REL_Y		1
#define BTN_LEFT	2
#define BTN_RIGHT	3
#define ABS_X		5
#define ABS_Y		6

extern void input_report(uint32_t evt, int value);
#define input_report_rel(x, y)	input_report(x, y)
#define input_report_abs(x, y)	input_report(x, y)
extern void input_sync(void);
extern int My_input_event_value(uint32_t evt);

#endif /* INPUT_H_ */
