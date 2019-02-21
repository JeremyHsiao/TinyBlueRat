
//#define _UART_C_

#include "chip.h"
#include "board.h"
#include "string.h"
#include "Uart.h"
#include "VirtualSerial.h"

// Common
#include "Define.h"
#include "Common.h"

// System
#include <intrins.h>


// Internal
#define  GLOBAL
#include "app_Uart.h"
#undef GLOBAL

// External
#define  GLOBAL extern
#include "Global.h"
#include "Debug.h"
#include "Nvram_Access.h"
#include "app_timer.h"
#include "Keyscan.h"
#include "Ir.h"
#include "Io.h"
#include "Initial.h"
#include "IrAuto.h"
#include "IrMacro.h"
#include "Timeout.h"
#include "Timer.h"
#undef GLOBAL

// Debug messages
void USB_DEBUG_PutHEX_U(uint8_t hex_value)
{
//	RS232putHEX_U(hex_value);
}

void USB_DEBUG_MSG(uint8_t *sent_data)
{
//	WriteMultiByteToUARTRingBuffer(sent_data,strlen(sent_data));
}
// End

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
uint16_t	Request_RC_RawData_Index, Current_RC_RawData_index;


Byte PopRxdQueue(Byte *return_data)
{
	uint8_t pop_byte_cnt;

	pop_byte_cnt = READ_ONE_BYTE(return_data);
	return pop_byte_cnt;
}

void PushIRData(Byte  bLevel,uint32_t  wTimeStamp)
{
	uint32_t w32Temp;

	 if (IRRear >= MAX_IRXmt_Size)
		 return;

	IRXmt[IRRear++] =  bLevel;
    w32Temp = wTimeStamp>>16;
    IRXmt[IRRear++] =  w32Temp;
    w32Temp = (wTimeStamp & 0xFFFF) >> 8;
    IRXmt[IRRear++] =  w32Temp;
    IRXmt[IRRear++] =  wTimeStamp & 0xFF;
/*
    if (IRRear == 24)
	{
		while (QueueSize >1);
		IRSendDataOut();
	}
*/
	IRResetTimerCnt = 0x00;
}

/*
void IRSendDataOut(void)
{
	Byte i,wChecksum=0x00;
    UARTputstr("$URD");
    UARTputchar(IRRear+1+1);		// data itself (IRRear bytes) + data_lenght (1 byte) + checksum (1 byte)
	for (i=0;i<IRRear;i++)
	{
		UARTputchar(IRXmt[i]);
		wChecksum ^= IRXmt[i];		// Checksum is only data itself
	}
	UARTputchar(wChecksum);
    UARTputstr("\r\n");
	IRRear = 0x00;
}
*/

uint8_t TryPushIRData(void)
{
    uint8_t  	bLevel, i;
    uint32_t  	temp_width, w32Temp;

	// Buffering data until full
	if(IRRear<MAX_IRXmt_Size)
	{
	    temp_width = sRC_TIMETABLE[bIrTimeIndexOut].wTimeStamp;
	    bLevel = sRC_TIMETABLE[bIrTimeIndexOut].bLevel;

	    IRXmt[IRRear++] =  bLevel;
	    w32Temp = (temp_width>>16) & 0xff;
	    IRXmt[IRRear++] =  w32Temp;
	    w32Temp = (temp_width>>8)  & 0xff;
	    IRXmt[IRRear++] =  w32Temp;
	    IRXmt[IRRear++] =  temp_width & 0xFF;

	    bIrTimeIndexOut = INC_INDEX8(bIrTimeIndexOut,IR_TBL_MAX);

	    i = TRUE;
	}
	else
	{
		i = FALSE;
	}
	IRResetTimerCnt = 0x00;

	return i;
}

//uint16_t	Request_RC_RawData_Index, Current_RC_RawData_index;


void IRSendDataOut(void)
{
	Byte i, wChecksum;

	if (IRRear>0)		// Already some data in buffer
	{
		if (Request_RC_RawData_Index == Current_RC_RawData_index )		// Current data is the data requested to deliver
		{
			IRResetTimerCnt = 0x00;

			UARTputstr("$URD");
			//
			UARTputchar((uint8_t)((2*2)+(IRRear*2)+(1*2)));	// Avoid 0x24 (36) and  multiplied by 2 because we use two ASCII to send one byte

			// Current data position
			UARTputHEX_U((Byte)(Current_RC_RawData_index>>8));
			UARTputHEX_U((Byte)(Current_RC_RawData_index&0x00ff));

			wChecksum = 0;
			for (i=0;i<IRRear;i++)
			{
				UARTputHEX_U(IRXmt[i]);
				wChecksum ^= IRXmt[i];		// Checksum is only data itself
			}
//			for (i=IRRear;i<MAX_IRXmt_Size;i++)
//			{
//				UARTputHEX_U(0xff);
//				wChecksum ^= 0xff;
//			}
			UARTputHEX_U(wChecksum);
			UARTputchar('\r');
			UARTputchar('\n');

			for (i=0;i<MAX_IRXmt_Size;i++)
			{
				IRXmt_previous [i] = IRXmt[i];
			}
			IRRear_previous = IRRear;

			Current_RC_RawData_index += IRRear/4;
			//Current_RC_RawData_index += MAX_IRXmt_Size/4;
			IRRear = 0x00;

			IRResetTimerCnt = 0x00;
			Wait_for_URD_Data = FALSE;

		}
		else if (Request_RC_RawData_Index < Current_RC_RawData_index )	// Current data is not the data requested to deliver (i.e., previous data is requested)
		{
			IRResetTimerCnt = 0x00;

			UARTputstr("$URD");
			//
			UARTputchar((uint8_t)((2*2)+(IRRear_previous*2)+(1*2)));	// Avoid 0x24 (36)  multiplied by 2 because we use two ASCII to send one byte

			// Current data position in BCD
			UARTputHEX_U((Byte)(Request_RC_RawData_Index>>8));
			UARTputHEX_U((Byte)(Request_RC_RawData_Index&0x00ff));

			wChecksum = 0;
			for (i=0;i<IRRear_previous;i++)
			{
				UARTputHEX_U(IRXmt_previous[i]);
				wChecksum ^= IRXmt_previous[i];		// Checksum is only data itself
			}
			UARTputHEX_U(wChecksum);
			UARTputchar('\r');
			UARTputchar('\n');

			IRResetTimerCnt = 0x00;
			Wait_for_URD_Data = FALSE;
		}
		else // here Request_RC_RawData_Index > Current_RC_RawData_index
		{
            printfs("Shouldn't be here in Uart.c. Uart.c L639");
			Wait_for_URD_Data = FALSE;
		}
	}
}

//////////////////////////////////////////////////////////////
// Put character to uart
//
// Arguments: ucVal - output character
//////////////////////////////////////////////////////////////
void UARTputchar(BYTE ucVal)
{
	WRITE_ONE_BYTE(ucVal);
	FLUSH_BYTE();
}

//////////////////////////////////////////////////////////////
// Put hexadecimal byte to uart
//
// Arguments: ucVal - output character
//////////////////////////////////////////////////////////////
const BYTE converting_CHAR_L[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
const BYTE converting_char[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
void UARTputhex(BYTE ucVal)
{
	uint8_t temp_data[2];
	BYTE temp;

	// High Nibble
	temp = (ucVal>>4)&0x0f;
	temp = converting_char[temp];
	temp_data[0] = temp; // high nibble

    // Low Nibble
	temp = (ucVal)&0x0f;
	temp = converting_char[temp];
	temp_data[1] = temp; // low nibble

	WRITE_MULTI_BYTE(temp_data, 2);
	FLUSH_BYTE();
}

void UARTputHEX_U(BYTE ucVal)
{
	uint8_t temp_data[2];
	BYTE temp;

	// High Nibble
	temp = (ucVal>>4)&0x0f;
	temp = converting_CHAR_L[temp];
	temp_data[0] = temp; // high nibble

    // Low Nibble
	temp = (ucVal)&0x0f;
	temp = converting_CHAR_L[temp];
	temp_data[1] = temp; // low nibble

	WRITE_MULTI_BYTE(temp_data, 2);
	FLUSH_BYTE();
}

void UARTputdec(uint32_t ucVal)
{
	BYTE temp, temp_number_buffer[20];

	temp = 0;
	do
	{
		temp_number_buffer[temp] = (ucVal % 10) + '0';
		ucVal /= 10;
		temp++;
	}
	while (ucVal>0);

	WRITE_MULTI_BYTE(temp_number_buffer, temp);
	FLUSH_BYTE();
}

// Please self-check if range is 00~99
void UARTputBCD_byte(uint8_t ucVal)
{
	UARTputchar((ucVal / 10) + '0');
	UARTputchar((ucVal % 10) + '0');
}

//////////////////////////////////////////////////////////////////////////////
// Put string to uart.
//
// Arguments: pFmt - string address
//////////////////////////////////////////////////////////////////////////////
void UARTputstr(char *pFmt)
{
    int data_length;

    data_length = strlen(pFmt);
    WRITE_MULTI_BYTE(pFmt, data_length);
    FLUSH_BYTE();
}

//////////////////////////////////////////////////////////////////////////////
// Put string to uart with variable argument
//
// Arguments: pFmt - string address
//            wVal - print variable
//////////////////////////////////////////////////////////////////////////////
void UARTprintf(BYTE code *pFmt, uint32_t wVal)
{
    int32_t 	str_len, temp;
    BYTE 		ucBff; // character buffer
    BYTE 		temp_number_buffer[20];
    void (*puthex_func)(BYTE);

    while (1)
    {
         ucBff = *pFmt; // get a character
         if (ucBff == _EOS_) // check end of string
             break;

         if (ucBff == '%') // check special case
         {
        	ucBff = *(++pFmt);

        	// with length
        	str_len = 0;
        	if ( (ucBff>='2') && (ucBff<='8') ) //2~8
        	{
        		str_len = ucBff-'0';
    			ucBff = *(++pFmt);
        	}

            switch(ucBff) // check next character
            {
                  case 'x': // Hexadecimal number
                  case 'X':

                	  if(ucBff=='x')
                	  {
                		  puthex_func = UARTputhex;
                	  }
                	  else
                	  {
                		  puthex_func = UARTputHEX_U;
                	  }

                	  temp_number_buffer[3]=0;
                	  temp_number_buffer[2]=0;
                	  temp_number_buffer[1]=0;

                	  temp = 2;			// minimal length is 2
                	  temp_number_buffer[0] = wVal & 0xff;
                	  wVal >>= 8;
                	  if(wVal!=0)
                	  {
                		  temp = 4;
                    	  temp_number_buffer[1] = wVal & 0xff;
                    	  wVal >>= 8;
                    	  if(wVal!=0)
                    	  {
                    		  temp = 6;
                    		  temp_number_buffer[2] = wVal & 0xff;
                    		  wVal >>= 8;
                        	  if(wVal!=0)
                        	  {
                        		  temp = 8;
                        		  temp_number_buffer[3] = wVal & 0xff;
                        		  //dwVal >>= 8;							// not required for 32bit input
                        	  }
                    	  }
                	  }

                	  if (str_len==0)
                	  {
                		  str_len = temp;
                	  }

                	  // must be 2,4,6,8 for Hexadecimal number
                	  switch (str_len)
                	  {
                	  	  case 8:
                	  		  puthex_func(temp_number_buffer[3]);
                	  	  case 6:
                	  		  puthex_func(temp_number_buffer[2]);
                	  	  case 4:
                	  		  puthex_func(temp_number_buffer[1]);
                	  	  case 2:
                	  		  puthex_func(temp_number_buffer[0]);
            	  		  	  break;
                	  	  default:
                	  		  // Shouldn't be here now
            	  		  	  break;
                	  }
                	  FLUSH_BYTE();
                      break;
                  case 'd': // decimal number
                  case 'i':
                	  // str_len is not yet implemented in decimal number
                	  temp = 0;
                	  do
                	  {
                		  temp_number_buffer[temp] = (wVal % 10) + '0';
                		  wVal /= 10;
                		  temp++;
                	  }
                	  while (wVal>0);

                	  while (temp>0)
                	  {
                		  temp--;
                		  UARTputchar(temp_number_buffer[temp]);
                		  FLUSH_BYTE();
                	  }
                	  break;
              } // switch
         }
         else // general
         {
        	 UARTputchar(ucBff); // put a character
        	 FLUSH_BYTE();
         }

         pFmt++; // next
    } // while
}

inline void RS232putchar(BYTE ucVal)
{
	WriteOneByteToUARTRingBuffer(ucVal);
}

inline void RS232putHEX_U(BYTE ucVal)
{
	uint8_t temp_data[2];
	BYTE temp;

	// High Nibble
	temp = (ucVal>>4)&0x0f;
	temp = converting_CHAR_L[temp];
	temp_data[0] = temp; // high nibble

    // Low Nibble
	temp = (ucVal)&0x0f;
	temp = converting_CHAR_L[temp];
	temp_data[1] = temp; // low nibble

	WriteMultiByteToUARTRingBuffer(temp_data, 2);
}


//const BYTE converting_CHAR_L[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void UART_Show_CPU_Timer()
{
	uint32_t	temp_timer = Chip_TIMER_ReadCount(LPC_TIMER32_0);
	uint8_t		index=0;
	char		temp_buf[8];

	UARTputchar('[');
	do
	{
      temp_buf[index]=converting_CHAR_L[temp_timer&0xf];
      index++;
      temp_timer>>=4;
	}
	while (temp_timer>0);

	do
	{
      index--;
	  UARTputchar(temp_buf[index]);
	}
	while(index>0);

	UARTputchar(']');
}

void UART_Command_Proc(void)			// 58,CMD, Length .....
{
	Word wAddr,wFreq,wBuf;
	Byte bStatus;
	Byte bKeyIndex;
	signed char scTemp;
	Byte i;
    Byte bRcvData;
    Byte bLen,bCount;
    uint8_t I2CRxd[16];	       /* I2C transmit buffer, 16 bytes max	*/


    /* put RxD's buffer into the command's buffer */
//    if	(sURC.Out_Index != sURC.In_Index)   // the queue is not empty
    if (PopRxdQueue(&bRcvData)>0)
    {
    	//RS232putHEX_U(bRcvData);

        bResetRxdCmdFlag = 0;
        // Need to modified
//        bRcvData = PopRxdQueue();
       
        if( (sURC.Cmd_Index == _UART_CMD_ADDRESS_) 
            && (bRcvData != URC_ADDRESS) )
        {
            return;
        }
        else if(sURC.Cmd_Index == _UART_CMD1_)
        {
            sURC.Cmd_Length = NORMAL_LENGTH;
            
            if(bRcvData == MWRITE_NVRAM)
            {
                sURC.Cmd_Length =_UART_CMD14_; // 14 Bytes
            }
            else if(bRcvData == BUFFER_DATA)
            {
                sURC.Cmd_Length = UART_RXD_MAX_LENGTH;
            }
            else if(bRcvData == SET_URD_ADDRESS)
            {
                sURC.Cmd_Length = 4;
            }
        }
        
        sURC.CmdBuffer[sURC.Cmd_Index++] = bRcvData;
        
        if(sURC.Cmd_Index < sURC.Cmd_Length)
        {
            return;
        }
    }
    else
    {
        if(bResetRxdCmdFlag)
        {
            bResetRxdCmdFlag = 0;
            
            /* reset command index */
            sURC.Cmd_Index  = 0;
            sURC.Cmd_Length = NORMAL_LENGTH;
        }
        return;
    }

    bLen = sURC.Cmd_Length;
    
    /* reset command index */
    sURC.Cmd_Index  = 0;
    sURC.Cmd_Length = NORMAL_LENGTH;

//    printf("\r\n UART CMD = %d ", sURC.CmdBuffer[_UART_CMD1_]);
    
    /* process command */

    switch(sURC.CmdBuffer[_UART_CMD1_])
    {
        case GETSTATUS:
        {
            if(sURC.CmdBuffer[_UART_CMD2_] == 0x01)
            {
                UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
                
                bStatus = bFunctionMode;
                   
                UARTputchar(bStatus);
                
                UARTputstr("\r\n");
            }
            else if (sURC.CmdBuffer[_UART_CMD2_] == 0x02)
            {   
                if((bFunctionMode == IR_ENC_MODE) || (bFunctionMode == IR_PC_SRIPT_MODE))
                {
                    UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
                    
                    bStatus = sIR_STATUS.bKeyPressed;
                    UARTputchar(bStatus);
                    UARTputstr("\r\n");
                }
                else if (bFunctionMode == IR_MARCO_MODE)
                {
                    UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
                    
                    bStatus = sIR_STATUS.bMarcoRun;
                    UARTputchar(bStatus);
                    UARTputstr("\r\n");
                }
            }
            break;   
        }    
        case CHANGEMODE:
        {
            bFunctionMode = sURC.CmdBuffer[_UART_CMD2_];
			StoreEEPROM(MODE_ADDR, bFunctionMode);
//            printf("here %X", bFunctionMode);
            
            switch(bFunctionMode)
            {
				case IR_PC_SRIPT_MODE:
                case IR_ENC_MODE:
                    UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                    UARTputstr("\r\n");
                    break;											 
                
                case IR_DEC_MODE:
                    UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                    UARTputstr("\r\n");
                    break;
                
                case IR_AUTO_MODE:
                    UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                    UARTputstr("\r\n");
                    break;
                
                case IR_MARCO_MODE:
                    UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                    UARTputstr("\r\n");
                    break;
                
                case IR_LEARNING_MODE:
				case IR_SIMU_MODE:
//				case IR_SELFTEST_MODE:
                    UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                    UARTputstr("\r\n");
                    break;

                case IR_NOTHING_MODE:
                    UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                    UARTputstr("\r\n");
                    break;
                    
                default:
                    UARTputstr("\r\n Error: not correct mode");
                    return;
            }

            Ir_Mode_Ini(bFunctionMode);
//			if (bFunctionMode == IR_SELFTEST_MODE)
//			{
//				sIR_STATUS.IrType =  sURC.CmdBuffer[_UART_CMD3_];
//				IR_CarrierBase();
//			}
            break;
        }
        case PRESSKEY:
        {   
            if((bFunctionMode == IR_ENC_MODE) || (bFunctionMode == IR_PC_SRIPT_MODE))
            {
                UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                UARTputstr("\r\n");
                
                bKeyIndex = sURC.CmdBuffer[_UART_CMD2_] - 1;
                IR_TimeTBL(bKeyIndex, TRUE);    /* first pressed */
                StartRepeatKeyTimer();
            }
            else if (bFunctionMode == IR_MARCO_MODE)
            {
                UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
                UARTputstr("\r\n");
                
                bKeyIndex = sURC.CmdBuffer[_UART_CMD2_] - 1;
                
                if(bKeyIndex < IR_MACRO_KEY_MAX)
                {
                    IrMacroRAMIni(bKeyIndex);
                    
                    StartMacro();
                    
                    StartMacroKeyTimer();
                }
                else
                {
                    UARTputstr("\r\n Error: not in correct range");
                    return;
                }
            }
            else
            {
                UARTputstr("\r\n Error: not in correct mode");
                return;
            }
            break;   
        }    
        case UNPRESSKEY:
        {
            UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");

            if((bFunctionMode == IR_ENC_MODE) || (bFunctionMode == IR_PC_SRIPT_MODE))
            {
                StopRepeatKeyTimer();
            }
            else if(bFunctionMode == IR_MARCO_MODE)
            {
                StopMacro();
            }
            break;   
        }    
        case DIRECT_CODE:
        {
            UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");
            
            bKeyIndex = 48;
            sIR_RAM_TBL[bKeyIndex].Type     = sURC.CmdBuffer[_UART_CMD2_];
            sIR_RAM_TBL[bKeyIndex].Mode     = sURC.CmdBuffer[_UART_CMD3_];
            sIR_RAM_TBL[bKeyIndex].Address  = sURC.CmdBuffer[_UART_CMD4_];
            sIR_RAM_TBL[bKeyIndex].Cmd      = sURC.CmdBuffer[_UART_CMD5_];
            sIR_RAM_TBL[bKeyIndex].Repeat   = 1;
            sIR_RAM_TBL[bKeyIndex].Rev0     = 0;
            sIR_RAM_TBL[bKeyIndex].Rev1     = 0;
            sIR_RAM_TBL[bKeyIndex].Rev2     = 0;
          
            IR_TimeTBL(bKeyIndex, TRUE);    /* first pressed */
            break;   
        }   
        case CARRIER_DEV:
        {
            UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");
            
            scTemp = sURC.CmdBuffer[_UART_CMD2_];
            if(scTemp >= 2)
            {
               scTemp = 2;
            }
            
            if(scTemp < -2)
            {
               scTemp = -2;
            }
            
            scTemp = 0;//carrier frequency no change.                      
            StoreEEPROM( CARRIER_ADDR, scTemp);
			scCarrierdif = scTemp;
            break;   
        }   
        case T_DEVIATION://0x84
        {
            UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");

            scTemp = sURC.CmdBuffer[_UART_CMD2_];
            if(scTemp > 20)
            {
               scTemp = 20;
            }

            if(scTemp < -20)
            {
               scTemp = -20;
            }

            StoreEEPROM(DEVIATION_ADDR, scTemp);

			scTdiffer = scTemp;

			IrTimerBase();
            break;   
        }    
        case RESET_RAM:
        {
            UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");
            
            IrCodeDefault();
            break;   
        }    
        case WRITENVRAM:
        {
         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");
            
         	wAddr = (Word)((sURC.CmdBuffer[_UART_CMD2_] << 8) | sURC.CmdBuffer[_UART_CMD3_]);
			StoreEEPROM(wAddr, sURC.CmdBuffer[_UART_CMD4_]);
            break;   
        }   
        case READNVRAM:
        {
			UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
			wAddr= (Word)((sURC.CmdBuffer[_UART_CMD2_] << 8) | sURC.CmdBuffer[_UART_CMD3_]);
			ReadEEPROM(wAddr, I2CRxd, 1);

			UARTputchar(I2CRxd[0]);
            UARTputstr("\r\n");
            break;   
        } 

//        case PWM_WRITE:
//        {
//         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
//            UARTputstr("\r\n");
//
//         	wAddr = (Word)(PWM_IR_DISTANCE_ADDR+(Word)(sURC.CmdBuffer[_UART_CMD2_])*2);
//			StoreEEPROM(wAddr, sURC.CmdBuffer[_UART_CMD3_]);
//			StoreEEPROM(wAddr+1, sURC.CmdBuffer[_UART_CMD4_]);
//            break;
//        }
//        case PWM_READ:
//        {
//			UART_CheckSum3(2, &sURC.CmdBuffer[0], bLen);
//			wAddr= (Word)(PWM_IR_DISTANCE_ADDR+(Word)(sURC.CmdBuffer[_UART_CMD2_])*2);
//			ReadEEPROM(wAddr, 2);
//			UARTputchar(I2CRxd[0]);
//			UARTputchar(I2CRxd[1]);
//            UARTputstr("\r\n");
//            break;
//        }
//        case REG_WRITE:
//        {
//         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
//            UARTputstr("\r\n");
//
//         	wAddr = (Word)((sURC.CmdBuffer[_UART_CMD2_] << 8) | sURC.CmdBuffer[_UART_CMD3_]);
//			bRegptr = (Byte *)wAddr;
//			*bRegptr = sURC.CmdBuffer[_UART_CMD4_];
////			StoreEEPROM(wAddr, sURC.CmdBuffer[_UART_CMD4_]);
//            break;
//        }
//        case REG_READ:
//        {
//			UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
//			wAddr= (Word)((sURC.CmdBuffer[_UART_CMD2_] << 8) | sURC.CmdBuffer[_UART_CMD3_]);
//			bRegptr = (Byte *)wAddr;
////			I2CRxd[0] =  *bRegptr;
////			ReadEEPROM(wAddr, 1);
//			UARTputchar(*bRegptr);
//            UARTputstr("\r\n");
//            break;
//        }

//        case DIMMING_CTRL:
//        {
//         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
//            UARTputstr("\r\n");
//			PWM_REG[3] =  sURC.CmdBuffer[_UART_CMD2_];
//            break;
//        }

//        case LIGHT_SENSOR:
//        {
///*
//			Delay1ms(30);
//			ADC_CON |= STRT_ADC;		// Start the ADC conversion, It need wait 12us
//			Delay1ms(2);
////			UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
//			UART_CheckSum2(2, &sURC.CmdBuffer[0], bLen);
//			UARTputchar(ADC_REG[1]);
//            UARTputstr("\r\n");
//*/
//			wBuf = 0x00;
////			Delay1ms(30);
//			for(i=0;i<16;i++)
//			{
//				ADC_CON |= STRT_ADC;		// Start the ADC conversion, It need wait 12us
//				Delay1ms(2);
//				wBuf += ADC_REG[1];
//			}
//			wBuf = wBuf/16;
////			UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
//			UART_CheckSum2(2, &sURC.CmdBuffer[0], bLen);
//			UARTputchar(wBuf);
//            UARTputstr("\r\n");
//            break;
//        }

//        case LIGHT_SENSOR2:
//        {
///*
//			Delay1ms(30);
//			ADC_CON |= STRT_ADC;		// Start the ADC conversion, It need wait 12us
//			Delay1ms(2);
////			UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
//			UART_CheckSum2(2, &sURC.CmdBuffer[0], bLen);
//			UARTputchar(ADC_REG[1]);
//            UARTputstr("\r\n");
//*/
//			wBuf = 0x00;
////			Delay1ms(30);
//			for(i=0;i<16;i++)
//			{
//				ADC_CON |= STRT_ADC;		// Start the ADC conversion, It need wait 12us
//				Delay1ms(2);
//				wBuf += ADC_REG[1];
//			}
//			wBuf = wBuf/16;
//			UART_CheckSum(2, &sURC.CmdBuffer[0], bLen);
////			UART_CheckSum2(2, &sURC.CmdBuffer[0], bLen);
//			UARTputchar(wBuf);
//            UARTputstr("\r\n");
//            break;
//        }

//        case LASER_CTRL:
//        {
//         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
//            UARTputstr("\r\n");
//			if (sURC.CmdBuffer[_UART_CMD2_])
//				PortA |=PA7;						//High
//			else
//				PortA &= ~PA7;						//Low
//            break;
//        }

//        case ROTATION_CTRL:
//        {
//         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
//            UARTputstr("\r\n");
//			PortC &= 0xF0;
//			PortC |= (sURC.CmdBuffer[_UART_CMD2_]&0x0F);
//            break;
//        }
		
//		case ROTATION_READY:
//		{
//			UART_CheckSum4(2, &sURC.CmdBuffer[0], bLen);
//			bStatus = PortC;
//			bStatus &= PC4;
//			if (bStatus == PC4)
//				UARTputchar(1);
//			else
//				UARTputchar(0);	      // Ready is low.
//            UARTputstr("\r\n");
//			break;
//		}

        case MWRITE_NVRAM:
        {
         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
         	UARTputstr("\r\n");
            
			wAddr = (Word)((sURC.CmdBuffer[_UART_CMD2_] << 8) | sURC.CmdBuffer[_UART_CMD3_]);
			for(i = 0; i < 8; i++)
			{
			    StoreEEPROM(wAddr+i, sURC.CmdBuffer[_UART_CMD6_+i]);
            }
            break;   
        }   
        case MREAD_NVRAM:
        {
            UART_CheckSum(9, &sURC.CmdBuffer[0], bLen);
			wAddr = (Word)((sURC.CmdBuffer[_UART_CMD2_] << 8) | sURC.CmdBuffer[_UART_CMD3_]);
			ReadEEPROM(wAddr, I2CRxd, 8);
			UARTputchar(I2CRxd[0]);
			UARTputchar(I2CRxd[1]);
			UARTputchar(I2CRxd[2]);
			UARTputchar(I2CRxd[3]);
			UARTputchar(I2CRxd[4]);
			UARTputchar(I2CRxd[5]);
			UARTputchar(I2CRxd[6]);
			UARTputchar(I2CRxd[7]);
            UARTputstr("\r\n");
            break;   
        }            
        case CODE_VERSION:
        {
            UART_CheckSum(strlen(VERSION) + 1, &sURC.CmdBuffer[0], bLen);
			UARTputstr((char *)VERSION);
            UARTputstr("\r\n");
            break;   
        }                

        case BUFFER_DATA:
        {
         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
         	UARTputstr("\r\n");

/*
         												//    C1, C2   ,C3
			bCount = sURC.CmdBuffer[_UART_CMD2_]/4;		//58,CMD,Length,D1,D2,D3,D4
			if (bCount > 6) bCount = 6;
			for (i=0;i<bCount;i++)
			{
	            if ((sURC.CmdBuffer[_UART_CMD3_+i*4]==IR_ZERO)||(sURC.CmdBuffer[_UART_CMD3_+i*4]==IR_ONE))
	            {
	            	IR_Transmit_Buffer_Push(sURC.CmdBuffer[_UART_CMD3_+i*4], (sURC.CmdBuffer[_UART_CMD4_+i*4] * 65536 + sURC.CmdBuffer[_UART_CMD5_+i*4] * 256 + sURC.CmdBuffer[_UART_CMD6_+i*4]));
	             }
	            else if (sURC.CmdBuffer[_UART_CMD3_+i*4]==IR_FREQ)
	            {
	            	wFreq = sURC.CmdBuffer[_UART_CMD4_+i*4] * 65536 + sURC.CmdBuffer[_UART_CMD5_+i*4] * 256 + sURC.CmdBuffer[_UART_CMD6_+i*4];
	            	if (wFreq <100)
	            	{
	            		wFreq = 38000;
	            	}
	            	IR_Carrier(wFreq);
	            }
			}
*/

         	if (sURC.CmdBuffer[_UART_CMD2_] > CurrentBuffer[_UART_CMD2_])		// New data is coming
         	{
         		// Can Tx data buffer and capture new coming data into buffer
         		//    C1, C2   ,C3
         		bCount = CurrentBuffer[_UART_CMD3_]/4;		//58,CMD,INDEX,Length,D1,D2,D3,D4
         		if (bCount > 6) bCount = 6;
         		for (i=0;i<bCount;i++)
         		{
         			if ((CurrentBuffer[_UART_CMD4_+i*4]==IR_ZERO)||(CurrentBuffer[_UART_CMD4_+i*4]==IR_ONE))
         			{
         				IR_Transmit_Buffer_Push(CurrentBuffer[_UART_CMD4_+i*4], (CurrentBuffer[_UART_CMD5_+i*4] * 65536 + CurrentBuffer[_UART_CMD6_+i*4] * 256 + CurrentBuffer[_UART_CMD7_+i*4]));
         			}
         			else if (CurrentBuffer[_UART_CMD4_+i*4]==IR_FREQ)
         			{
         				wFreq = CurrentBuffer[_UART_CMD5_+i*4] * 65536 + CurrentBuffer[_UART_CMD6_+i*4] * 256 + CurrentBuffer[_UART_CMD7_+i*4];
         				if (wFreq <100)
         				{
         					wFreq = 38000;
         				}
         				IR_Carrier(wFreq);
         			}
         			else
         			{
         				printfs("Error in BUFFER_DATA\r\n");
         			}
         		}

         		CurrentBuffer[_UART_CMD2_] = sURC.CmdBuffer[_UART_CMD2_];
         		// Fill new data into buffer
         		for (i=_UART_CMD3_; i< bLen; i++)
         		{
         			CurrentBuffer[i] = sURC.CmdBuffer[i];
         		}

         	}
         	else // (Current_Buffer_Data_Index == sURC.CmdBuffer[_UART_CMD2_])
         	{
         		// Only fill current data buffer
         		for (i=_UART_CMD2_; i< bLen; i++)
         		{
         			CurrentBuffer[i] = sURC.CmdBuffer[i];
         		}
         	}

			break;
		}

        case FLUSH_BUFFER_DATA_OR_TRANSMIT_DATA:
        {
         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");

            //Flush remining data
     		bCount = CurrentBuffer[_UART_CMD3_]/4;		//58,CMD,INDEX,Length,D1,D2,D3,D4
     		if (bCount > 6) bCount = 6;
     		for (i=0;i<bCount;i++)
     		{
     			if ((CurrentBuffer[_UART_CMD4_+i*4]==IR_ZERO)||(CurrentBuffer[_UART_CMD4_+i*4]==IR_ONE))
     			{
     				IR_Transmit_Buffer_Push(CurrentBuffer[_UART_CMD4_+i*4], (CurrentBuffer[_UART_CMD5_+i*4] * 65536 + CurrentBuffer[_UART_CMD6_+i*4] * 256 + CurrentBuffer[_UART_CMD7_+i*4]));
     			}
     			else if (CurrentBuffer[_UART_CMD4_+i*4]==IR_FREQ)
     			{
     				wFreq = CurrentBuffer[_UART_CMD5_+i*4] * 65536 + CurrentBuffer[_UART_CMD6_+i*4] * 256 + CurrentBuffer[_UART_CMD7_+i*4];
     				if (wFreq <100)
     				{
     					wFreq = 38000;
     				}
     				IR_Carrier(wFreq);
     			}
     			else
     			{
     				printfs("Error in BUFFER_DATA\r\n");
     			}
     		}
     		CurrentBuffer[_UART_CMD2_]=0;

			break;
		}

        case RECEVING_DATA:
        {
         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");
            if ((sURC.CmdBuffer[_UART_CMD2_]==IR_ZERO)||(sURC.CmdBuffer[_UART_CMD2_]==IR_ONE))
            {
            	IR_Transmit_Buffer_Push(sURC.CmdBuffer[_UART_CMD2_], (sURC.CmdBuffer[_UART_CMD3_] * 65536 + sURC.CmdBuffer[_UART_CMD4_] * 256 + sURC.CmdBuffer[_UART_CMD5_]));
             }
            else if (sURC.CmdBuffer[_UART_CMD2_]==IR_FREQ)
            {
            	wFreq = sURC.CmdBuffer[_UART_CMD3_] * 65536 + sURC.CmdBuffer[_UART_CMD4_] * 256 + sURC.CmdBuffer[_UART_CMD5_];
            	if (wFreq <100)
            	{
            		wFreq = 38000;
            	}
            	IR_Carrier(wFreq);
            }
            else
            {
            	printfs("Error in RECEVING_DATA\r\n");
            }
//            printf("\r\n Stamp %d", sRC_TIMETABLE[bTail].wTimeStamp);
//            printf("\r\n Level %d", sRC_TIMETABLE[bTail].bLevel);
		 	//if(++bTail >= IR_TBL_MAX) bTail = IR_TBL_MAX;

			break;
		}

        case TRANSMIT_DATA:
        {
         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");
            IR_Transmit_Buffer_StartSend();

//
//			bIrTimeIndexIn = bTail-1;
////			else bIrTimeIndexIn = IR_TBL_MAX - 1;
//
//			bIrTimeIndexOut = 0;
//			wFreq = sURC.CmdBuffer[_UART_CMD2_] * 256 + sURC.CmdBuffer[_UART_CMD3_];
//			if (wFreq == 0x00) wFreq = 38000;
//
//            printf("\r\n *****Start Transmit %d",bIrTimeIndexIn );
//			IR_Carrier(wFreq);
//		 	ClearWatchdogTimer();
//			ReplayWave();
//			ClearWatchdogTimer();
			break;
		}

		case REPEAT_DATA:
		{
//         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
//            UARTputstr("\r\n");
//
//			bIrTimeIndexIn = bTail-1;
////			else bIrTimeIndexIn = IR_TBL_MAX - 1;
//			bIrTimeIndexOut = 0;
//
//            printf("\r\n *****Repeat Transmit %d",bIrTimeIndexIn );
//			IR_Carrier(wFreq);
//		 	ClearWatchdogTimer();
//			ReplayWave();
//			ClearWatchdogTimer();
			break;
		}
		case SET_URD_ADDRESS:
		{
         	UART_CheckSum(1, &sURC.CmdBuffer[0], bLen);
            UARTputstr("\r\n");
            Request_RC_RawData_Index = sURC.CmdBuffer[_UART_CMD2_];
            // Mask out the highest bit because it is used for avoid checksum as '$'
            Request_RC_RawData_Index &= 0x7f;
            // END
            Request_RC_RawData_Index = Request_RC_RawData_Index * 0x100 + sURC.CmdBuffer[_UART_CMD3_];
            if (Request_RC_RawData_Index==0)
            {
            	Current_RC_RawData_index = 0;		// clear when starting from 0
            	bIrTimeIndexIn  = 0;
            	bIrTimeIndexOut = 0;
            	IRRear = 0;
            	IRRear_previous = 0;
            }
            Wait_for_URD_Data = TRUE;
//
//			bIrTimeIndexIn = bTail-1;
////			else bIrTimeIndexIn = IR_TBL_MAX - 1;
//			bIrTimeIndexOut = 0;
//
//            printf("\r\n *****Repeat Transmit %d",bIrTimeIndexIn );
//			IR_Carrier(wFreq);
//		 	ClearWatchdogTimer();
//			ReplayWave();
//			ClearWatchdogTimer();
			break;
		}
		default:
            break;              
    }
}

void UART_CheckSum(Byte bNums, Byte *Datapoint, Byte bLen)
{
    Byte i;
    Byte bChecksum;
    
    bChecksum = 0;
    UARTputstr("$ACK"); 
	UARTputchar(bNums);// for pc get one bytes.

    for(i = 0; i < bLen; i++, Datapoint++)
    {
        bChecksum ^= (*Datapoint);
    }
    
    UARTputchar(bChecksum);// for pc get one bytes.
}  

void UART_CheckSum2(Byte bNums, Byte *Datapoint, Byte bLen)
{
    Byte i;
    Byte bChecksum;
    
    bChecksum = 0;
    UARTputstr("$LIT"); 
	UARTputchar(bNums);// for pc get one bytes.

    for(i = 0; i < bLen; i++, Datapoint++)
    {
        bChecksum ^= (*Datapoint);
    }
    
    UARTputchar(bChecksum);// for pc get one bytes.
}

void UART_CheckSum3(Byte bNums, Byte *Datapoint, Byte bLen)
{
    Byte i;
    Byte bChecksum;
    
    bChecksum = 0;
    UARTputstr("$DAT"); 
	UARTputchar(bNums);// for pc get one bytes.

    for(i = 0; i < bLen; i++, Datapoint++)
    {
        bChecksum ^= (*Datapoint);
    }
    
    UARTputchar(bChecksum);// for pc get one bytes.
}
void UART_CheckSum4(Byte bNums, Byte *Datapoint, Byte bLen)
{
    Byte i;
    Byte bChecksum;
    
    bChecksum = 0;
    UARTputstr("$RDY"); 
	UARTputchar(bNums);// for pc get one bytes.

    for(i = 0; i < bLen; i++, Datapoint++)
    {
        bChecksum ^= (*Datapoint);
    }
    
    UARTputchar(bChecksum);// for pc get one bytes.
}
