/**
 * @file MD4.h
 * @brief definitions for mode 4
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD4_H_
#define _MD4_H_

// State definitions
#define MD4_STATE_INITIAL           0          // Just started, plays welcome message
#define MD4_STATE_CHOOSE_WORD       1          // Choose random word and intialize globals
#define MD4_STATE_SAY_STATUS        2          // Read out current word
#define MD4_STATE_SAY_MISTAKES      3          // Read out current mistakes
#define MD4_STATE_ASK_FOR_INPUT     4          // Prompt user for input
#define MD4_STATE_WAIT_INPUT        5          // Waiting for user input
#define MD4_STATE_PROC_INPUT        6          // Process user input
#define MD4_STATE_CHECK_MATCH       7          // Valid input was entered
#define MD4_STATE_EVALUATE_GAME     8          // Check for win or lose
#define MD4_STATE_READ_WORD         9          // User has finished the game

#define PRIME                   53   

int items_used_list[11];
int items_used;
char* item;
char *current_word;      // Holds the current word being guessed
char input_word[11];     // Holds the correctly input letters
int num_mistakes;        // Keeps track of mistakes
int input_word_index;    // Used to read out the status of the current input word
int game_status;

void md4_main(void);
void md4_reset(void);
void md4_call_mode_yes_answer(void);
void md4_call_mode_no_answer(void);

void md4_input_dot(char thisDot);
void md4_input_cell(char this_cell);

#endif /* _MD4_H_ */
