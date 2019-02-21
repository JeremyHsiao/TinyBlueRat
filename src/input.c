/*
 * input.c
 *
 *  Created on: 2014/2/11
 *      Author: jeremy.hsiao
 */

#include "chip.h"
#include "board.h"
#include "string.h"
#include "input.h"

int			rel_x=0, rel_y=0;
uint32_t	abs_x=0, abs_y=0;
Bool		btn_left=0, btn_right=0;

int			rel_x_buf=0, rel_y_buf=0;
uint32_t	abs_x_buf=0, abs_y_buf=0;
Bool		btn_left_buf=0, btn_right_buf=0;
uint32_t	bit_touch=0;

void input_report(uint32_t evt, int value)
{
	switch (evt)
	{
		case REL_X:
			rel_x_buf = value;
			bit_touch |= 1<< REL_X;
			break;

		case REL_Y:
			rel_y_buf = value;
			bit_touch |= 1<< REL_Y;
			break;

		case BTN_LEFT:
			btn_left_buf = value;
			bit_touch |= 1<< BTN_LEFT;
			break;

		case BTN_RIGHT:
			btn_right_buf = value;
			bit_touch |= 1<< BTN_RIGHT;
			break;

		case ABS_X:
			abs_x_buf = value;
			bit_touch |= 1<< ABS_X;
			break;

		case ABS_Y:
			abs_y_buf = value;
			bit_touch |= 1<< ABS_Y;
			break;

		default:
			break;
	}
}

void input_sync(void)
{
	// REL has higher priority in current implementation
	if( bit_touch&(1<< REL_X) )
	{
		rel_x = rel_x_buf;
	}
	else if( bit_touch&(1<< ABS_X) )
	{
		rel_x = (int32_t)abs_x_buf - (int32_t)abs_x;
		abs_x = abs_x_buf;
	}

	// REL has higher priority in current implementation
	if( bit_touch&(1<< REL_Y) )
	{
		rel_y = rel_y_buf;
	}
	else if( bit_touch&(1<< ABS_Y) )
	{
		rel_y = (int32_t)abs_y_buf - (int32_t)abs_y;
		abs_y = abs_x_buf;
	}

	if( bit_touch&(1<< BTN_LEFT) )
	{
		btn_left = btn_left_buf;
	}
	if( bit_touch&(1<< BTN_RIGHT) )
	{
		btn_right = btn_right_buf;
	}

    bit_touch=0;
}

int My_input_event_value(uint32_t evt)
{
	int	rtn_value = 0;

	switch (evt)
	{
		case REL_X:
			rtn_value = rel_x;
			break;

		case REL_Y:
			rtn_value = rel_y;
			break;

		case BTN_LEFT:
			rtn_value = btn_left;
			break;

		case BTN_RIGHT:
			rtn_value = btn_right;
			break;

		default:
			break;
	}

	return rtn_value;
}

