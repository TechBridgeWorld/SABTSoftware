/**
 * @file USART_Keypad.h
 * @brief handle USART-UI interactions
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _USART_KEYPAD_H_
#define _USART_KEYPAD_H_

#include <stdbool.h> 

#define CHAR 0
#define INT  1
#define LONG 2

#define TX_NEWLINE_KP { USART_transmitByteToKeypad(0x0d); \
                        USART_transmitByteToKeypad(0x0a);}

volatile char usart_keypad_received_data;
volatile char usart_keypad_data_to_transmit;
volatile bool usart_keypad_data_ready;
volatile bool usart_ui_message_ready;

volatile unsigned char usart_ui_received_packet[20];

void init_usart_keypad(void);
void usart_keypad_receive_action(void);
void usart_transmit_byte_to_keypad(unsigned char);
void usart_transmit_string_to_keypad(unsigned char*);
void usart_transmit_string_to_keypad_from_flash(char*);

#endif /* _USART_KEYPAD_H_ */
