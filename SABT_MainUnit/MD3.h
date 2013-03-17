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
#define STATE_CHECK_IF_CORRECT  5          // Valid input was entered, checking to see if it is right
#define STATE_INVALID_INPUT     6          // User input is invalid (not a real letter)
#define STATE_WRONG_INPUT       7          // User input is valid but wrong
#define STATE_CORRECT_INPUT     8          // User input is correct
#define STATE_DONE_WITH_CURRENT_ANIMAL 9   // Finished current animal correctly

#define PRIME                53

char button_bits;                          // button bits
int current_state;                         // current state of the mode
char expected_dot;                         // The expected response from user 
char last_dot;                             // char representing last big dot pressed
int animals_used_list[11];                 // Int array representing the animals that have been used already.
int animals_used;                          // Keeps track of number of animals used in current rotation
char* animal;                              // Name of current animal
char entered_letter;                       // Current letter being entered
int length_current_word;                   // Keeps track of how long the current input word is
int current_word_index;                    // Used to read back the letters in the current word
const char* animal_list[11];               // Array of animal names (index into it to make code cleaner)

void MD3_Main(void);
void MD3_Reset(void);
void MD3_CallModeYesAnswer(void);
void MD3_CallModeNoAnswer(void);

void MD3_InputDot(char thisDot);
void MD3_InputCell(char thisCell);

#endif /* _MD3_H_ */
