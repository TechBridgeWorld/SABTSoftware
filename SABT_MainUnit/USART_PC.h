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

#define TX_NEWLINE_PC { usart_transmit_byte_to_pc(0x0d); \
                        usart_transmit_byte_to_pc(0x0a);}

volatile char usart_pc_received_data;
volatile char usart_pc_data_to_transmit;
volatile bool usart_pc_data_ready;
volatile bool usart_pc_message_ready;
volatile unsigned char usart_pc_received_payload_len;

//count for checking incoming messages
char message_count;
bool valid_message;

volatile unsigned char usart_pc_received_packet[20];

void init_usart_pc(void);
unsigned char usart_pc_receive_action(void);
void usart_transmit_byte_to_pc(unsigned char);
void usart_transmit_string_to_pc(unsigned char*);
void usart_transmit_string_to_pc_from_flash(char*);

#endif /* _USART_PC_H_ */
