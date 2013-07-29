/**
* @file MD8.c
* @brief Code for mode 8 - English Number Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>

// Custom libraries
#include "audio.h"
#include "io.h"
#include "common.h"
#include "debug.h"
#include "script_common.h"

// State machine constants
#define STATE_NULL 0x00
#define STATE_MENU 0x01
#define STATE_GENQUES 0x02
#define STATE_PROMPT 0x03
#define STATE_INPUT 0x04
#define STATE_CHECK 0x05
#define STATE_REPROMPT 0x06

// Submode constants
#define SUBMODE_NULL 0x00
#define SUBMODE_LEARN 0x01
#define SUBMODE_PLAY 0x02

// Mode MP3 constants
#define MP3_MENU "MENU"
#define MP3_FOR_X_PRESS_DOTS "FXPD"
#define MP3_REPROMPT "RMEN"
#define MP3_INSTRUCTIONS "INST"

#define MAX_INCORRECT_TRIES 3

// Script and fileset parameters
#include "script_digits.h"
#define SCRIPT_ADDRESS &script_digits
#define SCRIPT_LENGTH SCRIPT_DIGITS_LENGTH
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD8_"

// State variables
static char next_state = STATE_NULL;
static char user_response = NO_DOTS;
static char submode = SUBMODE_NULL;
static int index = 0;
static glyph_t* curr_glyph = NULL;
static glyph_t* user_glyph = NULL;
static char cell = 0;
static char cell_pattern = 0;
static char cell_control = 0;
static int incorrect_tries = 0;

void md8_reset(void) {
	set_mode_globals(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
	next_state = STATE_MENU;
	user_response = NO_DOTS;
	submode = SUBMODE_NULL;
	index = 0;
	curr_glyph = NULL;
	user_glyph = NULL;
	cell = 0;
	cell_pattern = 0;
	cell_control = 0;
	incorrect_tries = 0;
	PRINTF("[MD8] Mode reset\n\r");
}

void md8_main(void) {
	switch (next_state) {
		
		case STATE_MENU:
			switch(create_dialog(MP3_MENU, DOT_1 | DOT_2 | ENTER_CANCEL)) {
				
				case NO_DOTS:
					break;

				case '1':
					PRINTF("[MD8] Submode: Learn\n\r");
					play_mp3(MODE_FILESET, MP3_INSTRUCTIONS);
					submode = SUBMODE_LEARN;
					next_state = STATE_GENQUES;
					break;

				case '2':
					PRINTF("[MD8] Submode: Play\n\r");
					play_mp3(MODE_FILESET, MP3_INSTRUCTIONS);
					submode = SUBMODE_PLAY;
					next_state = STATE_GENQUES;
					break;

				case CANCEL:
					PRINTF("[MD8] Quitting to main menu\n\r");
					quit_mode();
					break;

				case ENTER:
					PRINTF("[MD8] Re-issuing main menu prompt\n\r");
					next_state = STATE_MENU;
					break;

				default:
					break;			
			}
			break;

		case STATE_GENQUES:
			switch (submode) {

				case SUBMODE_LEARN:
					curr_glyph = get_next_glyph(SCRIPT_ADDRESS);
					if (curr_glyph == NULL) {
						reset_script_indices(SCRIPT_ADDRESS);
						next_state = STATE_GENQUES;
						break;
					}
					break;

				case SUBMODE_PLAY:
					curr_glyph = get_random_glyph(SCRIPT_ADDRESS);
					break;

				default:
					break;

			}
			sprintf(dbgstr, "[MD8] Next glyph: %s\n\r", curr_glyph->sound);
			PRINTF(dbgstr);
			play_mp3(LANG_FILESET, MP3_NEXT_LETTER);
			next_state = STATE_PROMPT;
			break;

		case STATE_PROMPT:
			switch(submode) {

				case SUBMODE_LEARN:
					play_glyph(curr_glyph);
					play_mp3(MODE_FILESET, MP3_FOR_X_PRESS_DOTS);
					play_dot_sequence(curr_glyph);
					break;

				case SUBMODE_PLAY:
					play_silence(500);
					play_glyph(curr_glyph);
					break;

				default:
					break;
			}
			next_state = STATE_INPUT;
			break;

		case STATE_INPUT:
			if (io_user_abort == true) {
				PRINTF("[MD8] User aborted input\n\r");
				next_state = STATE_REPROMPT;
				io_init();
			}
			cell = get_cell();
			if (cell == NO_DOTS) {
				break;
			}
			cell_pattern = GET_CELL_PATTERN(cell);
			cell_control = GET_CELL_CONTROL(cell);
			switch (cell_control) {
				case 0b11:
					user_glyph = search_script(SCRIPT_ADDRESS, cell_pattern);
					next_state = STATE_CHECK;
					PRINTF("[MD8] Checking answer\n\r");
					break;
				case 0b10:
					next_state = STATE_PROMPT;
					break;
				case 0b01:
					next_state = STATE_REPROMPT;
					break;
				case 0b00:
					break;
			}
			break;

		case STATE_CHECK:
			if (glyph_equals(curr_glyph, user_glyph)) {
				incorrect_tries = 0;
				PRINTF("[MD8] User answered correctly\n\r");
				play_mp3(LANG_FILESET, MP3_CORRECT);
				play_mp3(SYS_FILESET, MP3_TADA);
				next_state = STATE_GENQUES;
			} else {
				incorrect_tries++;
				PRINTF("[MD8] User answered incorrectly\n\r");
				play_mp3(LANG_FILESET, MP3_INCORRECT);
				play_mp3(LANG_FILESET, MP3_TRY_AGAIN);
				next_state = STATE_PROMPT;
				if (incorrect_tries >= MAX_INCORRECT_TRIES) {
					play_glyph(curr_glyph);
					play_mp3(MODE_FILESET, MP3_FOR_X_PRESS_DOTS);
					play_dot_sequence(curr_glyph);
					play_mp3(LANG_FILESET, MP3_TRY_AGAIN);
					next_state = STATE_INPUT;
				}
			}
			break;

		case STATE_REPROMPT:
			switch(create_dialog(MP3_REPROMPT,
				DOT_1 | DOT_2 | DOT_3 | ENTER_CANCEL | LEFT_RIGHT)) {
				case NO_DOTS:
					break;

				case '1':
					PRINTF("[MD8] Skipping character\n\r");
					next_state = STATE_GENQUES;
					break;

				case '2':
					PRINTF("[MD8] Playing pattern\n\r");
					play_glyph(curr_glyph);
					play_mp3(MODE_FILESET, MP3_FOR_X_PRESS_DOTS);
					play_dot_sequence(curr_glyph);
					play_mp3(LANG_FILESET, MP3_TRY_AGAIN);
					next_state = STATE_INPUT;
					break;

				case '3':
					PRINTF("[MD8] Reissuing prompt\n\r");
					next_state = STATE_PROMPT;
					break;

				case CANCEL:
					PRINTF("[MD8] Cancelling to submode menu\n\r");
					md8_reset();
					break;

				case ENTER:
					PRINTF("[MD8] Reissuing prompt\n\r");
					next_state = STATE_PROMPT;
					break;

				case LEFT:
					PRINTF("[MD8] Previous letter\n\r");
					switch (submode) {
						case SUBMODE_LEARN:
							curr_glyph = get_prev_glyph(SCRIPT_ADDRESS);
							if (curr_glyph == NULL) {
								curr_glyph = get_next_glyph(SCRIPT_ADDRESS);
							}
							break;
						case SUBMODE_PLAY:
							curr_glyph = get_random_glyph(SCRIPT_ADDRESS);
							break;
						default:
							break;
					}
					play_glyph(curr_glyph);
					break;

				case RIGHT:
					PRINTF("[MD8] Next letter\n\r");
					switch (submode) {
						case SUBMODE_LEARN:
							curr_glyph = get_next_glyph(SCRIPT_ADDRESS);
							if (curr_glyph == NULL) {
								curr_glyph = get_prev_glyph(SCRIPT_ADDRESS);
							}
							break;
						case SUBMODE_PLAY:
							curr_glyph = get_random_glyph(SCRIPT_ADDRESS);
							break;
						default:
							break;
					}
					play_glyph(curr_glyph);
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}
}
