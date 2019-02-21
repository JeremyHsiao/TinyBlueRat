#ifndef	_NVRAM_ACCESS_H
#define	_NVRAM_ACCESS_H

#define	PAGE_A0_ADDRESS					(EEPROM_PAGE_SIZE *	0x00)
#define	PAGE_A2_ADDRESS					(EEPROM_PAGE_SIZE *	0x01)
#define	PAGE_A4_ADDRESS					(EEPROM_PAGE_SIZE *	0x02)
#define	PAGE_A6_ADDRESS					(EEPROM_PAGE_SIZE *	0x03)
#define	PAGE_A8_ADDRESS					(EEPROM_PAGE_SIZE *	0x04)
#define	PAGE_AA_ADDRESS					(EEPROM_PAGE_SIZE *	0x05)
#define	PAGE_AC_ADDRESS					(EEPROM_PAGE_SIZE *	0x06)
#define	PAGE_AE_ADDRESS					(EEPROM_PAGE_SIZE *	0x07)
#define	PAGE_B0_ADDRESS					(EEPROM_PAGE_SIZE *	0x08)
#define	PAGE_B2_ADDRESS					(EEPROM_PAGE_SIZE *	0x09)
#define	PAGE_B4_ADDRESS					(EEPROM_PAGE_SIZE *	0x0A)
#define	PAGE_B6_ADDRESS					(EEPROM_PAGE_SIZE *	0x0B)
#define	PAGE_B8_ADDRESS					(EEPROM_PAGE_SIZE *	0x0C)
#define	PAGE_BA_ADDRESS					(EEPROM_PAGE_SIZE *	0x0D)
#define	PAGE_BC_ADDRESS					(EEPROM_PAGE_SIZE *	0x0E)


#define	KEY_RAMSTAR_ADDR                0x0000
#define	DEVIATION_ADDR					0x01F0
#define	CARRIER_ADDR					0x01F1
#define	MODE_ADDR               		0x0FFE
#define	KEY_RAM_MARK_ADDR               0x0FFF
#define	KEY_RAM_MARK_VALUE              (0x55)
#define	PWM_IR_DISTANCE_ADDR            0x1000

#define	SCRP_TYPE_ADDR                  0x0200
#define	SCRP_DELAY_H_ADDR               (SCRP_TYPE_ADDR+1)
#define	SCRP_DELAY_L_ADDR               (SCRP_DELAY_H_ADDR+1)
#define	SCRP_NUMS_ADDR                  (SCRP_DELAY_L_ADDR+1)

#define	SCRP_KEYSTART_ADDR              0x0210 

#define MACRO_0_ADDR                    0x0300
#define MACRO_1_ADDR                    0x0380
#define MACRO_2_ADDR                    0x0400
#define MACRO_3_ADDR                    0x0480
#define MACRO_4_ADDR                    0x0500
#define MACRO_5_ADDR                    0x0580
#define MACRO_6_ADDR                    0x0600
#define MACRO_7_ADDR                    0x0680
#define MACRO_8_ADDR                    0x0700
#define MACRO_9_ADDR                    0x0780
#define MACRO_10_ADDR                   0x0800
#define MACRO_11_ADDR                   0x0880
#define MACRO_12_ADDR                   0x0900
#define MACRO_13_ADDR                   0x0980
#define MACRO_14_ADDR                   0x0A00
#define MACRO_15_ADDR                   0x0A80
#define MACRO_16_ADDR                   0x0B00
#define MACRO_17_ADDR                   0x0B80
#define MACRO_18_ADDR                   0x0C00
#define MACRO_19_ADDR                   0x0C80

//#define USE_INTERNAL_EEPROM
#ifdef USE_INTERNAL_EEPROM
#include "InternalEEPROMAPI.h"
static inline void ReadEEPROM ( uint16_t Address, uint8_t *read_data, uint8_t Rcounter )
{
	// Because the first 64 byte is not usable, we shift 64 bytes unless it is >= 0x0fc0
	if(Address<0x0fc0)
	{
		Address += 64;
	}
	readInternalEEPROM (Address, read_data, Rcounter );
}
static inline void StoreEEPROM(uint16_t wAddr, uint8_t bData)
{
	// Because the first 64 byte is not usable, we shift 64 bytes unless it is >= 0x0fc0
	if(Address<0x0fc0)
	{
		Address += 64;
	}
	StoreIntEEPROM(wAddr, bData);
}
#else
#include "ExtEEPROM.h"
static inline void ReadEEPROM ( uint16_t Address, uint8_t *read_data, uint8_t Rcounter )
{
	ReadExtEEPROM (Address, read_data, Rcounter );
}
static inline void StoreEEPROM(uint16_t wAddr, uint8_t bData)
{
	StoreExtEEPROM(wAddr, bData);
}
#endif // USE_INTERNAL_FLASH

#endif //_NVRAM_ACCESS_H
