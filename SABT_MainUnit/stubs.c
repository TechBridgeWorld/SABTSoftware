#include "io.h"

#ifdef DEBUGMODE

void pc_requests_to_modify_modes_file(void){}

void pc_parse_message(void){}

void pc_control_key_pressed(void){}

void init_usart_pc(void){}

unsigned char usart_pc_receive_action(void){
	log_msg("Received action.");
	return 0;
}

void usart_transmit_byte_to_pc(unsigned char foo){
	log_msg((const char*) &foo);
}

void usart_transmit_string_to_pc(unsigned char* foo){
	log_msg((const char*) foo);
}

void usart_transmit_string_to_pc_from_flash(const char* foo){
	log_msg(foo);
}

#endif