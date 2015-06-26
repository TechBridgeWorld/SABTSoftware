/**
 * @file MD4.h
 * @brief Function declarations for mode 4 - One player Hangman
 * @author Kory Stiger (kstiger) Susan Zuo(tzuo)
 */

#ifndef _MD4_H_
#define _MD4_H_

void md4_main(void);
void md4_reset(void);
void md4_call_mode_yes_answer(void);
void md4_call_mode_no_answer(void);
void md4_call_mode_left(void);
void md4_call_mode_right(void);
void md4_input_dot(char this_dot);
void md4_input_cell(char this_cell);

#endif /* _MD4_H_ */