/**
 * @file USART_PC.c
 * @brief Deals with USART communication with main board - please update
 * @author Alex Etling (petling)
 * @author Nick LaGrow (nlagrow)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

bool USART_PC_header_received,USART_PC_length_reveived;
unsigned char USART_PC_prefix[3];
unsigned char USART_PC_receive_msgcnt;

/**
 * @brief Initializes the buad communication over USART.
 * @return Void
 */
void init_USART_PC(void)
{
  UCSR0B = 0x00; //disable while setting baud rate
  UCSR0A = 0x00;
  UCSR0C = 0x06;
  UBRR0L = 0x19;
  UBRR0H = 0x00; //set baud rate to 19,200 with 8MHz clock
  UCSR0B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
}

/**
 * @brief   Receives message stored in global USART_PC_Received_Data
 *          Then proceeds to decode message, use its value, and allow for more
 *          message to be processed by PC_parse_message()
 * @ref  tech_report.pdf
 * @return always 0?
 */
unsigned char USART_PC_ReceiveAction(void)
{
  USART_PC_DATA_RDY=false;
  message_count ++;

  // Received an entire line; process it
  if(USART_PC_Received_Data == CARR_RETURN)
  {
    message_count = 0;
    if(!valid_message)
    {
      valid_message = true;
      PRINTF((unsigned char *)"SABT - IMPROPER HEADER TYPE, MUST USE PC!\r\n");
    }
  }
  
  // if header not yet received, build it
  if(!USART_PC_header_received)
  {
    USART_PC_prefix[2]=USART_PC_Received_Data;
    USART_PC_prefix[0]=USART_PC_prefix[1];
    USART_PC_prefix[1]=USART_PC_prefix[2];

    if((USART_PC_prefix[0]=='P')&&(USART_PC_prefix[1]=='C') && (message_count == 2))
    {
      USART_PC_header_received=true;
      USART_PC_ReceivedPacket[0]=USART_PC_prefix[0];
      USART_PC_ReceivedPacket[1]=USART_PC_prefix[1];
      USART_PC_receive_msgcnt=2;
      //USART_PC_length_reveived=false;
      //USART_PC_received_playload_len=USART_PC_Received_Data;
      //USART_PC_ReceivedPacket[USART_PC_receive_msgcnt]=USART_PC_Received_Data;
      //USART_PC_length_reveived=true;
      //USART_PC_receive_msgcnt++;
    }
    else if(((USART_PC_prefix[0]!='P') || (USART_PC_prefix[1]!='C')) &&
            (message_count == 2))
    {
      valid_message = false;
    }
  }
  else{
    if(USART_PC_Received_Data==CARR_RETURN) //If carraige return found --> end of the command
    {
      USART_PC_received_playload_len=USART_PC_receive_msgcnt;
      USART_PC_Message_ready=true;
      USART_PC_header_received=false;
      //USART_PC_length_reveived=false;
    }
    USART_PC_ReceivedPacket[USART_PC_receive_msgcnt++]=USART_PC_Received_Data;
  }

/*
  else if(!USART_PC_length_reveived)
  {    
    if(USART_PC_receive_msgcnt==2)
    {
      USART_PC_received_playload_len=USART_PC_Received_Data;
      if(USART_PC_received_playload_len=='x') //If this is 'z', then this is the init port detect message >> Proceed with a port reply.
      {
        USART_PC_Message_ready=false;
        USART_PC_header_received=false;
        USART_PC_length_reveived=false;
        return 'z';
      }
      USART_PC_ReceivedPacket[USART_PC_receive_msgcnt]=USART_PC_Received_Data;
      USART_PC_length_reveived=true;
      USART_PC_receive_msgcnt++;
    }
    else
    {
      USART_PC_header_received=false;
    }
  }
  else
  {
    USART_PC_ReceivedPacket[USART_PC_receive_msgcnt++]=USART_PC_Received_Data;
    if(USART_PC_receive_msgcnt==USART_PC_received_playload_len) //full message has been received
    {
      USART_PC_Message_ready=true;
      USART_PC_header_received=false;
      USART_PC_length_reveived=false;
    }
  }    
*/
  return 0;
}

/**
 * @brief transmit one byte to UDR0 (PC connection) 
 * @param bData contains the byte that needs to be sent
 * return Void
 */
void USART_transmitByteToPC( unsigned char bData )
{
///*  Disabled PC TX temp
  while ( !(UCSR0A & (1<<UDRE0)) ); // Loop until the data register is empty
  UDR0 = bData;                     // Transmit one byte of data
//  */
}

/** 
 * @brief reads each byte of data and sends it to the Flash individually
 * @param strData   String     Contains message to be sent to PC
 * @return Void
 */
void USART_transmitStringToPCFromFlash(char* strData)
{
  while (pgm_read_byte(&(*strData)))
   USART_transmitByteToPC(pgm_read_byte(&(*strData++)));
}

/**
 * @brief transmit a string to UDR0 (PC connection)
 * @param strData string to transmit
 * @return Void
 * TODO: does this method send null terminator?
 */
void USART_transmitStringToPC(unsigned char* strData)
{
  while (*strData)
   USART_transmitByteToPC(*strData++);
}
