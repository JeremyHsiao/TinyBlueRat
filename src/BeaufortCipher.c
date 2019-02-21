/*
 * BeaufortCipher.c
 *
 *  Created on: 2014/2/14
 *      Author: jeremy.hsiao
 */

#include "chip.h"
#include "board.h"
#include "string.h"

// Shouldn't be larger than (>) 256 with current implementation
#define LetterSetSize	26

uint8_t Distance_Function(uint8_t dist, uint8_t src)
{
	uint8_t	ret_value;

	if (src>dist)
	{
		ret_value = LetterSetSize - src + dist;
	}
	else
	{
		ret_value = dist - src;
	}
	return ret_value;
}

void TestBeaufortCipher()
{
	char 		message_content[] = "DEFENDTHEEASTWALLOFTHECASTLE";
	char 		beaufort_cipher_key[] = "FORTIFICATION";

	uint8_t		cipher_output_buf[sizeof(message_content)];
	uint8_t		decode_output_buf[sizeof(message_content)];
	uint32_t	temp_index;

	// Encryption
	for (temp_index=0; temp_index<sizeof(message_content)-1; temp_index++ )
	{
		cipher_output_buf[temp_index] = Distance_Function((beaufort_cipher_key[temp_index%(sizeof(beaufort_cipher_key)-1)]-'A'),(message_content[temp_index]-'A')) + 'A';
	}
	cipher_output_buf[sizeof(message_content)-1]=0x00;

	// Decryption
	for (temp_index=0; temp_index<sizeof(message_content)-1; temp_index++ )
	{
		decode_output_buf[temp_index] = Distance_Function((beaufort_cipher_key[temp_index%(sizeof(beaufort_cipher_key)-1)]-'A'),(cipher_output_buf[temp_index]-'A')) + 'A';
	}
	decode_output_buf[sizeof(message_content)-1]=0x00;

	temp_index =2;
}
