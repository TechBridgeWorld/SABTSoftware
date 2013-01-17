#ifndef _USART_MCU_H_
#define _USART_MCU_H_

volatile char USART_MCU_Received_Data;
volatile bool USART_MCU_DATA_RDY;

void init_USART_MCU(void);
void USART_transmitByteToMCU( unsigned char bData );
void USART_transmitStringToMCUFromFlash(char* strData);
void USART_transmitStringToMCU(unsigned char* strData);

#endif

