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
 #include "mp3s.h"

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
	log_msg("*** MD9 - Arithmetic practice ***\n\r");

	// Global variables
	set_mode_globals(&script_digits, NULL, NULL);

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
		log_msg("[MD9] Operand 1: %d\n\r", md_op_1);
		
		md_op_2 = timer_rand() % bound;
		log_msg("[MD9] Operand 2: %d\n\r", md_op_2);
		
		
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
				log_msg("[MD9] Error: md_submode: %c\n\r",
					md_submode);
				
				quit_mode();
				break;
		}
	} while (!(md_res > 0 && md_res < bound));
	log_msg("[MD9] Result: %d\n\r", md_res);
		
}

void md9_play_question() {
	play_direction(MP3_WRITE_NUMBER_);
	play_number(md_op_1);
	switch (md_submode) {
		case SUBMODE_ADD:
			play_direction(MP3_PLUS);
			break;
		case SUBMODE_SUB:
			play_direction(MP3_MINUS);
			break;
		case SUBMODE_MUL:
			play_direction(MP3_TIMES);
			break;
		default:
			log_msg("[MD9] Error: md_submode: %c\n\r",
				md_submode);
			
			quit_mode();
			break;
	}
	play_number(md_op_2);
}

void md9_play_answer(void) {
	play_feedback(MP3_THE_ANSWER_IS);
	play_number(md_res);
}

void md9_main(void) {
	switch (md_next_state) {
		
		case STATE_MENU:
			md_last_dot = create_dialog(MP3_CHOOSE_LEVELS_3,
				(DOT_1 | DOT_2 | DOT_3));
			switch (md_last_dot) {
				
				case NO_DOTS:
					break;

				case '1':
					log_msg("[MD9] Submode: Addition\n\r");
					md_submode = SUBMODE_ADD;
					md_next_state = STATE_LVLSEL;
					break;

				case '2':
					log_msg("[MD9] Submode: Subtraction\n\r");
					md_submode = SUBMODE_SUB;
					md_next_state = STATE_LVLSEL;
					break;

				case '3':
					log_msg("[MD9] Submode: Multiplication\n\r");
					md_submode = SUBMODE_MUL;
					md_next_state = STATE_LVLSEL;
					break;

				default:
					log_msg("[MD9] Error: Invalid dot: ");
					SENDBYTE(md_last_dot);
					NEWLINE;
					quit_mode();
					break;
			}
			break;

		case STATE_LVLSEL:
			md_last_dot = create_dialog(MP3_CHOOSE_LEVELS_3,
				(DOT_1 | DOT_2 | DOT_3));
			switch (md_last_dot) {
				
				case NO_DOTS:
					break;

				case '1':
					log_msg("[MD9] Level: 1\n\r");
					md_level = LEVEL_1;
					play_direction(MP3_INSTRUCTIONS_MATH);
					md_next_state = STATE_GENQUES;
					break;

				case '2':
					log_msg("[MD9] Level: 2\n\r");
					md_level = LEVEL_2;
					play_direction(MP3_INSTRUCTIONS_MATH);
					md_next_state = STATE_GENQUES;
					break;

				case '3':
					log_msg("[MD9] Level: 3\n\r");
					md_level = LEVEL_3;
					play_direction(MP3_INSTRUCTIONS_MATH);
					md_next_state = STATE_GENQUES;
					break;

				default:
					log_msg("[MD9] Error: md_last_dot: ");
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
				log_msg("[MD12] User aborted input\n\r");
                play_feedback(MP3_HELP_MENU);
				md_next_state = STATE_REPROMPT;
				io_init();
				break;
			}
			if (get_number(&md_input_valid, &md_usr_res)) {
				if (md_input_valid) {
					log_msg("[MD9] User answer: %d\n\r", md_usr_res);
					play_feedback(MP3_YOU_ANSWERED);
					play_number(md_usr_res);
					md_next_state = STATE_CHECKANS;
				} else {
					log_msg("[MD9] IO error\n\r");
				}
			}
			break;

		case STATE_CHECKANS:
			if (md_usr_res == md_res) {
				// Correct answer
				md_incorrect_tries = 0;
				play_feedback(MP3_CORRECT);
				play_tada();
				md_next_state = STATE_GENQUES;
			} else {
				// Wrong answer
				md_incorrect_tries++;
				play_feedback(MP3_CORRECT);
				if (md_incorrect_tries >= MAX_INCORRECT_TRIES) {
					md9_play_answer();
				}
				play_feedback(MP3_TRY_AGAIN);
				md_next_state = STATE_PROMPT;
			}
			break;

		case STATE_REPROMPT:
				md_last_dot = create_dialog(MP3_WORD_COMMANDS,
                                            ENTER_CANCEL | LEFT_RIGHT | DOT_1
                                            | DOT_2 | DOT_3 | DOT_4
                                            | DOT_5 | DOT_6);
				switch (md_last_dot) {
					
					case NO_DOTS:
						break;

					
					case LEFT:
                        md_next_state = STATE_PROMPT;
                        io_init();
						break;
                    
                    case RIGHT:
						md9_play_answer();
                        md_next_state = STATE_GENQUES;
                        io_init();
						break;

					// Skipping question
					case ENTER:
						md_next_state = STATE_GENQUES;
						break;

					// Try again
					case CANCEL:
						play_feedback(MP3_EXIT_HELP_MENU);
						play_feedback(MP3_RET_TO_QUESTION);
                        md_next_state = STATE_INPUT;
                        io_init();
                        break;

                        
                    case '1': case '2': case '3': case '4': case '5': case '6':
                    	play_feedback(MP3_WORD_COMMANDS);
                        io_init();
                        break;


					default:
						break;
				}
			break;

		default:
			log_msg("[MD9] Error: next_state: ");
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
