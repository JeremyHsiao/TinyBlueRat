/****************************************************************************
 *   $Id:: eeprom.h 6193 2011-01-17 21:18:11Z nxp28548                      $
 *   Project: NXP LPC11xx EEPROM example
 *
 *   Description:
 *     This file contains EEPROM code header definition.
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
#ifndef __INTERNALEEPROMAPI_H
#define __INTERNALEEPROMAPI_H

extern void writeInternalEEPROM( uint32_t eeAddress, uint8_t* buffAddress, uint32_t byteCount );
extern void readInternalEEPROM( uint32_t eeAddress, uint8_t* buffAddress, uint32_t byteCount );
extern void StoreIntEEPROM(uint32_t wAddr, uint8_t bData);

#endif /* end __INTERNALEEPROMAPI_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
