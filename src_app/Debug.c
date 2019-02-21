/*==========================================================================
|
|  Project Name : LCD TV RC controller
|
|  File Name	: Debug.C
|
|===========================================================================
|
|  Copyright TPV Technology Group /P-Harmony Monitors (Taiwan) Ltd.
|  All rights are reserved. Reproduction in full or in part is prohibited
|  without the prior written consent of the copyright owner.
|
|  COMPANY CONFIDENTIAL
|
|  Date 		Author(s)		Status & Comments
|---------------------------------------------------------------------------
|  2007/04/27	HW Hsu 			Draft
|
|===========================================================================
|
|  Brief Description
|
|  This file is debug list and history.
|
|  Note
|
|  The KEIL C51 Compiler ( Version 7.07a ) is used.
|
==========================================================================*/

//#define _DEBUG_C_

#include "board.h"

// System

// Common
#include "Define.h"

// Internal
#define  GLOBAL
#include "Debug.h"
#undef GLOBAL

// External
#define  GLOBAL extern


#include "app_uart.h"
#include "version.h"
#undef GLOBAL

//#define _CODE_VERSION_    "V0.30.2"
///////////////////////////////////////
// Version message
///////////////////////////////////////
void ShowVersionMessage(void)
{
#if (_DEBUG_OUTPUT_EN_)
    UARTputstr("\r\n\r\n");
    UARTputstr("\r\n==========================================================");
    UARTputstr("\r\n[Project Name]: Universal Remote Controller" );
//	UARTputstr("  �U�λ�����");
//    UARTputstr("\r\n[Company]:      Department  ");
    UARTputstr("\r\n[Designed]:     LCD TV software development group");
    UARTputstr("\r\n[Leading]:      HW Hsu && Jeremy");
    UARTputstr("\r\n[Version]:	    ");
    UARTputstr(VERSION);
    UARTputstr("\r\n[Date]:         "_CODE_DATE_);
    UARTputstr("\r\n==========================================================");
    UARTputstr("\r\n\r\n");

#endif
}

///////////////////////////////////////////////////////////////////////////////
//V0.01 20070427    1.Bema 	: the compiler setting for one bank in RC.UV2.
//                  2.Bema 	: the original code is from BDS_EU 2006/6/25, keep Timer0, RS232,CPU initial setting.
//                  3.Bema 	: modify IO for 72khz output/11.0592 mhz output.
//V0.02 20070502    1.Bema 	: add Keyscan.c Nvram.c and  modify RC.UV2.
//V0.03 20070503    1.Bema 	: add storeEEPROMy in Nvram_Access.c 
//                  2.Bema 	: add getkey, keyscan function in keyscan.c 
//V0.04 20070504    1.Bema 	: add TH0_Step define for 1ms/100us/200us/500us
//				    2.Bema 	: add IR_Carrier function/add to Ir.c to UV2
//V0.05 20070507    1.Bema 	: add timer0 mode 2 auto reload.89us and IP set timer0 high pority.
//V0.06 20070509    1.Bema 	: add timer0 mode 1 222us test IR in main loop code.
//V0.07 20070510    1.Bema 	: remove watch dog set in timer0, o
//                  2.Bema  : use 89us timer0 mode2 reload 8 bit.
//                  3.Bema  : use reference wbakupIrCodeCounter  to IR_TimeTBL. only 0.5 second in main loop.
//                  4.Bema  : use FIFO to generate the code.
//                  5.Bema  : add one ending bit to let IR out Low
//V0.08 20070512    1.Bema 	: add toggle bit machanism. and keyscan timer using 100ms, other the Rs232 can not work.
//V0.09 20070514    1.Bema 	: add toggle bit machanism. and keyscan timer using 100ms, other the Rs232 can not work.
//				    1.Bema 	: timer skill for keyscan timer/10ms base.
//				    2.Bema 	: (0,82) function rc5 to Ir 0x2012 s2 low while >64
//				    3.Bema 	: xdata for NVRAM code sizer.
//V0.10 20070515    1.Bema 	: IrRaminitial.
//    				2.Bema 	: 24C32 read write ok.
//V0.11 20070516    1.Bema 	: Seperate the RC5/6 timer, not use define.
//V0.12 20070517    1.Bema 	: add IR status structure. add IR_time base/ ir carrier frequency.
//				    2.Bema 	: add MCU_12MHZ define status structure. add IR_time base/ ir carrier frequency.
//				    3.Bema 	: remove MCU_12MHZ define .use timer0 mode 2 16 bit for +-10%.
//V0.13 20070518    1.Ben   : Add RC6 IR Function & merge code.
//                  2.Ben   : Disable IrFIFOMax variable use bIrTimeIndexMax(Data type) to make calculate quickly.
//                  3.Bema  : Add UART cmd; restore the ircode table if 0xfff is not 0x55;
//V0.14 20070521    1.Bema  : Add URC function. 
//                  2.Bema  : Remove SICP.C / modify .UV2 files.
//                  3.Bema  : modify URC cmd.
//					4.Bema  : Ir carrier +- 2%;T-time: +-5%
//V0.15 20070522    1.Bema  : modify EEPROM address A0 to A8;
//                  2.Bema  : sRC_TIMETABLE[46] size change to 46, others the xdata will crupt;
//V0.16 20070523    1.Yss   : Add flash led light when press key.
//                  2.Bema  : Ir time interval 110ms.
//                  3.Bema  : add siRepearCounter for repeat function.
//                  4.Bema  : modify UMD function.
//V0.17 20070524    1.Bema  : add reset ram UMD function.
//					2.Bema  : first time to send the code time.
//V0.18 20070525    1.Bema  : Ir carrier frequecy not changed while RC5/6 changed.
//V0.19 20070529    1.Bema  : add RC_null type. ir carrier cycle not changed.add PRODUCTION in complier.
//                  2.Bema  : keyrepeat using timer control RC5 114ms// RC6 106ms.
//V0.20 20070530    1.Bema  : using IR_TimeLevel for time stamp and level;CONTER_OFFSET 10.
//                  2.Yuji  : Add NEC IR Function & merge code.
//V0.21 20070531    1.Bema  : add ES=1 for PRODUCTION define.
//                  2.Bema  : Add Uart for repeat function.
//                  3.Yuji  : Add NEC another type for another repeat function.
//                  4.Bill  : Add IR RC5X, RC6, NEC decoder code.
//V0.22 20070601    1.Bill  : Arrang the whole code
//                  2.Bill  : Arrange code part2.
//                  3.Bill  : Arrange code part3, solve the switching bugs between IR encoder and decoder mode.
//                  4.Bill  : protect the setting values loaded into the TL0, TH0
//V0.23 20070605    1.Bema  : While IR decoder send $IRK 6 RC5/RC6/NEC to PC,UARTputstr("$ACK"),to pc uart cmd.
//					2.Bema  : Add uart change mode cmd.
//V0.24 20070611    1.Bema  : modify change mode LED flash.
//V0.25 20070612    1.Bema  : add UART CMD direc cmd and IR_RAM_TBL_MAX=48-->49 for pc use.
//V0.26 20070613    1.Bema  : uart cmd length change to 6 for direct address;
//					2.Bema  : add UART CMD MWRITE_NVRAM/ MREAD_NVRAM
//V0.27 20070615    1.Yss   : add UARTputstr("$END") when T_DEVIATION in uart cmd
//                  2.Yss   : add putstr("....\r\n") in IrTimerBase function
//					3.Bema  : auto mode with key repeat function.
//V0.28 20070618    1.Bema  : prevent change mode, the press key function ack to pc.
//                  2.Bema  : mode change initial flash the light.
//                  3.Bema  : use uartpintf for pc ack if add production.
//                  4.Bema  : modify RC6 Time table and sloved smart picture/cc/sleep bug.
//V0.29 20070620    1.Bema  : RS232 protocol with PC redefine. ack, length, xor checksum.puchar(); not used ascii
//V0.30 20070621    1.Bema  : Add RS232 status mode; script delay time long int.					
//                  2.Bema  : the long int in timer0 will cause error, use two word instead.
//                  3.Bema  : add version code for pc mode get. add production.
//V1.00 20070621    1.Bema  : update version to 1.0
//V1.01 20070621    1.Bema  : check mode detect switch, not store in nvram; led flash use timer.
//					2.Bema  : uart not send \r\n to pc.
//V1.02 20070623    1.Bema  : finetune T: RC5 total 28T=24.892ms, RC6 52T=23.02ms.
//V1.03 20070626    1.Bema  : add MACRO mode for RC board.(irMarco.c;irmarco.h.)
//					2.Bema  : optimize uart setting.
//V1.04 20070627    1.Bema  : modify the title using chinese font.
//                  2.Bema  : while null key in Tranismitter,then sIR_STATUS.bKeyPressed=0, to solve pc tool fail. 
//					3.Bema  : add MACRO mode for PC tool directly control. 
//					4.Bema  : add busy cmd for MACRO/ENCODER mode.;key/rs232 cmd key  over 20 do nothing. 
//V1.05 20070628    1.Bill  : fix the LED mode flash times, modify the UART Txd buffer algorithm.
//V1.06 20070629    1.Bill  : modify the UART Rxd buffer and command process algorithm.
//                  2.Bill  : fix the mode LED flash period and modify the UART RxD reset time interval.
//                  3.Bill  : Use the base timer to shorten the interrupt service routine.
//V1.07 20070702    1.Bill  : optimize code.
//V1.08 20070703    1.Bill  : Correct the timer used for the ir decoder.
//                  2.Bill  : fix bugs: mixed IR type keys send the wrong IR pulse.
//                  3.Bema  : while null mode key, send 0 to pc, for pc script mode.
//V1.09 20070704    1.Bema  : Add pc sript mode, the same as ir encoder mode.
//                  2.Bill  : fix bug: the watchdog reset in the PC script mode.
//V1.10 20070705    1.Bill  : Add Lite Version to turn off some functions.
//V1.11 20071001    1.Bill  : Add SONY decode functions.
//V1.12 20071016    1.Yuji  : Add SONY encode functions.
//                  2.Yuji  : Modify SONY encode functions.
//V1.13 20071101    1.Bema  : Add FRC define, for 16 key, others 48keys.
//V1.14 20071101    1.Bema  : FRC default MACRO mode.
//V1.15 20080411    1.Bema  : Add Sharp RC receive function.
//V1.16 20080411    1.Bema  : Add Sharp RC Encode function.
//                  2.Bema  : Modify Sharp RC encode functions."PC" key is special
//V1.17 20080610    1.Bill  : Modify the Sharp Base freq of encodering and fix a bug
//V1.18 20100715    1.Bill  : add Matsushita RC decoder protocol */
//V1.19 20100717    1.Bill  : add Matsushita RC enecoder protocol */
//V1.20 20100719    1.Bill  : Fix: Matsushita RC code data conversion issue */
//V1.22 20120308    1.HW    : Add Panasonic RC encode and decoder protocol.
//V2.00 20121210    1.HW    : Transfer MCU to ARM M0
//V2.01 20130110    1.HW	: Full function migrate ready.
#ifdef  LITE_VERSION
    Byte code VERSION[] 	={"Lite V1.14.1"};
#else
    Byte code VERSION[] 	={"V2.01.0"};
#endif


