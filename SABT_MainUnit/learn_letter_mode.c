

/*
 * @file learn_letter_mode.c
 *
 * @brief code for helper functions "learning letter modes" (MD 2,7,8,12)
 * @author: Edward Cai
 */ 

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Custom libraries
#include "audio.h"
#include "io.h"
#include "common.h"
#include "debug.h"
#include "script_common.h"
#include "learn_letter_mode.h"

// State variables
static char mode_name[5];
static char next_state = STATE_NULL;
static char user_response = NO_DOTS;
static char submode = SUBMODE_NULL;
static bool should_shuffle = false;
static int index = 0;
static glyph_t* curr_glyph = NULL;
static glyph_t* user_glyph = NULL;
static char cell = 0;
static char cell_pattern = 0;
static char cell_control = 0;
static int incorrect_tries = 0;
static bool scrolled = false;

void learn_letter_reset(script_t* new_script, char* new_lang_fileset, char* new_mode_fileset) {
	set_mode_globals(new_script, new_lang_fileset, new_mode_fileset);
	strcpy(mode_name,new_mode_fileset);
	strtok(mode_name, "_");
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
	scrolled = false;
	sprintf(dbgstr, "[%s] Mode reset\n\r", mode_name;
	PRINTF(dbgstr);
}

void learn_letter_main(script_t* SCRIPT_ADDRESS, char* LANG_FILESET, char* MODE_FILESET) {
	switch (next_state) {
		case STATE_MENU:
			switch(create_dialog(MP3_MENU, DOT_1 | DOT_2 | ENTER_CANCEL)) {	
				case NO_DOTS:
					break;

				case '1':
					sprintf(dbgstr, "[%s] Submode: Learn\n\r", mode_name);
					PRINTF(dbgstr);			
					play_mp3(MODE_FILESET, MP3_INSTRUCTIONS);
					submode = SUBMODE_LEARN;
					next_state = STATE_GENQUES;
					break;

				case '2':
					sprintf(dbgstr, "[%s] Submode: Play\n\r", mode_name);
					PRINTF(dbgstr);
					play_mp3(MODE_FILESET, MP3_INSTRUCTIONS);
					submode = SUBMODE_PLAY;
					shuffle(SCRIPT_ADDRESS);
					should_shuffle = true;
					next_state = STATE_GENQUES;
					break;

				case CANCEL:
					sprintf(dbgstr, "[%s] Quitting to main menu \n\r", mode_name);
					PRINTF(dbgstr);
					quit_mode();
					break;

				case ENTER:
					sprintf(dbgstr, "[%s] Re-issuing main menu prompt\n\r", mode_name);
					PRINTF(dbgstr);
					next_state = STATE_MENU;
					break;

				default:
					break;
			}
			break;

		case STATE_GENQUES:
			curr_glyph = get_next_glyph(SCRIPT_ADDRESS, submode);
			sprintf(dbgstr, "[%s] Next glyph: %s\n\r",mode_name, curr_glyph->sound);
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
				sprintf(dbgstr, "[%s] User aborted input\n\r", mode_name);
				PRINTF(dbgstr);
				next_state = STATE_REPROMPT;
				io_init();
			}
			cell = get_cell();
			if (cell == NO_DOTS)
				break;
			cell_pattern = GET_CELL_PATTERN(cell);
			cell_control = GET_CELL_CONTROL(cell);
			switch (cell_control) {
				case WITH_ENTER:
					user_glyph = search_script(SCRIPT_ADDRESS, cell_pattern);
					next_state = STATE_CHECK;
					sprintf(dbgstr, "[%s] Checking answer \n\r", mode_name);
					PRINTF(dbgstr);
					break;
				case WITH_LEFT:
					next_state = STATE_PROMPT;
					break;
				case WITH_RIGHT:
					next_state = STATE_REPROMPT;
					break;
				case WITH_CANCEL:
					break;
			}
			break;

		case STATE_CHECK:
			if (glyph_equals(curr_glyph, user_glyph)) {
				if (curr_glyph -> next == NULL) {
					incorrect_tries = 0;
					sprintf(dbgstr, "[%s] User answered correctly\n\r", mode_name);
					PRINTF(dbgstr);
					play_mp3(LANG_FILESET, MP3_CORRECT);
					play_mp3(SYS_FILESET, MP3_TADA);
					next_state = STATE_GENQUES;
				}
				else {
					curr_glyph = get_next(SCRIPT_ADDRESS, curr_glyph);
					play_mp3(LANG_FILESET, MP3_NEXT_CELL);
					play_dot_sequence(curr_glyph);
					next_state = STATE_INPUT;
				}
			}
			else {
				incorrect_tries++;
				sprintf(dbgstr, "[%s] User answered incorrectly\n\r", mode_name);;
				PRINTF(dbgstr);			
				play_mp3(LANG_FILESET, MP3_INCORRECT);
				play_mp3(LANG_FILESET, MP3_TRY_AGAIN);
				curr_glyph = get_root(SCRIPT_ADDRESS, curr_glyph);
				next_state = STATE_PROMPT;
				if (incorrect_tries >= MAX_INCORRECT_TRIES) {
					play_glyph(curr_glyph);
					play_mp3(MODE_FILESET, MP3_FOR_X_PRESS_DOTS);
					play_dot_sequence(curr_glyph);
					next_state = STATE_INPUT;
				}
			}
		break;

		case STATE_REPROMPT:
			switch(create_dialog("SKIP", ENTER_CANCEL | LEFT_RIGHT)) {
				case NO_DOTS:
					break;

				case CANCEL:
					sprintf(dbgstr, "[%s] Reissuing prompt\n\r", mode_name);
					PRINTF(dbgstr);
					next_state = STATE_PROMPT;
					scrolled = false;
					break;

				case ENTER:
					sprintf(dbgstr, "[%s] Skipping character\n\r", mode_name);
					PRINTF(dbgstr);
					if (scrolled)
						next_state = STATE_PROMPT;
					else
						next_state = STATE_GENQUES;
					scrolled = false;
					break;

				case LEFT:
					sprintf(dbgstr, "[%s] Previous letter\n\r", mode_name);
					PRINTF(dbgstr);
					curr_glyph = get_prev_glyph(SCRIPT_ADDRESS, should_shuffle);
					play_glyph(curr_glyph);
					scrolled = true;
					break;

				default:
					break;
			}
				break;

		default:
			break;
	}
}
