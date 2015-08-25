/**
 * @file mode_11.h
 * @brief definitions for mode 11
 * @author Poornima Kaniarasu (pkaniara)
 */

#ifndef _mode_11_H_
#define _mode_11_H_

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


void mode_11_main(void);
void mode_11_reset(void);
void mode_11_call_mode_yes_answer(void);
void mode_11_call_mode_no_answer(void);

void mode_11_input_dot(char thisDot);
void mode_11_input_cell(char this_cell);
void mode_11_call_mode_left(void);
void mode_11_call_mode_right(void);


#endif /* _mode_11_H_ */
