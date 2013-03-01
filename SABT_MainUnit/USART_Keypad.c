/** 
 * @file USART_Keypad.c
 * @brief Deals with USART communication from the main board - please update 
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 * @author Nick LaGrow (nlagrow)
 */

#include "Globals.h"

bool USART_UI_header_received,USART_UI_length_received;
unsigned char USART_UI_prefix[3];
unsigned char USART_UI_receive_msgcnt;
unsigned char USART_UI_received_playload_len;


/**
 * @brief Initializes the baud communication over USART.
 * @return Void
 */
void init_USART_Keypad(void)
{
  UCSR1B = 0x00; //disable while setting baud rate
  UCSR1A = 0x00;
  UCSR1C = 0x06;
  UBRR1L = 0x19; //set baud rate lo
  UBRR1H = 0x00; //set baud rate hi 19,200 baud with 8MHz clock
  UCSR1B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
  USART_UI_length_received=false;
  USART_UI_header_received=false;
  USART_UI_Message_ready=false;
}


/**
 * @brief   Receives message stored in globabl USART_Keypad_Received_Data
 *          Then proceeds to decode message, use its value, and allow for more 
 *          messages to be sent. USART_UI_prefix - mini shift register which shifts
 *          from 2 -> 1 -> 0
 *          Transfers data from USART_Keypad_Reiceved_Data->USART_UI_ReceivedPacket
 * @ref  tech_report.pdf
 * @return Void
 */
void USART_Keypad_ReceiveAction(void)
{
  USART_Keypad_DATA_RDY = false;
 /* char buf[10]; 
  sprintf(buf, "%d", (int)USART_Keypad_Received_Data);
  USART_transmitStringToPC((unsigned char*)buf);
  TX_NEWLINE_PC;
*/
  // If no header has been found, build it
  if(!USART_UI_header_received)
  {
    USART_UI_prefix[2] = USART_Keypad_Received_Data;
    USART_UI_prefix[0] = USART_UI_prefix[1];
    USART_UI_prefix[1] = USART_UI_prefix[2];
    
    if((USART_UI_prefix[0] == 'U') && (USART_UI_prefix[1] == 'I'))
    {
      USART_UI_header_received = true;
      USART_UI_ReceivedPacket[0] = USART_UI_prefix[0];
      USART_UI_ReceivedPacket[1] = USART_UI_prefix[1];
      USART_UI_receive_msgcnt = 2;
      USART_UI_length_received = false;
      //USART_UI_received_playload_len=USART_Keypad_Received_Data;
      //USART_UI_ReceivedPacket[USART_UI_receive_msgcnt]=USART_Keypad_Received_Data;
      //USART_UI_length_reveived=true;
      //USART_UI_receive_msgcnt++;
    }
  }
  // Get the length of the payload
  else if(!USART_UI_length_received)
  {
    if(USART_UI_receive_msgcnt==2)
    {
      USART_UI_received_playload_len=USART_Keypad_Received_Data;
      USART_UI_ReceivedPacket[USART_UI_receive_msgcnt]=USART_Keypad_Received_Data;
      USART_UI_length_received=true;
      USART_UI_receive_msgcnt++;
    }
    else
    {
      USART_UI_header_received=false;
    }
  }
  // Build the actual message
  else
  {
    USART_UI_ReceivedPacket[USART_UI_receive_msgcnt++] = USART_Keypad_Received_Data;

    // Full message has been received
    if(USART_UI_receive_msgcnt == USART_UI_received_playload_len) 
    {
      USART_UI_Message_ready = true;
      USART_UI_header_received = false;
      USART_UI_length_received = false;
    }
  }
}

/**
 * @brief Transmits byte data from MC --> UI over UDR1
 * @param data - unsigned char, byte to transmit to UI
 * @return Void
 */
void USART_transmitByteToKeypad( unsigned char data )
{
  while ( !(UCSR1A & (1<<UDRE1)) )
    ;                       /* Wait for empty transmit buffer */
  UDR1 = data;               /* Start transmition */
}

/**
 * @brief Transmits string data from MC Flash --> UI over UDR1
 * @param string - char*, String to transmit to UI
 * @return Void
 */
void USART_transmitStringToKeypadFromFlash(char* string)
{
  while (pgm_read_byte(&(*string)))
   USART_transmitByteToKeypad(pgm_read_byte(&(*string++)));
}

/**
 * @brief Transmits string data from MC --> UI over UDR1
 * @param string - char*, String to transmit to UI
 * @return Void
 */
void USART_transmitStringToKeypad(unsigned char* string)
{
  while (*string)
   USART_transmitByteToKeypad(*string++);
}
