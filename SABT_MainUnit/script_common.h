/**
 * @file script_common.h
 * @brief Definitions for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_COMMON_H_
#define _SCRIPT_COMMON_H_ 

#include "alphabet.h"
#include <stdbool.h>

/* Reserved script bit patterns */


alphabet_t* get_alphabet_by_bits(char bit_pattern, script_t* this_script);
alphabet_t* get_alphabet_by_sound(char* sound, script_t* this_script);
bool is_same_alphabet(alphabet_t* a1, alphabet_t* a2);

#endif /* _SCRIPT_COMMON_H_ */
