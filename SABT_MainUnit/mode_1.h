/**
 * @file mode_1.h
 * @brief definitions for mode 1
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _mode_1_H_
#define _mode_1_H_


char expected_dot;               // The expected response from user 
char used_num_cnt;

void mode_1_main(void);
void mode_1_reset(void);
void mode_1_call_mode_yes_answer(void);
void mode_1_call_mode_no_answer(void);

void mode_1_input_dot(char this_dot);
void mode_1_input_cell(char this_cell);

#endif /* _mode_1_H_ */
