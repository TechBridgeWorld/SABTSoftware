/**
 * @file MD13.h
 * @brief Function declarations for mode 13 - Number Game
 * @author Susan Zuo(tzuo)
 */

#ifndef _MD13_H_
#define _MD13_H_

void md13_main(void);
void md13_reset(void);
void md13_call_mode_yes_answer(void);
void md13_call_mode_no_answer(void);
void md13_call_mode_left(void);
void md13_call_mode_right(void);
void md13_input_dot(char this_dot);
void md13_input_cell(char this_cell);

#endif /* _MD9_H_ */