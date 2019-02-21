/****************************************************************************
 *   $Id:: i2c.h 3662 2010-06-03 19:47:02Z usb00423                         $
 *   Project: NXP LPC11xx I2C example
 *
 *   Description:
 *     This file contains I2C code header definition.
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
#ifndef __I2C_H 
#define __I2C_H

extern void I2C_IRQHandler( void );
extern uint32_t I2CInit(void);

#endif /* end __I2C_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
