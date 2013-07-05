/**
 * @file MD9.c
 * @brief Code for mode 9 - Maths Practice
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "common.h"
#include "globals.h"
#include "audio.h"

#define MD9_STATE_NULL 0x00
#define MD9_STATE_MENU 0x01
#define MD9_STATE_LVLSEL 0x02
#define MD9_STATE_GENQUES 0x03
#define MD9_STATE_PROMPT 0x04
#define MD9_STATE_INPUT 0x05

// Submodes for different arithmetic operations
#define MD9_SUBMODE_NUL 0x00
#define MD9_SUBMODE_ADD 0x01
#define MD9_SUBMODE_SUB	0x02
#define MD9_SUBMODE_MUL 0x03

// Difficulty levels
#define MD9_LEVEL_NULL	0x0000
#define MD9_LEVEL_1			0x0001
#define MD9_LEVEL_2			0x0002
#define MD9_LEVEL_3			0x0003

// Used to set global fileset variables
#define MD9_LANG_FILESET "ENG_"
#define MD9_MODE_FILESET "MD9_"

// Prompt files
// Submode menu prompt
#define MP3_MENU	"MENU" //Menu
// Level select prompt
#define MP3_LVLSEL "LVSL" //Level select
// Pre-question prompt
#define MP3_PREQUES "WHIS" //What is
// In-question operators
#define MP3_PLUS	"PLUS"
#define MP3_MINUS	"MINS"
#define MP3_TIMES	"TIMS"

// Limits
#define MAX_DIGITS 3

static script_t script_common;

// State variables
static char md_next_state = MD9_STATE_NULL;
static char md_submode = MD9_SUBMODE_NUL;
static char md_last_dot = NO_DOTS;
static int md_level = 0;
static int md_op_1 = -1;
static int md_op_2 = -1;
static int md_res = -1;
static int md_usr_res = -1;

void md9_reset(void) {
	PRINTF("*** MD9 - Arithmetic practice ***\n\r");

	// Global variables
	set_mode_globals(&script_common, MD9_LANG_FILESET, MD9_MODE_FILESET);

	// State variables
	md_next_state = MD9_STATE_MENU;
	md_submode = MD9_SUBMODE_NUL;
	md_last_dot = NO_DOTS;
	md_level = MD9_LEVEL_NULL;
	md_op_1 = -1;
	md_op_2 = -1;
	md_res = -1;
	md_usr_res = -1;
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
			case MD9_SUBMODE_ADD:
				md_res = md_op_1 + md_op_2;
				break;
			case MD9_SUBMODE_SUB:
				md_res = md_op_1 - md_op_2;
				break;
			case MD9_SUBMODE_MUL:
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
	play_mp3(mode_fileset, MP3_PREQUES);
	play_number(md_op_1);
	switch (md_submode) {
		case MD9_SUBMODE_ADD:
			play_mp3(mode_fileset, MP3_PLUS);
			break;
		case MD9_SUBMODE_SUB:
			play_mp3(mode_fileset, MP3_MINUS);
			break;
		case MD9_SUBMODE_MUL:
			play_mp3(mode_fileset, MP3_TIMES);
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

void md9_main(void) {
	switch (md_next_state) {
		
		case MD9_STATE_MENU:
			md_last_dot = create_dialog(MP3_MENU,
				(DOT_1 | DOT_2 | DOT_3));
			switch (md_last_dot) {
				
				case NO_DOTS:
					break;

				case '1':
					PRINTF("[MD9] Submode: Addition\n\r");
					md_submode = MD9_SUBMODE_ADD;
					md_next_state = MD9_STATE_LVLSEL;
					break;

				case '2':
					PRINTF("[MD9] Submode: Subtraction\n\r");
					md_submode = MD9_SUBMODE_SUB;
					md_next_state = MD9_STATE_LVLSEL;
					break;

				case '3':
					PRINTF("[MD9] Submode: Multiplication\n\r");
					md_submode = MD9_SUBMODE_MUL;
					md_next_state = MD9_STATE_LVLSEL;
					break;

				default:
					PRINTF("[MD9] Error: Invalid dot: ");
					SENDBYTE(md_last_dot);
					NEWLINE;
					quit_mode();
					break;
			}
			break;

		case MD9_STATE_LVLSEL:
			md_last_dot = create_dialog(MP3_LVLSEL,
				(DOT_1 | DOT_2 | DOT_3));
			switch (md_last_dot) {
				
				case NO_DOTS:
					break;

				case '1':
					PRINTF("[MD9] Level: 1\n\r");
					md_level = MD9_LEVEL_1;
					md_next_state = MD9_STATE_GENQUES;
					break;

				case '2':
					PRINTF("[MD9] Level: 2\n\r");
					md_level = MD9_LEVEL_2;
					md_next_state = MD9_STATE_GENQUES;
					break;

				case '3':
					PRINTF("[MD9] Level: 3\n\r");
					md_level = MD9_LEVEL_3;
					md_next_state = MD9_STATE_GENQUES;
					break;

				default:
					PRINTF("[MD9] Error: md_last_dot: ");
					SENDBYTE(md_last_dot);
					NEWLINE;
					quit_mode();
					break;
			}
			break;

		case MD9_STATE_GENQUES:
			md9_generate_question();
			md_next_state = MD9_STATE_PROMPT;
			break;

		case MD9_STATE_PROMPT:
			md9_play_question();
			md_next_state = MD9_STATE_INPUT;
			break;

		case MD9_STATE_INPUT:
			if (get_line() == true) {
				PRINTF(io_line);
				NEWLINE;
				md_next_state = MD9_STATE_GENQUES;
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
