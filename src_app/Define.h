
#ifndef _DEFINE_H_
#define _DEFINE_H_

//////////////////////////////////////////////
//////////////////////////////////////////////
#include <stddef.h>

//////////////////////////////////////////////
// General
//////////////////////////////////////////////
#ifdef _PORTING_ARM_		 // Add for ARM
typedef unsigned char     bit; 
#define data
#define bdata
#define idata
#define pdata
#define xdata
#define code
#define _at_
#endif // #ifdef _PORTING_ARM_

typedef bit               BIT;
typedef unsigned char     UCHAR;
typedef unsigned char     uCHAR;
typedef unsigned char     BYTE;
typedef unsigned int      UINT;
typedef unsigned int      uINT;
typedef unsigned int      WORD;
typedef unsigned long     ULONG;
typedef unsigned long     uLONG;
typedef unsigned long     DWORD;




typedef  bit             Bit;
//typedef  bit             Bool;
typedef  bit             BOOL;
typedef  unsigned char   Byte;
typedef  unsigned int    Word;




typedef unsigned char     *PUCHAR;
typedef BYTE              *PBYTE;
typedef UINT              *PUINT;
typedef WORD              *PWORD;
typedef ULONG             *PULONG;
typedef DWORD             *PDWORD;
typedef void              *PVOID;





//novatek .......
#define xByte		volatile Byte xdata
#define xWord		Byte xdata

#define SysRegH 	xByte
#define SysRegX		xWord


/*
// boolean
typedef enum _BOOL
{
    FALSE = 0,
    TRUE
}BOOL;
*/


// combination 2 bytes
typedef union _WordType
{
    struct
    {
        BYTE _byte1;
        BYTE _byte0;
    }separate;

    WORD  total;
}WordType;

// combination 4 bytes
typedef union _DWordType
{
    struct
    {
        BYTE _byte3;
        BYTE _byte2;
        BYTE _byte1;
        BYTE _byte0;
    }separate;

    DWORD  total;
}DWordType;

#ifndef _PORTING_ARM_
// flag byte
typedef struct _FlagByteType
{
    BYTE bit0 : 1;
    BYTE bit1 : 1;
    BYTE bit2 : 1;
    BYTE bit3 : 1;
    BYTE bit4 : 1;
    BYTE bit5 : 1;
    BYTE bit6 : 1;
    BYTE bit7 : 1;
}FlagByteType;
#endif // #ifndef _PORTING_ARM_

//////////////////////////////////////////////
// Keil C
//////////////////////////////////////////////
#define DATA              data
#define BDATA             bdata
#define IDATA             idata
#define PDATA             pdata
#define XDATA             xdata
#define RDATA             code
//NEW ADD DECLEAR
#define Byte             BYTE
//END OF NEW ADD DECLEAR
#define CBYTE             ((unsigned char volatile code  *) 0)
#define DBYTE             ((unsigned char volatile data  *) 0)
#define PBYTE             ((unsigned char volatile pdata *) 0)
#define XBYTE             ((unsigned char volatile xdata *) 0)

//////////////////////////////////////////////
// Macro
//////////////////////////////////////////////
#define HIBYTE_REF(addr)          (*((BYTE *) & (addr)))
#define LOBYTE_REF(addr)          (*((BYTE *) & (addr + 1)))
#define MAKEWORD(value1, value2)  (((WORD)(value1)) * 0x100) + (value2)
#define MAKEUINT(value1, value2)  (((WORD)(value1)) * 0x100) + (value2)
#define H2BYTE(value)             ((BYTE)((value) / 0x10000))
#define HIBYTE(value)             ((BYTE)((value) / 0x100))
#define LOBYTE(value)             ((BYTE)(value))

#define HINIBBLE(value)           ((value) / 0x10)
#define LONIBBLE(value)           ((value) & 0x0f)

#define max(a, b)                 (((a) > (b)) ? (a) : (b))
#define min(a ,b)                 (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////
// Others
//////////////////////////////////////////////
#define _ENABLE      1
#define _DISABLE     0

// pin
#define _HIGH    1
#define _LOW     0

// bit
#define _BIT0	0x0001
#define _BIT1	0x0002
#define _BIT2	0x0004
#define _BIT3	0x0008
#define _BIT4	0x0010
#define _BIT5	0x0020
#define _BIT6	0x0040
#define _BIT7	0x0080
#define _BIT8	0x0100
#define _BIT9	0x0200
#define _BIT10	0x0400
#define _BIT11	0x0800
#define _BIT12	0x1000
#define _BIT13	0x2000
#define _BIT14	0x4000
#define _BIT15	0x8000

#ifdef _PORTING_ARM_
#else
#define _bit0_(val)  ((bit)(val & _BIT0))
#define _bit1_(val)  ((bit)(val & _BIT1))
#define _bit2_(val)  ((bit)(val & _BIT2))
#define _bit3_(val)  ((bit)(val & _BIT3))
#define _bit4_(val)  ((bit)(val & _BIT4))
#define _bit5_(val)  ((bit)(val & _BIT5))
#define _bit6_(val)  ((bit)(val & _BIT6))
#define _bit7_(val)  ((bit)(val & _BIT7))
#define _bit8_(val)  ((bit)(val & _BIT8))
#define _bit9_(val)  ((bit)(val & _BIT9))
#define _bit10_(val) ((bit)(val & _BIT10))
#define _bit11_(val) ((bit)(val & _BIT11))
#define _bit12_(val) ((bit)(val & _BIT12))
#define _bit13_(val) ((bit)(val & _BIT13))
#define _bit14_(val) ((bit)(val & _BIT14))
#define _bit15_(val) ((bit)(val & _BIT15))
#endif // #ifndef _PORTING_ARM_

// register
typedef struct _RegUnitType
{
    Word index;
    BYTE value;
}RegUnitType;    

typedef struct _VDRegUnitType
{
    Byte index;
    BYTE value;
}VDRegUnitType; 


typedef enum _vdSignalType
{
    SIG_NTSC,
    SIG_PAL,
    SIG_SECAM,
    SIG_NUMS, // signal number

    SIG_NONE = -1
}vdSignalType;




#define _END_OF_TBL_ -1 // end of register table


//#define CC_VCHIP

#endif

