/**
 * @file USART_PC.h
 * @brief handle USART-PC interactions
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _USART_PC_H_
#define _USART_PC_H_

#include <stdbool.h> 

#define CHAR        0
#define INT         1
#define LONG        2
#define CARR_RETURN 13

#define TX_NEWLINE_PC { USART_transmitByteToPC(0x0d); \
                        USART_transmitByteToPC(0x0a);}

volatile char USART_PC_Received_Data;
volatile char USART_PC_Data_To_Transmit;
volatile bool USART_PC_DATA_RDY;
volatile bool USART_PC_Message_ready;
volatile unsigned char USART_PC_received_playload_len;

//count for checking incoming messages
char message_count;
bool valid_message;

volatile unsigned char USART_PC_ReceivedPacket[20];

void init_USART_PC(void);
unsigned char USART_PC_ReceiveAction(void);
void USART_transmitByteToPC(unsigned char);
void USART_transmitStringToPC(unsigned char*);
void USART_transmitStringToPCFromFlash(char*);

#endif /* _USART_PC_H_ */
