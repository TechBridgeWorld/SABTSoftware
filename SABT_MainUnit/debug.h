/**
* @file debug.h
* @brief Constants and macros for PC based debugging
* @author Vivek Nair (viveknair@cmu.edu)
*/

// SENDBYTE can be used to send a character
// NEWLINE does what you think it's supposed to

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "usart_pc.h"

#define SENDBYTE(msg) \
  usart_transmit_byte_to_pc((unsigned char)msg)

#define NEWLINE	\
  log_msg("\n\r");

char dbgstr[64];

#endif /* _DEBUG_H_ */