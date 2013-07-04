/**
 * @file MD7.h
 * @brief Header file for Mode 7 - Hindi Braille glyph practice
 * @authoer Vivek Nair (viveknai)
 */

#ifndef _MD7_H_
#define _MD7_H_

void md7_main(void);
void md7_reset(void);
void md7_call_mode_yes_answer(void);
void md7_call_mode_no_answer(void);
void md7_input_dot(char this_dot);
void md7_input_cell(char this_cell);
void md7_call_mode_left(void);
void md7_call_mode_right(void);

#endif /* _MD7_H_ */
