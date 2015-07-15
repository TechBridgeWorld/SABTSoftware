/**
 * @file MD5.h
 * @brief definitions for mode 5
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD5_H_
#define _MD5_H_

void md5_main(void);
void md5_reset(void);
void md5_call_mode_yes_answer(void);
void md5_call_mode_no_answer(void);

void md5_input_dot(char thisDot);
void md5_input_cell(char this_cell);

#endif /* _MD5_H_ */
