/**
 * @file MD5.h
 * @brief definitions for mode 5
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD5_H_
#define _MD5_H_

// State definitions
#define MD5_STATE_INITIAL           0          // Just started, plays welcome message
#define MD5_STATE_SETUP_VARS        1          // Sets up variables
#define MD5_STATE_WAIT_INPUT_1      2          // Player 1 enters word
#define MD5_STATE_PROC_INPUT_1      3          // Check for valid input word
#define MD5_STATE_WAIT4P2           4          // Wait for transition to player2 to press enter
#define MD5_STATE_SAY_STATUS        5          // Read out current word
#define MD5_STATE_SAY_MISTAKES      6          // Read out current mistakes
#define MD5_STATE_ASK_FOR_GUESS     7          // Prompt user for input
#define MD5_STATE_WAIT_INPUT_2      8          // Waiting for user input
#define MD5_STATE_PROC_INPUT_2      9          // Process user input
#define MD5_STATE_CHECK_MATCH       10         // Valid input was entered
#define MD5_STATE_EVALUATE_GAME     11         // Check for win or lose
#define MD5_STATE_READ_WORD         12         // User has finished the game

#define PRIME                       53   
#define MAX_LEN                     10

char* item;
char player1_word[MAX_LEN + 1];       // Holds the current word being guessed
char input_word[MAX_LEN + 1];         // Holds the correctly input letters
int num_mistakes;                     // Keeps track of mistakes
int input_word_index;                 // Used to read out the status of the current input word
int game_status;

void md5_main(void);
void md5_reset(void);
void md5_call_mode_yes_answer(void);
void md5_call_mode_no_answer(void);

void md5_input_dot(char thisDot);
void md5_input_cell(char this_cell);

#endif /* _MD5_H_ */
