/**
 * @file USART_MCU.c
 * @brief contains code for the USART->MCU interface
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "GlobalsUI.h"

/**
 * @brief initialize the USART hardware
 *        Baud Rate: 19,200
 *        Clock: 8MHz
 * @return Void
 */
void init_usart_mcu(void)
{
  UCSR0B = 0x00; //disable while setting baud rate
  UCSR0A = 0x00;
  UCSR0C = 0x06;
  UBRR0L = 0x19;
  UBRR0H = 0x00; //set baud rate to 19,200 with 8MHz clock
  UCSR0B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
}

/**
 * @brief interrupt handler for USART0_RX
 * Fires when we receive data from the MCU.
 * @return Void
 */
ISR(_VECTOR(18))
{
  usart_mcu_received_data = UDR0;
  usart_mcu_data_ready = true;
};

/**
 * @brief receive action sent from PC
 * @return Void
 */
void usart_pc_receive_action(void)
{
  usart_mcu_data_ready = false;
}

/**
 * @brief transmit a single byte to the data register
 * @param data the data to transmit
 * @return Void (transmits data)
 */
void usart_transmit_byte_to_mcu(unsigned char data)
{
  // Wait for the data register to be empty by checking for
  // Data Register Empty flag (UDRE0)
  while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = data;
}

/**
 * @brief transmit a string from flash memory
 * @param str_data pointer to the beginnning of the string in flash mem
 * @return Void
 */
void usart_transmit_string_to_mcu_from_flash(char* str_data)
{
  while (pgm_read_byte(&(*str_data)))
  {
    usart_transmit_byte_to_mcu(pgm_read_byte(&(*str_data++)));
  }
}

/**
 * @brief transmit a string to the data register
 * @param strData pointer to the beginning of the string
 * @return Void
 */
void usart_transmit_string_to_mcu(unsigned char* str_data)
{
  while (*str_data)
  {
    usart_transmit_byte_to_mcu(*str_data++);
  }
}
