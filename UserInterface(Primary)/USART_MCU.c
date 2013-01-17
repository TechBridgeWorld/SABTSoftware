#include "GlobalsUI.h"

char TempBytes[20];

void init_USART_MCU(void)
{
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x19;
	UBRR0H = 0x00; //set baud rate to 19,200 with 8MHz clock
	UCSR0B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
}

ISR(USART_RX_vect){
	USART_MCU_Received_Data=UDR0;
	USART_MCU_DATA_RDY=true;	
};

void USART_PC_ReceiveAction(void){
	USART_MCU_DATA_RDY=false;
	//USART_transmitByteToMCU(USART_PC_Received_Data);
}

void USART_transmitByteToMCU( unsigned char bData )
{
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0=bData;
}

void USART_transmitStringToMCUFromFlash(char* strData)
{
  while (pgm_read_byte(&(*strData)))
   USART_transmitByteToMCU(pgm_read_byte(&(*strData++)));
}


void USART_transmitStringToMCU(unsigned char* strData)
{
  while (*strData)
   USART_transmitByteToMCU(*strData++);

}
