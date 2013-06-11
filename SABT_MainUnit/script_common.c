/**
 * @file script_common.c
 * @brief Declarations for common script wrapper functions
 * @author Vivek Nair (viveknai)
 */

#include "alphabet.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Finds an alphabet from a specified script based on a bit pattern
 * @param char bit_pattern, script_t* this_script
 * @return alphabet_t* corresponding to alphabet it found, NULL if not found
 */
alphabet_t* get_alphabet_by_bits(char bit_pattern, script_t* this_script) {
	alphabet_t* this_alphabet = NULL;
	alphabet_t* script_alphabets = this_script->alphabets;
	short script_length = this_script->length;
	for (int i = 0; i < script_length; i++) {
		this_alphabet = &script_alphabets[i];
		if (this_alphabet->bit_pattern == bit_pattern) {
			return this_alphabet;
		}
	}
	return NULL;
}

/**
 * @brief Finds an alphabet from a specified script based on sound
 * @param char* sound, script_t* this_script
 * @return alphabet_t* corresponding to alphabet it found, NULL if not found
 */
alphabet_t* get_alphabet_by_sound(char* sound, script_t* this_script) {
	alphabet_t* this_alphabet = NULL;
	alphabet_t* script_alphabets = this_script->alphabets;
	short script_length = this_script->length;
	for (int i = 0; i < script_length; i++) {
		this_alphabet = &script_alphabets[i];
		if (strcmp(this_alphabet->sound, sound) == 0) {
			return this_alphabet;
		}
	}
	return NULL;
}

/**
 * @brief Checks if 2 alphabets are the same
 * @param alphabet_t* a1, a2
 * @return bool
 */
bool is_same_alphabet(alphabet_t* a1, alphabet_t* a2) {
	if (a1 != NULL && a2 != NULL) {
		return (a1->bit_pattern == a2->bit_pattern);
	} else {
		return false;
	}
}
