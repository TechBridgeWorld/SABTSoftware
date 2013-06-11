/**
 * @file script_common.h
 * @brief Definitions for common script wrapper functions
 * @author Vivek Nair (viveknai)
 */

#ifndef _SCRIPT_COMMON_H_
#define _SCRIPT_COMMON_H_ 

#include "alphabet.h"
#include <stdbool.h>

/* Reserved script bit patterns */
#define ENTER			0b11111111
#define CANCEL		0b11111110
#define LEFT			0b11111101
#define RIGHT			0b11111100
#define VOL_UP		0b11111011
#define VOL_DOWN	0b11111010

alphabet_t* get_alphabet_by_bits(char bit_pattern, script_t* this_script);
alphabet_t* get_alphabet_by_sound(char* sound, script_t* this_script);
bool is_same_alphabet(alphabet_t* a1, alphabet_t* a2);

#endif /* _SCRIPT_COMMON_H_ */
