/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/
#define RELEASE_MODE DEBUG_ALL_LOG

#include "board.h"
#include "gpio.h"
#include "i2c.h"
#include "adc.h"
#include "Timer.h"
#include "Uart.h"
#include "usb_common.h"

// Common
#include "Define.h"
#include "string.h"
// Internal
#define  GLOBAL
#undef GLOBAL

// External
#define  GLOBAL extern
#include "global.h"
#include "Ir.h"
#include "version.h"
#include "app_uart.h"
#include "Io.h"
#include "Nvram_Access.h"
#include "Keyscan.h"
#include "IrAuto.h"
#include "IrMacro.h"
#include "timeout.h"
#include "Initial.h"
#include "Debug.h"
#include "IrDA.h"
#include "app_timer.h"
#include "Common.h"
#include "automata_common.h"
#include "CheckTouchRC.h"
#undef GLOBAL

//#include "cec.h"
extern void SystemInit(void);

extern volatile uint8_t ir_address,ir_command,valid_key;
//extern volatile uint32_t timer32_0_counter;

/* Main Program */

// For unit-test only
//void TestIrTxRxInit(void)
//{
//	bIrTimeIndexIn  = 0;
//	bIrTimeIndexOut = 0;
//	IR_Transmit_Buffer_Init();
//
//	IR_Carrier(38000);	// Setup PWM_period (input parameter in HZ)
//	Setup_IR_PWM_duty_cycle(50); // default is 50%
//	Setup_IR_PWM_Pulse_by_IR_Level(0);
//
//	Chip_GPIO_WriteDirBit(LPC_GPIO_PORT, TEST_OUT1_GPIO_PORT_NUM, TEST_OUT1_GPIO_BIT_NUM, true); 	// Set this as output
//
//	GPIOInit();
//	IrDA_init();
//}
//
//void TestIrTx(void)
//{
//    IR_Transmit_Buffer_Push(1,10000);
//    IR_Transmit_Buffer_Push(0,20000);
//    IR_Transmit_Buffer_Push(1,30000);
//    IR_Transmit_Buffer_Push(0,40000);
//    IR_Transmit_Buffer_StartSend();
//}
//
//void TestIrRx(void)
//{
//    uint32_t  	bLevel;
//    uint32_t  	temp_width;
//    uint32_t	wTimeStamp;
//
//	while (IRDA_Pulse_Pop(&temp_width)!=FALSE)
//	{
//		temp_width = PCLK_FREQUENCY / temp_width;
//        if (temp_width>10000)
//        {
//        	//UARTprintf("IrDA:%d\r\n", temp_width);
//			Find_Carry_Frequency(temp_width);
//        }
//	}
//
//	while (bIrTimeIndexOut !=bIrTimeIndexIn)
//	{
//        wTimeStamp = sRC_TIMETABLE[bIrTimeIndexOut].wTimeStamp;
//        bLevel = sRC_TIMETABLE[bIrTimeIndexOut].bLevel;
//        UARTputchar('0'+bLevel);
//        UARTprintf(":%d\r\n", wTimeStamp);
//        //IR_Transmit_Buffer_Push(bLevel,wTimeStamp);
//
//		bIrTimeIndexOut++;
//
//		if(bIrTimeIndexOut >= IR_TBL_MAX)
//		{
//			bIrTimeIndexOut = 0;
//		}
//	}
//
//	//TestIrTx();
//	//IR_Transmit_Buffer_StartSend();
//}


int app_main (void)
{
    uint8_t  	bLevel,i;
    uint32_t  	temp_width;
    uint32_t	wTimeStamp;

    SystemInit();
    Board_Init();
    I2CInit();
    MyUSB_Init();
	//UARTInit(57600);
	//UARTInit(115200);
	UARTInit(230400);
//	printfs("*************************\n");
//	printfs("Project:URC Tool ,v1.0\n");
//	printfs("Build date: \a");
//	printfs(__DATE__"\n");
//	printfs("Build time: \a");
//	printfs(__TIME__"\n");
//	printfs("*************************\n");

//	UARTputstr("*************************\r\n");
//	UARTputstr("Project:URC IR ,v1.0\r\n");
//	UARTputstr("Build date: "__DATE__"\r\n");
//	UARTputstr("Build time: "__TIME__"\r\n");
//	UARTputstr("*************************\r\n");

//  example of UARTprintf:
//	temp_width = 0x12AB45E8;
//	UARTprintf("x  %x : ", temp_width);
//	UARTprintf("d  %d\n", temp_width);
//	UARTprintf("2x %2x\n", temp_width);
//	UARTprintf("4x %4x\n", temp_width);
//	UARTprintf("6x %6x\n", temp_width);
//	UARTprintf("8x %8x\n", temp_width);
//	UARTprintf("X  %X\n", temp_width);
//	UARTprintf("2X %2X\n", temp_width);
//	UARTprintf("4X %4X\n", temp_width);
//	UARTprintf("6X %6X\n", temp_width);
//	UARTprintf("8X %8X\n", temp_width);

	/* Initialize the PWM first, then init Timer */
	// Set IR TX pin as output
	IR_Carrier(38000);	// Setup PWM_period (input parameter in HZ)
	Setup_IR_PWM_duty_cycle(50); // default is 50%
	Setup_IR_PWM_Pulse_by_IR_Level(0);
	Timer_Init();

	ADCInit(); /* Initialize ADC (AD0 only) */

	GPIOInit();
	IrDA_init();
	Set_LED(1);
    SystemTimer10ms = 10000;
    b_decoding = FALSE;

	PowerOnInit();
    ShowVersionMessage();   // show version message
    CheckMode();
	Ir_Mode_Ini(bFunctionMode);

//	temp_width = 32000;
//    while (1)
//    {
//		IR_Transmit_Buffer_Init();
//    	IR_Transmit_Buffer_Push(1,500);
//    	IR_Transmit_Buffer_Push(0,1000);
//    	IR_Transmit_Buffer_Push(1,3000);
//    	IR_Transmit_Buffer_Push(0,300);
//    	IR_Transmit_Buffer_Push(1,1200);
//    	IR_Carrier(temp_width);	// Setup PWM_period (input parameter in HZ)
//     	IR_Transmit_Buffer_StartSend();
//     	Delayus(1000000);
//    }

	while (1)                                /* Loop forever */
	{

		USB_task_in_main_loop();

		// For testing IrDA measurement
		while (IRDA_Pulse_Pop(&temp_width)!=FALSE)
		{
			temp_width = 48000000 / temp_width;
			if (temp_width>10000)
				Find_Carry_Frequency(temp_width);

			USB_task_in_main_loop();
		}

/*
		//Testing external I2C EEPROM READ/WRITE
		uint16_t	address = 0;//(8*1024)-1;
		uint8_t		read_data[16];

		while(1)
		{
			address = 0;
			while(address<((8*1024)-1))
			{

				ReadEEPROM ( address, read_data, 8 );
			    //StoreEEPROM(uint16_t wAddr, uint8_t bData)

				address+=8;
				USB_task_in_main_loop();
				Delayus(1000);
			}
		}
*/
/*
		// Testing decode new protocol
		while (1)
		{

			USB_task_in_main_loop();

			// Check if pulse available
			if (bIrTimeIndexOut !=bIrTimeIndexIn)
			{
				wTimeStamp = sRC_TIMETABLE[bIrTimeIndexOut].wTimeStamp;
				bLevel = sRC_TIMETABLE[bIrTimeIndexOut].bLevel;
				bIrTimeIndexOut = INC_INDEX8(bIrTimeIndexOut,IR_TBL_MAX);
//				uint32_t	temp_value;
//				temp_value = wTimeStamp>>4;
//				UARTputHEX_U((temp_value>>8)&0xff);
//				UARTputHEX_U(temp_value&0xff);
//				UARTputchar('\r');
//				UARTputchar('\n');

				// if pulse available
				_MDrv_IR_ISR(wTimeStamp);
			}

			USB_task_in_main_loop();

			// Required when Timeout mechanism is implemented
			if(CheckMyTimer_us_TimeOut()!=FALSE)
			{
				ClearMyTimer_us();
				_MDrv_IR_ISR(RC6_AUTOMATA_TIMEOUT_TIME);
			}

			// END
			USB_task_in_main_loop();
		}
*/
/*
// Personal Testing Purpose

		IR_Transmit_Buffer_Init();
		while (1)
		{
			uint32_t data_packet = (0x300401)<<1 | 0x01 ;
			uint32_t bit_index;

 // Test code for transmission
			if(bIrTimeIndexOut_Output == bIrTimeIndexIn_Output)
			{
				bit_index=29;
				do
				{
					if (data_packet&0x01)
					{
						IR_Transmit_Buffer_Push(1,264);
						IR_Transmit_Buffer_Push(0,262);
					}
					else
					{
						IR_Transmit_Buffer_Push(0,264);
						IR_Transmit_Buffer_Push(1,262);
					}
					data_packet>>=1;
					bit_index--;
				}
				while (bit_index > 0);
				IR_Transmit_Buffer_Push(0,(20000-(263*2*29)));
				IR_Carrier(38000);	// Setup PWM_period (input parameter in HZ)
				IR_Transmit_Buffer_StartSend();
			}

			 // Test code for IR receiver

			while (bIrTimeIndexOut !=bIrTimeIndexIn)
			{
				wTimeStamp = sRC_TIMETABLE[bIrTimeIndexOut].wTimeStamp;
				bLevel = sRC_TIMETABLE[bIrTimeIndexOut].bLevel;

				if(bLevel!=0)
				{
					UARTputchar('+');
				}
				else
				{
					UARTputchar('-');
				}

				if(wTimeStamp>=0x1000)
				{
					UARTputHEX_U(wTimeStamp>>8);
				}
				else if(wTimeStamp>=0x100)
				{
					uint8_t temp_char = (wTimeStamp>>8)&0x0f;
					if(temp_char>=10)
					{
						UARTputchar('A'+temp_char-10);
					}
					else
					{
						UARTputchar('0'+temp_char);
					}
				}

				wTimeStamp&=0xff;
				if(wTimeStamp>=0x10)
				{
					UARTputHEX_U(wTimeStamp);
				}
				else
				{
					if(wTimeStamp>=10)
					{
						UARTputchar('A'+wTimeStamp-10);
					}
					else
					{
						UARTputchar('0'+wTimeStamp);
					}
				}
				UARTputchar('\r');
				UARTputchar('\n');
				//USB_task_in_main_loop();
				bIrTimeIndexOut = INC_INDEX8(bIrTimeIndexOut,IR_TBL_MAX);
			}
		}
*/


		// For testing Timer1 Capture for CEC
//		if(CEC_Pulse_Pop(&bLevel, &temp_width )!=FALSE)
//		{
//            // Post-processing to become IR Rx data
//            temp_width = (temp_width*TIMER16B0_PRESCALER)/TIMER0_PRESCALER;
//            if(bLevel!=0)
//            {
//            	bLevel = 0;
//            }
//            else
//            {
//            	bLevel = 1;
//            }
//			printf("%d:", bLevel);
//			printf("%d\n", temp_width);
//
//			// Push into IR Rx buffer for testing purpose
//			sRC_TIMETABLE[bIrTimeIndexIn].wTimeStamp =	(temp_width&0xffff);
//            sRC_TIMETABLE[bIrTimeIndexIn].bLevel = bLevel;
//			bIrTimeIndexIn++;
//			if(bIrTimeIndexIn >= IR_TBL_MAX)
//			{
//				bIrTimeIndexIn = 0;
//			}
//		}

		// Deal with IR Rx buffer in different modes
		if (bFunctionMode == IR_DEC_MODE)
		{
			while (bIrTimeIndexOut !=bIrTimeIndexIn)
			{
				wTimeStamp = sRC_TIMETABLE[bIrTimeIndexOut].wTimeStamp;
				bLevel = sRC_TIMETABLE[bIrTimeIndexOut].bLevel;
				/*
					printf(":%x ", wTimeStamp>>16);
					wTimeStamp = wTimeStamp & 0xFFFF;
					printf("%x ", wTimeStamp>>8);
					wTimeStamp = wTimeStamp & 0xFF;
					printf("%x\r\n", wTimeStamp);
				 */
				bIrTimeIndexOut = INC_INDEX8(bIrTimeIndexOut,IR_TBL_MAX);
				b_decoding = TRUE;
				ScanIR(bLevel,wTimeStamp);
			}
		}
		else if (bFunctionMode == IR_LEARNING_MODE)
		{
			while ((bIrTimeIndexOut !=bIrTimeIndexIn)&&(TryPushIRData()==TRUE))
			{
				IRResetTimerCnt = 0x00;
			}

			if (Wait_for_URD_Data==TRUE)
			{
//				if (IRRear>=MAX_IRXmt_Size)
//				{
//					IRSendDataOut();
//				}
//				else if ((IRResetTimerCnt>12)&&(IRRear>0))
//				{
//					IRSendDataOut();
//				}
				if ((IRRear>0))
					IRSendDataOut();
			}
		}
		else
		{
			bIrTimeIndexOut = bIrTimeIndexIn;
		}
		// end

		if ((bFunctionMode == IR_ENC_MODE)||(bFunctionMode ==IR_PC_SRIPT_MODE))
		{
	        KeyScan();
	        USB_task_in_main_loop();
            PcKeyProcess();
		}
		else  if (bFunctionMode == IR_MARCO_MODE)
		{
            IrMacoKeyScan();
            USB_task_in_main_loop();
            Ir_MacroProc();
		}

		USB_task_in_main_loop();

	    if(ClearIrDecDataFlag)
	    {
	        ClearIrDecDataFlag = 0;
	        if(b_decoding)
	        {
	            b_decoding = FALSE;
	            Reset_ir_dec();
	//             IR_LED_OUT_LOW;
	        }
	    }

	    USB_task_in_main_loop();

//	    if (ClearIrDAFlag && (bFunctionMode == IR_LEARNING_MODE))
   		if (IRDataResetTimerCnt > 20)	// 200ms
   	    {
   	    	memset(&sIrDA_Accumlate, 0, sizeof(sIrDA_Accumlate));
//			ClearIrDAFlag = 0;
  			IRDataResetTimerCnt = 0x00;
   	    }

		UART_Command_Proc();

		USB_task_in_main_loop();

		TimeOutCtrl();


	}
}
