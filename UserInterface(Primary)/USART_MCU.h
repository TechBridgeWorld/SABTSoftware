/**
 * @file USART_MCU.h
 * @brief definitions for the USART->MCU interface
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _USART_MCU_H_
#define _USART_MCU_H_

volatile char usart_mcu_received_data;
volatile bool usart_mcu_data_ready;
volatile bool transmit_complete;

void init_usart_mcu(void);
void usart_transmit_byte_to_mcu(unsigned char data);
void usart_transmit_string_to_mcu_from_flash(char* str_data);
void usart_transmit_string_to_mcu(unsigned char* str_data);

#endif /* _USART_MCU_H_ */
