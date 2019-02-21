/****************************************************************************
 *   $Id:: i2c.c 9374 2012-04-19 22:58:18Z nxp41306                         $
 *   Project: NXP LPC11xx I2C example
 *
 *   Description:
 *     This file contains I2C code example which include I2C initialization, 
 *     I2C interrupt handler, and APIs for I2C access.
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
#include "i2c.h"

#define SPEED_100KHZ         100000
#define SPEED_400KHZ         400000

static int mode_poll;   /* Poll/Interrupt mode flag */

static inline void i2c_state_handling(I2C_ID_T id)
{
	if (Chip_I2C_IsMasterActive(id)) {
		Chip_I2C_MasterStateHandler(id);
	} else {
		Chip_I2C_SlaveStateHandler(id);
	}
}

/* 
From device to device, the I2C communication protocol may vary, 
in the example below, the protocol uses repeated start to read data from or 
write to the device:
For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO 
for master write: the sequence is: STA,Addr(W),offset,RE-STA,Addr(w),data...STO
Thus, in state 8, the address is always WRITE. in state 10, the address could 
be READ or WRITE depending on the I2C command.
*/   

/*****************************************************************************
** Function name:		I2C_IRQHandler
**
** Descriptions:		I2C interrupt handler, deal with master mode only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C_IRQHandler(void) 
{
	i2c_state_handling(I2C0);
}

/* Set I2C mode to polling/interrupt */
static void i2c_set_mode(I2C_ID_T id, int polling)
{
	if(!polling) {
		mode_poll &= ~(1 << id);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
		NVIC_EnableIRQ(I2C0_IRQn);
	} else {
		mode_poll |= 1 << id;
		NVIC_DisableIRQ(I2C0_IRQn);
		Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandlerPolling);
	}
}

/*****************************************************************************
** Function name:		I2CInit
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
** 
*****************************************************************************/
uint32_t I2CInit(void)
{
	Board_I2C_Init(I2C0);

	/* Initialize I2C */
	Chip_I2C_Init(I2C0);
	Chip_I2C_SetClockRate(I2C0, SPEED_100KHZ);

	/* Set default mode to interrupt */
	i2c_set_mode(I2C0, 0);

	return( TRUE );
}
/******************************************************************************
**                            End Of File
******************************************************************************/

