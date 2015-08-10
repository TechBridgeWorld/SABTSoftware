/**
 * @file MD11.h
 * @brief definitions for mode 11
 * @author Poornima Kaniarasu (pkaniara)
 */

#ifndef _MD11_H_
#define _MD11_H_

// Int array representing the noises that have been used already.
int noises_used_list[11];
// Keeps track of number of noises used in current rotation
int noises_used;                          
char* noise;                              // Name of current noise

char entered_letter;                       // Current letter being entered  // @todo why does this mode have this if 3 doesn't?
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
