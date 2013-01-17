#include "Globals.h"

bool USART_PC_header_received,USART_PC_length_reveived;
unsigned char USART_PC_prefix[3];
unsigned char USART_PC_receive_msgcnt;


void init_USART_PC(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x19;
 UBRR0H = 0x00; //set baud rate to 19,200 with 8MHz clock
 UCSR0B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
}

unsigned char USART_PC_ReceiveAction(void){
	USART_PC_DATA_RDY=false;

	if(!USART_PC_header_received)
	{
		USART_PC_prefix[2]=USART_PC_Received_Data;
		USART_PC_prefix[0]=USART_PC_prefix[1];
		USART_PC_prefix[1]=USART_PC_prefix[2];
		if((USART_PC_prefix[0]=='P')&&(USART_PC_prefix[1]=='C'))
		{
			USART_PC_header_received=true;
			USART_PC_ReceivedPacket[0]=USART_PC_prefix[0];
			USART_PC_ReceivedPacket[1]=USART_PC_prefix[1];
			USART_PC_receive_msgcnt=2;
			//USART_PC_length_reveived=false;
			//USART_PC_received_playload_len=USART_PC_Received_Data;
			//USART_PC_ReceivedPacket[USART_PC_receive_msgcnt]=USART_PC_Received_Data;
			//USART_PC_length_reveived=true;
			//USART_PC_receive_msgcnt++;
		}
	}
	else{
		if(USART_PC_Received_Data==13) //If carraige return found --> end of the command
		{
			USART_PC_received_playload_len=USART_PC_receive_msgcnt;
			USART_PC_Message_ready=true;
			USART_PC_header_received=false;
			//USART_PC_length_reveived=false;
		}
		USART_PC_ReceivedPacket[USART_PC_receive_msgcnt++]=USART_PC_Received_Data;
	}

/*
	else if(!USART_PC_length_reveived)
	{		
		if(USART_PC_receive_msgcnt==2)
		{
			USART_PC_received_playload_len=USART_PC_Received_Data;
			if(USART_PC_received_playload_len=='x') //If this is 'z', then this is the init port detect message >> Proceed with a port reply.
			{
				USART_PC_Message_ready=false;
				USART_PC_header_received=false;
				USART_PC_length_reveived=false;
				return 'z';
			}
			USART_PC_ReceivedPacket[USART_PC_receive_msgcnt]=USART_PC_Received_Data;
			USART_PC_length_reveived=true;
			USART_PC_receive_msgcnt++;
		}
		else
		{
			USART_PC_header_received=false;
		}
	}
	else
	{
		USART_PC_ReceivedPacket[USART_PC_receive_msgcnt++]=USART_PC_Received_Data;
		if(USART_PC_receive_msgcnt==USART_PC_received_playload_len) //full message has been received
		{
			USART_PC_Message_ready=true;
			USART_PC_header_received=false;
			USART_PC_length_reveived=false;
		}
	}		
*/
	return 0;
}

void USART_transmitByteToPC( unsigned char bData )
{
///*	Disabled PC TX temp
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0=bData;
//	*/
}

void USART_transmitStringToPCFromFlash(char* strData)
{
  while (pgm_read_byte(&(*strData)))
   USART_transmitByteToPC(pgm_read_byte(&(*strData++)));
}


void USART_transmitStringToPC(unsigned char* strData)
{
  while (*strData)
   USART_transmitByteToPC(*strData++);
}
