/**
 * @file mode_3.h
 * @brief definitions for mode 3
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MODE_3_H_
#define _MODE_3_H_
 
//static char last_dot;                             // char representing last big dot pressed
// Int array representing the animals that have been used already.
int animals_used_list[11];                 
// Keeps track of number of animals used in current rotation
int animals_used;                          
char* animal;                              // Name of current animal

//char entered_letter;                       // Current letter being entered
// Keeps track of how long the user entered word is
int length_entered_word;                   
// Used to read back the letters in the current word
int current_word_index;
//bool to determine state transition
bool got_input;
//stores the sub-mode the game is played in 

//mode selected or not


void mode_3_main(void);
void mode_3_reset(void);
void mode_3_call_mode_yes_answer(void);
void mode_3_call_mode_no_answer(void);

void mode_3_input_dot(char thisDot);
void mode_3_input_cell(char this_cell);
void mode_3_call_mode_left(void);
void mode_3_call_mode_right(void);


#endif
