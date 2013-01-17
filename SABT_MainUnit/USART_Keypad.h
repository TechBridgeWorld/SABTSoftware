#ifndef _USART_KEYPAD_H_
#define _USART_KEYPAD_H_

#include <stdbool.h> 

#define CHAR 0
#define INT  1
#define LONG 2

#define TX_NEWLINE_KP {USART_transmitByteToKeypad(0x0d); USART_transmitByteToKeypad(0x0a);}

volatile char USART_Keypad_Received_Data;
volatile char USART_Keypad_Data_To_Transmit;
volatile bool USART_Keypad_DATA_RDY;
volatile bool USART_UI_Message_ready;

volatile unsigned char USART_UI_ReceivedPacket[20];

void init_USART_Keypad(void);
void USART_Keypad_ReceiveAction(void);
void USART_transmitByteToKeypad(unsigned char);
void USART_transmitStringToKeypad(unsigned char*);
void USART_transmitStringToKeypadFromFlash(char*);

#endif
