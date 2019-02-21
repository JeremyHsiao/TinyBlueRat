
/***************************************************************************
* INCLUDE FILES
***************************************************************************/
/*------------------------------------*/
/* Include files					  */
/*------------------------------------*/

#include "chip.h"
#include "board.h"
#include "gpio.h"
#include "string.h"
#include "i2c.h"
#include "Timer.h"
#include "ExtEEPROM.h"

// Common
#include "Define.h"
// Internal
#define  GLOBAL
#undef GLOBAL
// External
#define  GLOBAL extern
#include "app_Uart.h"
#undef GLOBAL

/*------------------------------------*/
/* Local data or definition 		  */
/*------------------------------------*/

#define ONEBYTE_WRITE_BUF_SIZE	(2+1)
void WriteExtEEPROM_OneByte ( uint16_t Address, uint8_t write_data )
{
	uint8_t 			I2CBuffer[ONEBYTE_WRITE_BUF_SIZE];
	int8_t				retry_cnt = I2C_RETRY_TIME;
	int					ret;

	I2CBuffer[0] = (uint8_t)(Address >> 8);
	I2CBuffer[1] = (uint8_t) Address;
	I2CBuffer[2] = write_data;
	ret = Chip_I2C_MasterSend(I2C_INTERFACE_ID, (M24C32_EPROM_SLAVE_ADDR>>1), I2CBuffer, ONEBYTE_WRITE_BUF_SIZE);  // return 0 if all written

	while ((ret>0)&&(retry_cnt>0))
	{
		Delayus(I2C_RETRY_WAIT_MS*1000);
		ret = Chip_I2C_MasterSend(I2C_INTERFACE_ID, (M24C32_EPROM_SLAVE_ADDR>>1), I2CBuffer, ONEBYTE_WRITE_BUF_SIZE);
		retry_cnt--;;
	}

	if (ret>0)
	{
		return;
		// trip error here.
	}
}

#define PAGE_WRITE_BUF_SIZE		(M24C32_EPROM_PAGE_SIZE+2)
void WriteExtEEPROM_32_64_Page_Write ( uint16_t Address, uint8_t *data_ptr, uint8_t data_length )
{
	uint8_t 			I2CBuffer[PAGE_WRITE_BUF_SIZE];
	int8_t				retry_cnt = I2C_RETRY_TIME;
	int					ret;

	I2CBuffer[0] = (uint8_t)(Address >> 8);
	I2CBuffer[1] = (uint8_t) Address;
	memcpy((I2CBuffer+2),data_ptr,data_length);
	ret = Chip_I2C_MasterSend(I2C_INTERFACE_ID, (M24C32_EPROM_SLAVE_ADDR>>1), I2CBuffer, (data_length+2));  // return 0 if all written

	while ((ret>0)&&(retry_cnt>0))
	{
		Delayus(I2C_RETRY_WAIT_MS*1000);
		ret = Chip_I2C_MasterSend(I2C_INTERFACE_ID, (M24C32_EPROM_SLAVE_ADDR>>1), I2CBuffer, (data_length+2));
		retry_cnt--;
	}

	if (ret>0)
	{
		return;
		// trip error here.
	}
}

void WriteExtEEPROM_MultiByte ( uint16_t Address, uint8_t *write_data, uint8_t MaxCounter )
{
	uint16_t	Current_addr;
	uint8_t		*data_ptr;
	uint32_t	temp, remaining_length;

#ifdef M24C32_PAGE_WRITE

	Current_addr = Address;
	remaining_length = MaxCounter;
	data_ptr = write_data;

	// Segment 1: data before a complete page
	temp = Current_addr % M24C32_EPROM_PAGE_SIZE;
	if(temp!=0)
	{
		temp = M24C32_EPROM_PAGE_SIZE-temp;				// remaining_byte_cnt before page boundary

		if(temp>remaining_length)					// more than data to be sent, set to length of data
		{
			temp = remaining_length;
		}
		WriteExtEEPROM_32_64_Page_Write( Current_addr, data_ptr , temp );
		data_ptr += temp;
		Current_addr += temp;
		remaining_length -= temp;
	}

	// Segment 2: full-page data sending at page boundary
	while (remaining_length>=M24C32_EPROM_PAGE_SIZE)
	{
		temp = M24C32_EPROM_PAGE_SIZE;						// full-page
		WriteExtEEPROM_32_64_Page_Write( Current_addr, data_ptr , temp );
		data_ptr += temp;
		Current_addr += temp;
		remaining_length -= temp;
	}

	// Segment 3: remaining non-full-page data sending at page boundary
	if (remaining_length> 0)
	{
		temp = remaining_length;
		WriteExtEEPROM_32_64_Page_Write( Current_addr, data_ptr , temp );
//		data_ptr += temp;
//		Current_addr += temp;
//		remaining_length -= temp;
	}


#else
    data_ptr = write_data;
	for ( Counter=0; Counter<MaxCounter; Counter++)
	{
		WriteExtEEPROM_OneByte(Address,*data_ptr);
		Address++;
		data_ptr++;
	}
#endif
}

void ReadExtEEPROM ( uint16_t Address, uint8_t *read_data, uint8_t Rcounter )
{
	uint8_t 			I2CBuffer[2];	// Write Address first (2byte)
	int8_t				retry_cnt = I2C_RETRY_TIME;
	int					ret;

	I2CBuffer[0] = (uint8_t)(Address >> 8);
	I2CBuffer[1] = (uint8_t) Address;

	ret = Chip_I2C_MasterCmd2ByteRead(I2C_INTERFACE_ID, (M24C32_EPROM_SLAVE_ADDR>>1), I2CBuffer, read_data, Rcounter);

	//
	// If none or only part of data read, retry unread part of data
	//
	while ((ret<Rcounter)&&(retry_cnt>0))
	{
		Delayus(I2C_RETRY_WAIT_MS*1000);
		if(ret>0)
		{
			Address+=ret;
			read_data+=ret;
			Rcounter-=ret;
			I2CBuffer[0] = (uint8_t)(Address >> 8);
			I2CBuffer[1] = (uint8_t) Address;
		}
		ret = Chip_I2C_MasterCmd2ByteRead(I2C_INTERFACE_ID, (M24C32_EPROM_SLAVE_ADDR>>1), I2CBuffer, read_data, Rcounter);
		retry_cnt--;
	}

	if (ret!=Rcounter)
	{

		UARTputHEX_U((Address>>8)&0xff);
		UARTputHEX_U(Address&0xff);
		UARTputchar('\r');
		UARTputchar('\n');

		return;
		// trip error here.
	}
}

// Check if data is different before write
void StoreExtEEPROM(uint16_t wAddr, uint8_t bData)
{
	uint8_t		temp_buffer[1];

	ReadExtEEPROM(wAddr, temp_buffer, 1);
	if (temp_buffer[0] != bData)
    {
		WriteExtEEPROM_OneByte(wAddr, bData);
		ReadExtEEPROM(wAddr, temp_buffer, 1);
		if (temp_buffer[0] != bData)
		{
			return;
			// trip error here.
		}
    }
}
