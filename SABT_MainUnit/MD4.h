/**
 * @file MD4.h
 * @brief definitions for mode 4
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD4_H_
#define _MD4_H_

// State definitions
#define STATE_INITIAL           0          // Just started, plays welcome message
#define STATE_CHOOSE_WORD       1          // Choose random word and intialize globals
#define STATE_SAY_STATUS        2          // Read out current word and mistakes
#define STATE_WAIT_INPUT        3          // Waiting for user input
#define STATE_PROC_INPUT        4          // Process user input
#define STATE_CHECK_MATCH       5          // Valid input was entered

#define PRIME                53      

void md4_main(void);
void md4_reset(void);
void md4_call_mode_yes_answer(void);
void md4_call_mode_no_answer(void);

void md4_input_dot(char thisDot);
void md4_input_cell(char this_cell);

#endif /* _MD4_H_ */
