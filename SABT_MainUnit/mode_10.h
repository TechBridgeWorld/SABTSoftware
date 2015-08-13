/**
 * @file mode_10.h
 * @brief definitions for mode 10
 * @author Poornima Kaniarasu (pkaniara)
 */

#ifndef _mode_10_H_
#define _mode_10_H_

// State definitions
#define mode_10_INITIAL                  0          // Just started, plays welcome message
#define mode_10_SELECT_MODE				1		   // Selecting the submode
#define mode_10_INIT				2		   // Submode initialisation
#define mode_10_REQUEST_INPUT            3          // Request for input from user - part 1
#define mode_10_CELL1					4		   // Waiting for user input - cell 1
#define mode_10_CELL2					5		   // Waiting for user input - cell 2   
#define mode_10_CHECK				    6		   // Checks if pattern is right
#define mode_10_REQUEST_WRITE			7		   // Requests the user to write in the pattern
#define mode_10_SPELL_PATTERN			8		   // Spells out the pattern for the user


char entered_letter;                       // Current letter being entered
// Keeps track of how long the user entered word is
int length_entered_word;                   
// Used to read back the letters in the current word
int current_word_index;
//bool to determine state transition
bool got_input;

//mode selected or not


void mode_10_main(void);
void mode_10_reset(void);
void mode_10_call_mode_yes_answer(void);
void mode_10_call_mode_no_answer(void);

void mode_10_input_dot(char thisDot);
void mode_10_input_cell(char this_cell);
void mode_10_call_mode_left(void);
void mode_10_call_mode_right(void);


#endif /* _mode_10_H_ */
