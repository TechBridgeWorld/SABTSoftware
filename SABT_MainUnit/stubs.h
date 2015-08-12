/**
* @file debug.h
* @brief Constants and macros for PC based debugging
* @author Vivek Nair (viveknair@cmu.edu)
*/

// SENDBYTE can be used to send a character
// NEWLINE does what you think it's supposed to

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef DEBUGMODE

void pc_requests_to_modify_modes_file(void);
void pc_parse_message(void);
void pc_control_key_pressed(void);

void init_usart_pc(void);
unsigned char usart_pc_receive_action(void);
void usart_transmit_byte_to_pc(unsigned char foo);
void usart_transmit_string_to_pc(unsigned char* foo);
void usart_transmit_string_to_pc_from_flash(const char* foo);

#endif
#endif /* _DEBUG_H_ */