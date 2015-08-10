/**
 * @file MD1.h
 * @brief definitions for mode 1
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD1_H_
#define _MD1_H_


char expected_dot;               // The expected response from user 
char used_num_cnt;

void md1_main(void);
void md1_reset(void);
void md1_call_mode_yes_answer(void);
void md1_call_mode_no_answer(void);

void md1_input_dot(char this_dot);
void md1_input_cell(char this_cell);

#endif /* _MD1_H_ */
