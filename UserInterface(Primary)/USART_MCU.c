/**
 * @file USART_MCU.c
 * @brief contains code for the USART->MCU interface
 * @author Nick LaGrow (nlagrow)
 */

#include "GlobalsUI.h"

char TempBytes[20];

/**
 * @brief initialize the USART hardware
 * Baud Rate: 19,200
 * Clock: 8MHz
 * @return Void
 */
void init_USART_MCU(void)
{
	UCSR0B = 0x00; //disable while setting baud rate
	UCSR0A = 0x00;
	UCSR0C = 0x06;
	UBRR0L = 0x19;
	UBRR0H = 0x00; //set baud rate to 19,200 with 8MHz clock
	UCSR0B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
}

// TODO: what does this do?
ISR(USART_RX_vect){
	USART_MCU_Received_Data=UDR0;
	USART_MCU_DATA_RDY=true;	
};

/**
 * @brief receive action sent from PC
 * TODO: why is line commented out / how does it work
 * @return Void
 */
void USART_PC_ReceiveAction(void){
	USART_MCU_DATA_RDY=false;
	//USART_transmitByteToMCU(USART_PC_Received_Data);
}

/**
 * @brief transmit a single byte to the data register
 * @param bData the data to transmit
 * UCSR0A : register
 * UDRE0 : USART Data register empty (bit 5 of UCSR0A
 * UDR0 : Data register
 * @return Void (transmits data)
 */
void USART_transmitByteToMCU( unsigned char bData )
{
	while ( !(UCSR0A & (1<<UDRE0)) );
	UDR0 = bData;
}

/**
 * @brief transmit a string from flash memory
 * @param strData pointer to the beginnning of the string in flash mem
 * @return Void
 */
void USART_transmitStringToMCUFromFlash(char* strData)
{
  while (pgm_read_byte(&(*strData)))
   USART_transmitByteToMCU(pgm_read_byte(&(*strData++)));
}

/**
 * @brief transmit a string to the data register
 * @param strData pointer to the beginning of the string
 * @return Void
 */
void USART_transmitStringToMCU(unsigned char* strData)
{
  while (*strData)
   USART_transmitByteToMCU(*strData++);
}
