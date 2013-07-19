/**
 * @file MD10.h
 * @brief definitions for mode 10
 * @author Poornima Kaniarasu (pkaniara)
 */

#ifndef _MD10_H_
#define _MD10_H_

// State definitions
#define MD10_STATE_INITIAL                  0          // Just started, plays welcome message
#define MD10_STATE_SELECT_MODE				1		   // Selecting the submode
#define MD10_STATE_SUBMODE_INIT				2		   // Submode initialisation
#define MD10_STATE_REQUEST_INPUT            3          // Request for input from user - part 1
#define MD10_STATE_CELL1					4
#define MD10_STATE_WAIT_INPUT1              5           // Waiting for user input
#define MD10_STATE_CELL2					6
#define MD10_STATE_WAIT_INPUT2              7    
#define MD10_STATE_PROMPT				    8

#define PRIME                53
#define NUM_SUB_MODES		 3

char entered_letter;                       // Current letter being entered
// Keeps track of how long the user entered word is
int length_entered_word;                   
// Used to read back the letters in the current word
int current_word_index;
//bool to determine state transition
bool got_input;

//mode selected or not


void md10_main(void);
void md10_reset(void);
void md10_call_mode_yes_answer(void);
void md10_call_mode_no_answer(void);

void md10_input_dot(char thisDot);
void md10_input_cell(char this_cell);
void md10_call_mode_left(void);
void md10_call_mode_right(void);


#endif /* _MD10_H_ */
