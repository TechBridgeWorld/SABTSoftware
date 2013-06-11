/**
 * @file MD6.h
 * @brief Definitions for mode 6 - Free Play
 * @author Vivek Nair (viveknai)
 */

#ifndef _MD6_H_
#define _MD6_H_

void md6_main(void);
void md6_reset(void);
void md6_call_mode_yes_answer(void);
void md6_call_mode_no_answer(void);
void md6_input_dot(char this_dot);
void md6_input_cell(char this_cell);

#endif /* _MD6_H_ */
