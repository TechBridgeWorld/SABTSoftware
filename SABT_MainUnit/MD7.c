/**
 * @file MD7.c
 * @brief Mode logic for Mode 7 - Hindi Braille alphabet practice
 * @author Vivek Nair (viveknai)
 */
#include <stdlib.h>

#include "audio.h"
#include "common.h"
#include "Globals.h"

/* *NOTE* - For new language modules, replace this header file with the header
						for the new script */
#include "script_devanagari.h"

//Mode states
#define STATE_MENU			0x00
#define STATE_INIT 			0x01
#define STATE_PROMPT		0x02
#define STATE_INPUT			0x03
#define STATE_CHECK 		0x04
#define STATE_ADVANCE 	0x05
#define STATE_INCORRECT 0x06
#define STATE_TRYAGAIN 	0x07
#define STATE_SKIP			0x08
#define STATE_START			0x09

/*	Submodes
 *	The mode has 2 submodes - a sequential LEARN submode that teaches the
 *	alphabet and a PLAY submode that picks random letters to test on */
#define SUBMODE_NONE		0x00
#define SUBMODE_LEARN		0x01
#define SUBMODE_PLAY		0x02

//State magic numbers
#define	PLAY_MODE_SIZE 				5
#define MAX_INCORRECT_TRIES		3

//Script and language configuration
/* *NOTE* - After adding a new header file and language files on the SD card,
 						edit the following 3 variables */
static script_t* this_script = &script_devanagari;
static char lang_fileset[5] = "HIN_";
static char mode_fileset[5] = "MD7_";

/* Defines initial state for mode state machine */
static char next_state = STATE_START;

//State variables
static char submode;
static char button_bits;
static char last_dot;
static short play_indices[PLAY_MODE_SIZE];
static short current_index;
static alphabet_t* current_alphabet;
static alphabet_t* input_alphabet;
static short correct_count;
static short incorrect_count;
static short incorrect_tries;
//static char debug[10];

/**
 * @brief Resets mode state variables to default values
 * @param void
 * @return void
 */
void reset_state() {
	submode = SUBMODE_NONE;
	button_bits = 0b00000000;
	last_dot = 0b00000000;
	for (int i = 0; i < PLAY_MODE_SIZE; i++) {
		play_indices[i] = 0;
	}
	current_index = 0;
	current_alphabet = NULL;
	input_alphabet = NULL;
	correct_count = 0;
	incorrect_count = 0;
	incorrect_tries = 0;
	PRINTF("State variables reset\n\r");
}

/**
 * @brief Implements mode state machine
 * @param void
 * @return void
 */
void md7_main(void) {

	switch(next_state) {

		case STATE_START:
			PRINTF("*** MD7 - Hindi Braille practice ***\n\r");
			PRINTF("STATE_START\n\r");
			reset_state();
			next_state = STATE_MENU;
			break;

		//Resets mode and lets user choose between LEARN and PLAY submodes
		case STATE_MENU:
			//User presses dot 1 for LEARN, dot 2 for PLAY
			switch (last_dot) {
				case 0:
					play_mp3(mode_fileset, "MENU");
					last_dot = 1;
					break;

				case 1:
					break;


				case '1':
					submode = SUBMODE_LEARN;
					PRINTF("** LEARN submode **\n\r");
					next_state = STATE_INIT;
					last_dot = 0;
					break;

				case '2':
					submode = SUBMODE_PLAY;
					PRINTF("** PLAY submode **\n\r");
					next_state = STATE_INIT;
					last_dot = 0;
					break;

				default:
					play_mp3(lang_fileset, "INVP");
					next_state = STATE_MENU;
					last_dot = 0;
					break;
			}
			break;

		//Initialises submode
		case STATE_INIT:
			PRINTF("STATE_INIT\n\r");

			switch (submode) {
				//Prepares for sequential learn mode
				case SUBMODE_LEARN:
					play_mp3(mode_fileset, "LINT");
					current_alphabet = &(this_script->alphabets[0]);
					break;

				// Generates random indices from the script for play
				case SUBMODE_PLAY:
					play_mp3(mode_fileset, "PINT");
					PRINTF("Initialising play letter array: ");
					for (int i = 0; i < PLAY_MODE_SIZE; i++) {
						play_indices[i] = timer_rand() % this_script->length;
						PRINTF((this_script->alphabets[play_indices[i]]).sound);
						PRINTF(", ");
					}
					NEWLINE;
					current_alphabet =
						&(this_script->alphabets[play_indices[0]]);
					break;

				default:
					break;
			}
			next_state = STATE_PROMPT;
			break; 

		//Play prompt for letter
		case STATE_PROMPT:
			
			PRINTF("STATE_PROMPT\n\r");
			switch (submode) {
				case SUBMODE_LEARN:
					play_alphabet(lang_fileset, current_alphabet);
					play_mp3(mode_fileset, "LPRO"); 
					play_dot_sequence(lang_fileset, current_alphabet); 
					break;

				//Does not play dot sequence in play mode
				case SUBMODE_PLAY:
					play_alphabet(lang_fileset, current_alphabet);
					break;

				default:
					break;
			}
			button_bits = 0;
			next_state = STATE_INPUT;
			break;
		
		//Wait for user to complete input of dots 
		case STATE_INPUT:
			//Adds new dots to bit pattern and proceeds to check when ENTER is
			//	pressed
			if (last_dot != 0) {
				if (last_dot >= '1' && last_dot <= '6') {
					 PRINTF("Dot added: ");
					 SENDBYTE(last_dot);
					 PRINTF("\n\r");
					 button_bits = add_dot(button_bits, last_dot);
					 play_dot(lang_fileset, last_dot);
				} else if (last_dot == ENTER) {
						next_state = STATE_CHECK;
				}
			}
			last_dot = 0;
			break;

		//Checks cell input from user
		case STATE_CHECK:
			PRINTF("STATE_CHECK\n\r");
			input_alphabet = get_alphabet_by_bits(button_bits, this_script);
			if (is_same_alphabet(current_alphabet, input_alphabet)) {
				correct_count++;
				incorrect_tries = 0;
				play_mp3(mode_fileset, "CORR");
				next_state = STATE_ADVANCE;
			} else {
				incorrect_tries++;
				play_input_error(lang_fileset, button_bits);
				next_state = STATE_INCORRECT;
			}
			break;

		//If user input matches current alphabet
		case STATE_ADVANCE:
			PRINTF("STATE_ADVANCE\n\r");
			//If correct, then check if target for submode achieved otherwise just
			//	proceed to next letter
			current_index++;
			switch (submode) {
				case SUBMODE_LEARN:
					if (current_index >= this_script->length) {
						play_mp3(mode_fileset, "LCOM");
						next_state = STATE_TRYAGAIN;
					} else {
						play_mp3(mode_fileset, "NEXT");
						current_alphabet = &((this_script->alphabets)[current_index]);
						next_state = STATE_PROMPT;
					}
					break; 

				case SUBMODE_PLAY:
					if (current_index >= PLAY_MODE_SIZE) {
						play_mp3(mode_fileset, "PCOM");
						next_state = STATE_TRYAGAIN;
					} else {
						play_mp3(mode_fileset, "NEXT");
						current_alphabet =
							&(this_script->alphabets[play_indices[current_index]]);
						next_state = STATE_PROMPT; 
					}
					break;

				default:
					break;
			}
			break;
			
		//If user input does not match current alphabet
		case STATE_INCORRECT:
			PRINTF("STATE_INCORRECT\n\r");
			//After a certain number of incorrect answers, ask user if they want
			// to skip
			if (incorrect_tries >= MAX_INCORRECT_TRIES) {
				next_state = STATE_SKIP;
			} else {
				next_state = STATE_PROMPT;
			}
			break;

		//Asks user to try again
		case STATE_TRYAGAIN:
			switch (last_dot) {
				case 0:
					play_mp3(mode_fileset, "TAGA");
					last_dot = 1;
					break;

				case 1:
					break;

				case '1':
					next_state = STATE_START;
					last_dot = 0;
					break;

				case '2':
					/* TODO - Quit mode */
					last_dot = 0;
					break;

				default:
					play_mp3(lang_fileset, "INVP");
					last_dot = 0;
					break;
			}
			break;

		case STATE_SKIP:
			switch (last_dot) {
				case 0:
					play_mp3(mode_fileset, "SKIP");
					last_dot = 1;
					break;

				case 1:
					break;
				
				case '1':
					next_state = STATE_ADVANCE;
					last_dot = 0;
					break;

				case '2':
					incorrect_tries = 0;
					next_state = STATE_PROMPT;
					last_dot = 0;
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
	next_state = STATE_START;
}

void md7_call_mode_yes_answer(void) {
	last_dot = ENTER;
}

void md7_call_mode_no_answer(void) {
	PRINTF("Received CANCEL signal");
	NEWLINE;
	if (next_state == STATE_INPUT) {
		PRINTF("Switching to mode submenu");
		NEWLINE;
		next_state = STATE_MENU;
	} else if (next_state == STATE_MENU) {
		PRINTF("To implement: QUIT");
		NEWLINE;
		/* TODO - Quit mode */
	} else {
		PRINTF("Setting last_dot");
		NEWLINE;
		last_dot = CANCEL;
	}
}

void md7_input_dot(char this_dot) {
	last_dot = this_dot;
}

void md7_input_cell(char this_cell) {
	/* TODO  */
}
