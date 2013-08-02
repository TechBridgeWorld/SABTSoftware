/**
* @file debug.h
* @brief Constants and macros for PC based debugging
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Use dbgstr to construct a message using sprintf, then send stuff with PRINTF
// SENDBYTE can be used to send a character
// NEWLINE does what you think it's supposed to

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "usart_pc.h"

#define PRINTF(msg) \
  usart_transmit_string_to_pc((unsigned char*)msg)

#define SENDBYTE(msg) \
  usart_transmit_byte_to_pc((unsigned char)msg)

#define NEWLINE	PRINTF("\n\r");

char dbgstr[64];

#endif /* _DEBUG_H_ */