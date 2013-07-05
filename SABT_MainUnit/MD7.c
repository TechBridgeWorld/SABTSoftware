/**
 * @file MD7.c
 * @brief Mode logic for Mode 7 - Hindi Braille glyph practice
 * @author Vivek Nair (viveknair@cmu.edu)
 */
#include <stdlib.h>

#include "audio.h"
#include "common.h"
#include "script_common.h"
#include "Globals.h"

/* *NOTE* - For new language modules, replace this header file with the header
						for the new script */
#include "script_hindi.h"

//Mode states
#define MD7_STATE_MENU			0x00
#define MD7_STATE_INIT 			0x01
#define MD7_STATE_PROMPT		0x02
#define MD7_STATE_INPUT			0x03
#define MD7_STATE_CHECK 		0x04
#define MD7_STATE_NEXT 			0x05
#define MD7_STATE_INCORRECT 0x06
#define MD7_STATE_TRYAGAIN 	0x07
#define MD7_STATE_SKIP			0x08
#define MD7_STATE_START			0x09
#define MD7_STATE_PREV			0x0B

/*	Submodes
 *	The mode has 2 submodes - a sequential LEARN submode that teaches the
 *	glyph and a PLAY submode that picks random letters to test on */
#define MD7_SUBMODE_NONE		0x00
#define MD7_SUBMODE_LEARN		0x01
#define MD7_SUBMODE_PLAY		0x02

//State magic numbers
#define	PLAY_MODE_SIZE 				5
#define MAX_INCORRECT_TRIES		3

// *NOTE* Update this macro to refer to script length from script header file
#define SCRIPT_LENGTH SCRIPT_HINDI_LENGTH

//Script and language configuration
/* *NOTE* - After adding a new header file and language files on the SD card,
 						edit the following 3 variables */
static script_t* this_script = &script_hindi;
static char* this_lang_fileset = script_hindi.fileset;
static char this_mode_fileset[5] = "MD7_";

/* Defines initial state for mode state machine */
static char next_state = MD7_STATE_START;
static char submode;
static char button_bits;
static char last_dot;
static short learn_flags[SCRIPT_LENGTH];
static short play_indices[PLAY_MODE_SIZE];
static short play_flags[PLAY_MODE_SIZE];
static short current_index;
static glyph_t* current_glyph;
static glyph_t* input_glyph;
static short correct_count;
static short incorrect_count;
static short incorrect_tries;
static short* flag_array;
static short bound;
static char com_mp3[5];
static char debug[64];
static char correct = 0;
static char cancel = 0;

void md7_reset_vars() {
	set_mode_globals(this_script, this_lang_fileset, this_mode_fileset);
	submode = MD7_SUBMODE_NONE;
	button_bits = 0b00000000;
	last_dot = 0b00000000;
	for (int i = 0; i < SCRIPT_LENGTH; i++) {
		learn_flags[i] = 0;
	}
	for (int i = 0; i < PLAY_MODE_SIZE; i++) {
		play_indices[i] = 0;
		play_flags[i] = 0;
	}
	current_index = 0;
	current_glyph = NULL;
	input_glyph = NULL;
	correct_count = 0;
	incorrect_count = 0;
	incorrect_tries = 0;
	flag_array = NULL;
	bound = 0;
	strcpy(com_mp3,"");
	correct = 0;
	cancel = 0;
	PRINTF("State variables reset\n\r");
}

void md7_main(void) {

	switch(next_state) {

		// Initialises mode
		case MD7_STATE_START:
			PRINTF("*** MD7 - Hindi Braille practice ***\n\r");
			PRINTF("MD7_STATE_START\n\r");
			md7_reset_vars();
			next_state = MD7_STATE_MENU;
			break;

		//Resets mode and lets user choose between LEARN and PLAY submodes
		case MD7_STATE_MENU:
			//User presses dot 1 for LEARN, dot 2 for PLAY
			switch (last_dot) {
				case 0:
					play_mp3(mode_fileset, "MENU");
					incorrect_tries = 0;
					last_dot = 1;
					break;

				case 1:
					break;

				case '1':
					submode = MD7_SUBMODE_LEARN;
					PRINTF("** LEARN submode **\n\r");
					next_state = MD7_STATE_INIT;
					last_dot = 0;
					incorrect_tries = 0;
					break;

				case '2':
					submode = MD7_SUBMODE_PLAY;
					PRINTF("** PLAY submode **\n\r");
					next_state = MD7_STATE_INIT;
					last_dot = 0;
					incorrect_tries = 0;
					break;

				default:
					incorrect_tries++;
					play_mp3(lang_fileset, "INVP");
					if (incorrect_tries >= MAX_INCORRECT_TRIES) {
						last_dot = 0;
					} else {
						last_dot = 1;
					}
					break;
			}
			break;

		//Initialises submode
		case MD7_STATE_INIT:
			PRINTF("MD7_STATE_INIT\n\r");

			switch (submode) {
				//Prepares for sequential learn mode
				case MD7_SUBMODE_LEARN:
					play_mp3(mode_fileset, "LINT");
					current_glyph = &(this_script->glyphs[0]);
					break;

				// Generates random indices from the script for play
				case MD7_SUBMODE_PLAY:
					play_mp3(mode_fileset, "PINT");
					PRINTF("Initialising play letter array: ");
					for (int i = 0; i < PLAY_MODE_SIZE; i++) {
						play_indices[i] = timer_rand() % SCRIPT_LENGTH;
						play_flags[i] = 0;
						PRINTF((this_script->glyphs[play_indices[i]]).sound);
						PRINTF(", ");
					}
					NEWLINE;
					current_glyph =
						&(this_script->glyphs[play_indices[0]]);
					break;

				default:
					break;
			}
			next_state = MD7_STATE_PROMPT;
			break; 

		//Play prompt for letter
		case MD7_STATE_PROMPT:
			
			PRINTF("MD7_STATE_PROMPT\n\r");
			switch (submode) {
				case MD7_SUBMODE_LEARN:
					play_glyph(current_glyph);
					play_mp3(mode_fileset, "LPRO"); 
					play_dot_sequence(current_glyph); 
					break;

				//Does not play dot sequence in play mode
				case MD7_SUBMODE_PLAY:
					play_glyph(current_glyph);
					break;

				default:
					break;
			}
			button_bits = 0;
			next_state = MD7_STATE_INPUT;
			break;
		
		//Wait for user to complete input of dots 
		case MD7_STATE_INPUT:
			//Adds new dots to bit pattern and proceeds to check when ENTER is
			//	pressed
			switch (last_dot) {
				case 0:
					break;

				case ENTER:
					next_state = MD7_STATE_CHECK;
					break;

				case LEFT:
					play_mp3(mode_fileset, "NEXT");
					next_state = MD7_STATE_PREV;
					break;

				case RIGHT:
					play_mp3(mode_fileset, "NEXT");
					correct = 0;
					next_state = MD7_STATE_NEXT;
					return;
					break;

				case CANCEL:
					// Press CANCEL once to delete last character, press CANCEL again
					// to 
					if (cancel == 0) {
						play_mp3(lang_fileset, "CANC");
						button_bits = 0x00;
						cancel = 1;
					} else {
						cancel = 0;
						next_state = MD7_STATE_START;
					}
					break;

				default:
					if (last_dot >= '1' && last_dot <= '6') {
						 PRINTF("Dot added: ");
						 SENDBYTE(last_dot);
						 PRINTF("\n\r");
						 button_bits = add_dot(button_bits, last_dot);
						 play_dot(last_dot);
					}
					break;
			}
			last_dot = 0;
			break;

		//Checks cell input from user
		case MD7_STATE_CHECK:
			PRINTF("MD7_STATE_CHECK\n\r");

			//When user presses no dots and presses ENTER, prompt again
			if (button_bits == 0x00) {
				if (incorrect_tries >= MAX_INCORRECT_TRIES) {
					play_mp3(mode_fileset, "PLSP");
					switch (submode) {
						case MD7_SUBMODE_LEARN:
							play_mp3(mode_fileset, "LINT");
							break;
						case MD7_SUBMODE_PLAY:
							play_mp3(mode_fileset, "PINT");
							break;
						default:
							break;
					}
					next_state = MD7_STATE_PROMPT;
				} else {
					incorrect_tries++;
					play_mp3(lang_fileset, "INVP");
					next_state = MD7_STATE_INPUT;
				}
			} else {
				//Otherwise check glyph
				input_glyph = get_glyph_by_pattern(button_bits);
				if (glyph_equals(current_glyph, input_glyph)) {
					play_mp3(lang_fileset, "CORR");
					if (correct_count + 1 < bound)
						play_mp3(mode_fileset, "NEXT");
					incorrect_tries = 0;
					correct = 1;
					last_dot = RIGHT;
					next_state = MD7_STATE_NEXT;
				} else {
					incorrect_tries++;
					play_mp3(lang_fileset, "INVP");
					next_state = MD7_STATE_INCORRECT;
				}
			}
			break;

		//If user input does not match current glyph
		case MD7_STATE_INCORRECT:
			PRINTF("MD7_STATE_INCORRECT\n\r");
			//After a certain number of incorrect answers, ask user if they want
			// to skip
			if (incorrect_tries >= MAX_INCORRECT_TRIES) {
				next_state = MD7_STATE_SKIP;
			} else {
				next_state = MD7_STATE_PROMPT;
			}
			break;

		//Asks user to try again
		case MD7_STATE_TRYAGAIN:
			switch (last_dot) {
				case 0:
					play_mp3(mode_fileset, "TAGA");
					last_dot = 1;
					break;

				case 1:
					break;

				case '1':
					next_state = MD7_STATE_INIT;
					last_dot = 0;
					break;

				case '2':
					/* TODO - Quit mode */
					quit_mode();
					break;

				case CANCEL:
					next_state = MD7_STATE_START;
					break;

				default:
					play_mp3(lang_fileset, "INVP");
					last_dot = 0;
					break;
			}
			break;

		// Skip letter menu
		case MD7_STATE_SKIP:
			switch (last_dot) {
				case 0:
					play_mp3(mode_fileset, "SKIP");
					last_dot = 1;
					break;

				case 1:
					break;
				
				case '1':
					next_state = MD7_STATE_NEXT;
					play_mp3(mode_fileset, "NEXT");
					incorrect_tries = 0;
					correct = 0;
					last_dot = RIGHT;
					break;

				case '2':
					incorrect_tries = 0;
					next_state = MD7_STATE_PROMPT;
					last_dot = 0;
					break;

				case '3':
					play_glyph(current_glyph);
					play_mp3(mode_fileset, "LPRO");
					play_pattern(current_glyph->pattern);
					button_bits = 0x00;
					next_state = MD7_STATE_INPUT;
					last_dot = 0;
					break;

				case LEFT:
					next_state = MD7_STATE_PREV;
					break;

				case RIGHT:
					correct = 0;
					next_state = MD7_STATE_NEXT;
					break;

				case CANCEL:
					next_state = MD7_STATE_START;
					break;

				default:
					play_mp3(lang_fileset, "INVP");
					last_dot = 0;
					break;
			}
			break;

		// Advances through list of letters
		case MD7_STATE_NEXT:
			switch(last_dot) {

				case 0:
					break;

				case RIGHT:
					PRINTF("Next glyph");
					NEWLINE;

					switch (submode) {
						case MD7_SUBMODE_LEARN:
							flag_array = learn_flags;
							bound = SCRIPT_LENGTH;
							strcpy(com_mp3, "LCOM");
							break;

						case MD7_SUBMODE_PLAY:
							flag_array = play_flags;
							bound = PLAY_MODE_SIZE;
							strcpy(com_mp3, "PCOM");
							break;

						default: 
							break;
					}

					//If correct, mark index as completed
					if (correct == 1) {
						flag_array[current_index] = 1;
						correct_count++;
					}

					//If # of correct entries == length of script/play array,
					//	then submode complete
					if (correct_count == bound) {
						play_mp3(mode_fileset, com_mp3);
						next_state = MD7_STATE_TRYAGAIN;
					} else {
						//Loop till an index not yet completed is found
						for (int i = PLUS_ONE_MOD(current_index, bound);
									i != current_index;
									i = PLUS_ONE_MOD(i, bound)) {
							if (flag_array[i] == 0) {
								current_index = i;
								break;
							}
						}

						switch (submode) {
							case MD7_SUBMODE_LEARN:
								current_glyph = &((this_script->glyphs)[current_index]);
								break;

							case MD7_SUBMODE_PLAY:
								current_glyph =
									&(this_script->glyphs[play_indices[current_index]]);
								break;

							default:
								break;
						}

						//If just scrolling through letters, play sound
						if (correct == 0) {
							play_glyph(current_glyph);
						} else {
							next_state = MD7_STATE_PROMPT;
							correct = 0;
						}
					}
					last_dot = 0;
					break;

				case ENTER:
					last_dot = 0;
					next_state = MD7_STATE_PROMPT;
					break;

				case LEFT:
					next_state = MD7_STATE_PREV;
					break;

				case CANCEL:
					next_state = MD7_STATE_START;
					break;

				default:
					play_mp3(lang_fileset, "INVP");
					last_dot = 0;
					break;
			}
			break;

		// Goes back in list of letters
		case MD7_STATE_PREV:
			switch (last_dot) {

				case 0:
					break;

				case LEFT:
					PRINTF("Previous glyph");
					NEWLINE;
					
					switch (submode) {
						case MD7_SUBMODE_LEARN:
							flag_array = learn_flags;
							bound = SCRIPT_LENGTH;
							strcpy(com_mp3, "LCOM");
							break;

						case MD7_SUBMODE_PLAY:
							flag_array = play_flags;
							bound = PLAY_MODE_SIZE;
							strcpy(com_mp3, "PCOM");
							break;

						default:
							break;
					}

					//Loop till an index not yet completed is found
					for (int i = MINUS_ONE_MOD(current_index, bound);
								i != current_index;
								i = MINUS_ONE_MOD(i, bound)) {
						if (flag_array[i] == 0) {
							current_index = i;
							break;
						}
					}
					sprintf(debug, "Current index: %d", current_index);
					PRINTF(debug);
					NEWLINE;

					switch (submode) {
						case MD7_SUBMODE_LEARN:
							current_glyph = &((this_script->glyphs)[current_index]);
							break;

						case MD7_SUBMODE_PLAY:
							current_glyph =
								&(this_script->glyphs[play_indices[current_index]]);
							break;

						default:
							break;
					}
					play_glyph(current_glyph);
					last_dot = 0;
					break;

				case RIGHT:
					correct = 0;
					next_state = MD7_STATE_NEXT;
					break;

				case ENTER:
					last_dot = 0;
					next_state = MD7_STATE_PROMPT;
					break;

				case CANCEL:
					next_state = MD7_STATE_START;
					break;

				default:
					play_mp3(lang_fileset, "INVP");
					last_dot = 0;
					break;
			}
			break;

		default:
			break;
	}
}

void md7_reset(void) {
	next_state = MD7_STATE_START;
}

void md7_call_mode_yes_answer(void) {
	last_dot = ENTER;
}

void md7_call_mode_no_answer(void) {
	last_dot = CANCEL;
}

void md7_input_dot(char this_dot) {
	last_dot = this_dot;
}

void md7_input_cell(char this_cell) {
	/* TODO  */
}

void md7_call_mode_left() {
	last_dot = LEFT;
}

void md7_call_mode_right() {
	last_dot = RIGHT;
}
