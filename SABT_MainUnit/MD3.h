/**
 * @file MD3.h
 * @brief definitions for mode 3
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD3_H_
#define _MD3_H_

// State definitions
#define STATE_INITIAL           0          // Just started, plays welcome message
#define STATE_REQUEST_INPUT1    1          // Request for input from user - part 1
#define STATE_REQUEST_INPUT2    2          // Request for input from user - part 2
#define STATE_WAIT_INPUT        3          // Waiting for user input
#define STATE_PROC_INPUT        4          // Processed user input
#define STATE_CHECK_IF_CORRECT  5          // Valid input was entered
#define STATE_INVALID_INPUT     6          // User input is invalid (not a real letter)
#define STATE_WRONG_INPUT       7          // User input is valid but wrong
#define STATE_CORRECT_INPUT     8          // User input is correct
#define STATE_DONE_WITH_CURRENT_ANIMAL 9   // Finished current animal correctly

#define PRIME                53

char last_dot;                             // char representing last big dot pressed
// Int array representing the animals that have been used already.
int animals_used_list[11];                 
// Keeps track of number of animals used in current rotation
int animals_used;                          
char* animal;                              // Name of current animal
char entered_letter;                       // Current letter being entered
// Keeps track of how long the current input word is
int length_current_word;                   
// Used to read back the letters in the current word
int current_word_index;
//bool to determine state transition
bool got_input;

void md3_main(void);
void md3_reset(void);
void md3_call_mode_yes_answer(void);
void md3_call_mode_no_answer(void);

void md3_input_dot(char thisDot);
void md3_input_cell(char this_cell);

#endif /* _MD3_H_ */
