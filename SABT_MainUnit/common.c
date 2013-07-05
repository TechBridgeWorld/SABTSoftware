/**
 * @file common.c
 * @brief Definitions for some common library functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "globals.h"
#include "audio.h"
#include "common.h"
#include "script_common.h"
#include "glyph.h"

#include <stdbool.h>

/*
* @brief Sets script globals for a new language
* @param script_t* - Pointer to new script
* @param char* - Pointer to mode fileset string
* @return void
*/
void set_mode_globals(script_t* new_script, char* new_lang_fileset,
	char* new_mode_fileset) {
	lang_script = new_script;
	lang_fileset = new_lang_fileset;
	mode_fileset = new_mode_fileset;
}

/**
 * @brief Generates a pseudo-random integer based on the system timer
 * @param void
 * @return int - Pseudo-random value
 */
int timer_rand(void) {
	return TCNT1 * 53;
}

/**
 * @brief Adds a dot to a bit pattern representing a cell
 * @param char bits - Old bit pattern
 * @param char dot - Dot to add
 * @return char - New bit pattern
 */
char add_dot(char bits, char dot) {
	return (bits | 1 << (CHARTOINT(dot) - 1));
}

/**
 * @brief Quits the current mode and returns to the main menu
 * @param void
 * @return void
 */
void quit_mode(void) {
	ui_is_mode_selected = false;
  ui_current_mode_index = 0;
  ui_current_mode_number = ui_modes[ui_current_mode_index];
  play_mp3("","MM");
}

/*
* @brief Returns 10^n
* @param int n - Power
* @return int - 10^n
*/
int ten_to_the(int n) {
	int res = 1;
	if (n < 0) {
		return 0;
	} else {
		while (n > 0) {
			res *= 10;
			n--;
		}
		return res;
	}
}

/*
* @brief Gets number of digits
* @param int - Number to determine number of digits for
* @return int - Number of digits
*/
int get_num_of_digits(int number) {
	if (number < 0) {
		number = -number;
	}
	int digits = 0;
	while (number != 0) {
		number = number / 10;
		digits++;
	}
	return digits;
}

/**
 * @brief  Given a char, in last_cell, play the corresponding number
 *         sound file
 * @return Void
 */

void play_requested_dot(char play_dot)
{
  // This will hold formatted file to access
  char req_mp3[10];
  sprintf((char*)req_mp3, "dot_%c", play_dot);
  play_mp3(NULL,req_mp3);
}

/*

int get_digit_at(int num, int digit) {
	return (num / ten_to_the(digit - 1)) % 10;
}

void number_init(int res) {
	digits = get_num_of_digits(res);
	correct_digits = 0;
	is_num_sign_entered = false;
	button_bits = 0x00;
	is_number_initialised = true;
	input_glyph = NULL;
	incorrect_tries = 0;
}

bool number_check_num_sign() {
	if (button_bits == SYM_NUM_SIGN) {
		is_num_sign_entered = true;
	} else {
		input_error();
	}
}

bool number_handler(int* inp, int res) {
	if (is_number_initialised == false) {
		number_init();
	}

	switch (ui_last_dot) {
		case NO_DOTS:


		case WAIT:
			return false;
			break;

		case '1': case '2': case '3': case '4': case '5': case '6':
			button_bits = add_dot(button_bits, ui_last_dot);
			return true;
			break;

		case CANCEL:
			play_mp3(lang_fileset, "CANC");
			button_bits = 0x00;
			return false;
			break;

		case ENTER:
			if (is_num_sign_entered == false) {
				if (number_check_num_sign() == true) {
					return false;
				} else {
					input_error();
					return false;
				}
			}
	}
}
*/
