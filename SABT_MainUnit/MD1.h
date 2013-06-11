/**
 * @file MD1.h
 * @brief definitions for mode 1
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD1_H_
#define _MD1_H_

// State definitions
#define STATE_INITIAL        0   // Just started, plays welcome message
#define STATE_REQUEST_INPUT1 1   // Request for input from user - part 1
#define STATE_REQUEST_INPUT2 2   // Request for input from user - part 2
#define STATE_WAIT_INPUT     3   // Waiting for user input
#define STATE_PROC_INPUT     4   // Processed user input
#define NUM_DOTS			 6   //number of dots for each braille cell

#define PRIME                53

int current_state;               // current state of the mode
char expected_dot;               // The expected response from user 
static char last_dot;                   // char representing last big dot pressed
char used_num_cnt;

void md1_main(void);
void md1_reset(void);
void md1_call_mode_yes_answer(void);
void md1_call_mode_no_answer(void);

void md1_input_dot(char this_dot);
void md1_input_cell(char this_cell);

#endif /* _MD1_H_ */
