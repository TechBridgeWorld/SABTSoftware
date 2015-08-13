/**
 * @file mode_4.h
 * @brief Function declarations for mode 4 - One player Hangman
 * @author Kory Stiger (kstiger) Susan Zuo(tzuo)
 */

#ifndef _mode_4_H_
#define _mode_4_H_

void mode_4_main(void);
void mode_4_reset(void);
void mode_4_call_mode_yes_answer(void);
void mode_4_call_mode_no_answer(void);
void mode_4_call_mode_left(void);
void mode_4_call_mode_right(void);
void mode_4_input_dot(char this_dot);
void mode_4_input_cell(char this_cell);

#endif /* _mode_4_H_ */