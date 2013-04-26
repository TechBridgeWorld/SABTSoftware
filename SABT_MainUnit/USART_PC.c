/**
 * @file USART_PC.c
 * @brief Deals with USART communication with main board - please update
 * @author Alex Etling (petling)
 * @author Nick LaGrow (nlagrow)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

bool usart_pc_header_received, usart_pc_length_received;
int usart_received_payload_len;
unsigned char usart_pc_prefix[3];
unsigned char usart_pc_receive_msgcnt;

/**
 * @brief Initializes the buad communication over USART.
 * @return Void
 */
void init_usart_pc(void)
{
  UCSR0B = 0x00; //disable while setting baud rate
  UCSR0A = 0x00;
  UCSR0C = 0x06;
  UBRR0L = 0x19;
  UBRR0H = 0x00; //set baud rate to 19,200 with 8MHz clock
  UCSR0B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
}

/**
 * @brief   Receives message stored in global usart_pc_received_data
 *          Then proceeds to decode message, use its value, and allow for more
 *          message to be processed by PC_parse_message()
 * @ref  tech_report.pdf
 * @return always 0?
 */
unsigned char usart_pc_receive_action(void)
{
  usart_pc_data_ready = false;

  message_count++;

  // Received an entire line; process it
  if(usart_pc_received_data == CARR_RETURN)
  {
    message_count = 0;
    if(!valid_message)
    {
      valid_message = true;
      PRINTF((unsigned char *)"SABT - IMPROPER HEADER TYPE, MUST USE PC!\r\n");
    }
  }

  // if header not yet received, build it
  if(!usart_pc_header_received)
  {
    usart_pc_prefix[2] = usart_pc_received_data;
    usart_pc_prefix[0] = usart_pc_prefix[1];
    usart_pc_prefix[1] = usart_pc_prefix[2];

    if((usart_pc_prefix[0]=='P') && (usart_pc_prefix[1]=='C') && (message_count == 2))
    {
      usart_pc_header_received = true;
      usart_pc_received_packet[0] = usart_pc_prefix[0];
      usart_pc_received_packet[1] = usart_pc_prefix[1];
      usart_pc_receive_msgcnt = 2;
    }
    else if(((usart_pc_prefix[0] != 'P') || (usart_pc_prefix[1] != 'C')) &&
        (message_count == 2))
    {
      valid_message = false;
    }
  }
  else
  {
    // If carraige return found --> end of the command
    if(usart_pc_received_data == CARR_RETURN)
    {
      usart_received_payload_len = usart_pc_receive_msgcnt;
      usart_pc_message_ready = true;
      usart_pc_header_received = false;
    }
    usart_pc_received_packet[usart_pc_receive_msgcnt++] = usart_pc_received_data;
  }

  return 0;
}

/**
 * @brief transmit one byte to UDR0 (PC connection) 
 * @param data contains the byte that needs to be sent
 * return Void
 */
void usart_transmit_byte_to_pc(unsigned char data)
{
  while (!(UCSR0A & (1<<UDRE0)));   // Loop until the data register is empty
  UDR0 = data;                      // Transmit one byte of data
}

/** 
 * @brief reads each byte of data and sends it to the Flash individually
 * @param str_data   String     Contains message to be sent to PC
 * @return Void
 */
void usart_transmit_string_to_pc_from_flash(char* str_data)
{
  while (pgm_read_byte(&(*str_data)))
    usart_transmit_byte_to_pc(pgm_read_byte(&(*str_data++)));
}

/**
 * @brief transmit a string to UDR0 (PC connection)
 * @param str_data string to transmit
 * @return Void
 * TODO: does this method send null terminator?
 */
void usart_transmit_string_to_pc(unsigned char* str_data)
{
  while (*str_data)
    usart_transmit_byte_to_pc(*str_data++);
}
