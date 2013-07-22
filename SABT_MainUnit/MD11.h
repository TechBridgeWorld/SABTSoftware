/**
 * @file MD11.h
 * @brief definitions for mode 11
 * @author Poornima Kaniarasu (pkaniara)
 */

#ifndef _MD11_H_
#define _MD11_H_

// State definitions
#define STATE_INITIAL                  0          // Just started, plays welcome message
#define STATE_REQUEST_INPUT1           1          // Request for input from user - part 1
#define STATE_REQUEST_INPUT2           2          // Request for input from user - part 2
#define STATE_WAIT_INPUT               3          // Waiting for user input
#define STATE_PROC_INPUT               4          // Processed user input
#define STATE_CHECK_IF_CORRECT         5          // Valid input was entered
#define STATE_READ_ENTERED_LETTERS     6          // User input is invalid (not a real letter)
#define STATE_WRONG_INPUT              7          // User input is valid but wrong
#define STATE_CORRECT_INPUT            8          // User input is correct
#define STATE_DONE_WITH_CURRENT_NOISE  9   // Finished current noise correctly
#define STATE_SELECT_MODE			   10 
#define STATE_PLAY_SOUND			   11
#define STATE_PROMPT				   12
#define STATE_LETTER_HINT			   13
#define STATE_WORD_HINT				   14
#define STATE_BUTTON_HINT			   15

#define PRIME                	53

// Int array representing the noises that have been used already.
int noises_used_list[11];                 
// Keeps track of number of noises used in current rotation
int noises_used;                          
char* noise;                              // Name of current noise
char entered_letter;                       // Current letter being entered
// Keeps track of how long the user entered word is
int length_entered_word;                   
// Used to read back the letters in the current word
int current_word_index;
//bool to determine state transition
bool got_input;
//stores the sub-mode the game is played in 

//mode selected or not


void md11_main(void);
void md11_reset(void);
void md11_call_mode_yes_answer(void);
void md11_call_mode_no_answer(void);

void md11_input_dot(char thisDot);
void md11_input_cell(char this_cell);
void md11_call_mode_left(void);
void md11_call_mode_right(void);


#endif /* _MD11_H_ */
