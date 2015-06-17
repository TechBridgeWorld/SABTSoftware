/**
 * @file MD9.c
 * @brief Code for mode 9 - Maths Practice
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "common.h"
#include "globals.h"
#include "audio.h"
#include "script_common.h"
#include "script_digits.h"

#define STATE_NULL 0x00
#define STATE_MENU 0x01
#define STATE_LVLSEL 0x02
#define STATE_GENQUES 0x03
#define STATE_PROMPT 0x04
#define STATE_INPUT 0x05
#define STATE_CHECKANS 0x06
#define STATE_REPROMPT 0x07

// Submodes for different arithmetic operations
#define SUBMODE_NUL 0x00
#define SUBMODE_ADD 0x01
#define SUBMODE_SUB	0x02
#define SUBMODE_MUL 0x03

// Difficulty levels
#define LEVEL_NULL	0x0000
#define LEVEL_1			0x0001
#define LEVEL_2			0x0002
#define LEVEL_3			0x0003

// Used to set global fileset variables
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD9_"

// Prompt files
// Submode menu prompt
#define MP3_MENU	"MENU" //Menu
// Level select prompt
#define MP3_LVLSEL "LVLS" //Level select
#define MP3_INSTRUCTIONS "INST"
// Pre-question prompt
#define MP3_PREQUES "WHIS" //What is
// In-question operators
#define MP3_PLUS	"PLUS"
#define MP3_MINUS	"MINS"
#define MP3_TIMES	"TIMS"
#define MP3_YOU_ANSWERED "UANS"
// Skip prompt
#define MP3_SKIP "RMEN"
#define MP3_THE_ANSWER_IS "TAIS"

// Limits
#define MAX_DIGITS 3
#define MAX_INCORRECT_TRIES 3

// State variables
static char md_next_state = STATE_NULL;
static char md_submode = SUBMODE_NUL;
static char md_last_dot = NO_DOTS;
static int md_level = 0;
static int md_op_1 = -1;
static int md_op_2 = -1;
static int md_res = -1;
static int md_usr_res = -1;
static bool md_input_ready = false;
static bool md_input_valid = false;
static bool md_incorrect_tries = 0;


void md9_reset(void) {
	PRINTF("*** MD9 - Arithmetic practice ***\n\r");

	// Global variables
	set_mode_globals(&script_digits, LANG_FILESET, MODE_FILESET);

	// State variables
	md_next_state = STATE_MENU;
	md_submode = SUBMODE_NUL;
	md_last_dot = NO_DOTS;
	md_level = LEVEL_NULL;
	md_op_1 = -1;
	md_op_2 = -1;
	md_res = -1;
	md_usr_res = -1;
	md_input_ready = false;
	md_input_valid = false;
	md_incorrect_tries = 0;
}

void md9_generate_question(void) {
	
	// Sets bound for result
	int bound = 1;
	for (int i = 0; i < md_level; i++) {
		bound *= 10;
	}

	do {
		md_op_1 = timer_rand() % bound;
		sprintf(dbgstr, "[MD9] Operand 1: %d\n\r", md_op_1);
		PRINTF(dbgstr);
		md_op_2 = timer_rand() % bound;
		sprintf(dbgstr, "[MD9] Operand 2: %d\n\r", md_op_2);
		PRINTF(dbgstr);
		
		switch (md_submode) {
			case SUBMODE_ADD:
				md_res = md_op_1 + md_op_2;
				break;
			case SUBMODE_SUB:
				md_res = md_op_1 - md_op_2;
				break;
			case SUBMODE_MUL:
				md_res = md_op_1 * md_op_2;
				break;
			default:
				sprintf(dbgstr, "[MD9] Error: md_submode: %c\n\r",
					md_submode);
				PRINTF(dbgstr);
				quit_mode();
				break;
		}
	} while (!(md_res > 0 && md_res < bound));
	sprintf(dbgstr, "[MD9] Result: %d\n\r", md_res);
	PRINTF(dbgstr);	
}

void md9_play_question() {
	play_mp3(MODE_FILESET, MP3_PREQUES);
	play_number(md_op_1);
	switch (md_submode) {
		case SUBMODE_ADD:
			play_mp3(MODE_FILESET, MP3_PLUS);
			break;
		case SUBMODE_SUB:
			play_mp3(MODE_FILESET, MP3_MINUS);
			break;
		case SUBMODE_MUL:
			play_mp3(MODE_FILESET, MP3_TIMES);
			break;
		default:
			sprintf(dbgstr, "[MD9] Error: md_submode: %c\n\r",
				md_submode);
			PRINTF(dbgstr);
			quit_mode();
			break;
	}
	play_number(md_op_2);
}

void md9_play_answer(void) {
	play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
	play_number(md_res);
}

void md9_main(void) {
	switch (md_next_state) {
		
		case STATE_MENU:
			md_last_dot = create_dialog(MP3_MENU,
				(DOT_1 | DOT_2 | DOT_3));
			switch (md_last_dot) {
				
				case NO_DOTS:
					break;

				case '1':
					PRINTF("[MD9] Submode: Addition\n\r");
					md_submode = SUBMODE_ADD;
					md_next_state = STATE_LVLSEL;
					break;

				case '2':
					PRINTF("[MD9] Submode: Subtraction\n\r");
					md_submode = SUBMODE_SUB;
					md_next_state = STATE_LVLSEL;
					break;

				case '3':
					PRINTF("[MD9] Submode: Multiplication\n\r");
					md_submode = SUBMODE_MUL;
					md_next_state = STATE_LVLSEL;
					break;

				default:
					PRINTF("[MD9] Error: Invalid dot: ");
					SENDBYTE(md_last_dot);
					NEWLINE;
					quit_mode();
					break;
			}
			break;

		case STATE_LVLSEL:
			md_last_dot = create_dialog(MP3_LVLSEL,
				(DOT_1 | DOT_2 | DOT_3));
			switch (md_last_dot) {
				
				case NO_DOTS:
					break;

				case '1':
					PRINTF("[MD9] Level: 1\n\r");
					md_level = LEVEL_1;
					play_mp3(MODE_FILESET, MP3_INSTRUCTIONS);
					md_next_state = STATE_GENQUES;
					break;

				case '2':
					PRINTF("[MD9] Level: 2\n\r");
					md_level = LEVEL_2;
					play_mp3(MODE_FILESET, MP3_INSTRUCTIONS);
					md_next_state = STATE_GENQUES;
					break;

				case '3':
					PRINTF("[MD9] Level: 3\n\r");
					md_level = LEVEL_3;
					play_mp3(MODE_FILESET, MP3_INSTRUCTIONS);
					md_next_state = STATE_GENQUES;
					break;

				default:
					PRINTF("[MD9] Error: md_last_dot: ");
					SENDBYTE(md_last_dot);
					NEWLINE;
					quit_mode();
					break;
			}
			break;

		case STATE_GENQUES:
			md9_generate_question();
			md_next_state = STATE_PROMPT;
			break;

		case STATE_PROMPT:
			md9_play_question();
			md_next_state = STATE_INPUT;
			break;

		case STATE_INPUT:
			if (io_user_abort == true) {
				PRINTF("[MD12] User aborted input\n\r");
				md_next_state = STATE_REPROMPT;
				io_init();
				break;
			}
			if (get_number(&md_input_valid, &md_usr_res)) {
				if (md_input_valid) {
					sprintf(dbgstr, "[MD9] User answer: %d\n\r", md_usr_res);
					PRINTF(dbgstr);
					play_mp3(MODE_FILESET, MP3_YOU_ANSWERED);
					play_number(md_usr_res);
					md_next_state = STATE_CHECKANS;
				} else {
					PRINTF("[MD9] IO error\n\r");
				}
			}
			break;

		case STATE_CHECKANS:
			if (md_usr_res == md_res) {
				// Correct answer
				md_incorrect_tries = 0;
				play_mp3(LANG_FILESET, MP3_CORRECT);
				play_mp3(SYS_FILESET, MP3_TADA);
				md_next_state = STATE_GENQUES;
			} else {
				// Wrong answer
				md_incorrect_tries++;
				play_mp3(LANG_FILESET, MP3_INCORRECT);
				if (md_incorrect_tries >= MAX_INCORRECT_TRIES) {
					md9_play_answer();
				}
				play_mp3(LANG_FILESET, MP3_TRY_AGAIN);
				md_next_state = STATE_PROMPT;
			}
			break;

		case STATE_REPROMPT:
				md_last_dot = create_dialog(MP3_SKIP,
					ENTER_CANCEL | LEFT_RIGHT);
				switch (md_last_dot) {
					
					case NO_DOTS:
						break;

					// Playing answer
					case RIGHT: case LEFT:
						md9_play_answer();
						break;

					// Skipping question
					case ENTER:
						md_next_state = STATE_GENQUES;
						break;

					// Try again
					case CANCEL:
						md_next_state = STATE_PROMPT;
						break;

					default:
						break;
				}
			break;

		default:
			PRINTF("[MD9] Error: next_state: ");
			SENDBYTE(md_next_state);
			NEWLINE;
			quit_mode();
			break;
	}
}

void md9_call_mode_yes_answer(void) {

}

void md9_call_mode_no_answer(void) {

}

void md9_call_mode_left(void) {

}

void md9_call_mode_right(void) {

}

void md9_input_dot(char this_dot) {

}

void md9_input_cell(char this_cell) {
		
}
