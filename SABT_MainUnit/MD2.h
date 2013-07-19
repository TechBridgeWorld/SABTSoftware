/**
 * @file md2.h
 * @brief definitions for mode 2
 * @author Nick LaGrow
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD2_H_
#define _MD2_H_

//Enter - just needs to be more then top cell value
// This is arbitrary

#include "script_common.h"

#define NUM_BUT 6

#define LET_CLST_SIZE 5


// State definitions
#define STATE_INITIAL         0   // Just started, plays welcome message
#define STATE_REQUEST_INPUT_1 1   // Request for input from user - part 1
#define STATE_REQUEST_INPUT_2 2   // Request for input from user - part 2
#define STATE_WAIT_INPUT      3   // Waiting for user input
#define STATE_PROC_INPUT      4   // Processed user input
#define STATE_ERROR_1         5   // first part of the error sequence
#define STATE_ERROR_2         6   // second part of the error sequence
#define SET_LETTER_VALS	      7   // Set the letter vals for random and current
#define STATE_BUTT_TO_PRESS_1 8   //play press sound before telling which buttons to press
#define STATE_BUTT_TO_PRESS_2 9   //play each of the buttons requested 


char letter_set;
char random_count;
char current_count;
char initial_letter;
char current_letter;
char current_random_letter;
char use_random_letter;
char curr_button;
char used_let_cnt;

char get_bits_from_letters(char let);

void md2_main(void);
void md2_reset(void);
void md2_call_mode_yes_answer(void);
void md2_call_mode_no_answer(void);

void md2_input_dot(char this_dot);
void md2_input_cell(char this_cell);

#endif /* _MD2_H_ */
