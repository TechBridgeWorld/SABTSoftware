/**
 * @file script_common.c
 * @brief Code for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "alphabet.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Finds an alphabet from a specified script based on a bit pattern
 * @param char bit_pattern - Bit pattern to look for
 * @param script_t* this_script - Script to look in
 * @return alphabet_t* - Corresponding to alphabet it found, NULL if not found
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
 * @param char* sound - Phonetic sound string
 * @param script_t* this_script - Script to look in
 * @return alphabet_t* - corresponding to alphabet it found, NULL if not found
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
 * @param alphabet_t* a1 - First alphabet
 * @param alphabet_t* a2 - Second alphabet
 * @return bool - true if same, false otherwise
 */
bool is_same_alphabet(alphabet_t* a1, alphabet_t* a2) {
	if (a1 != NULL && a2 != NULL) {
		return (a1->bit_pattern == a2->bit_pattern);
	} else {
		return false;
	}
}
