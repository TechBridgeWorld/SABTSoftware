/**
 * @file common.h
 * @brief Definitions for some common library functions
 * @author Vivek Nair
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#define PLUS_ONE_MOD(X, BOUND) ((X + 1 > BOUND - 1) ? (0) : (X + 1))	
#define MINUS_ONE_MOD(X, BOUND) ((X - 1 < 0) ? (BOUND - 1) : (X - 1))

int timer_rand();
char add_dot(char bits, char dot);

#endif /* _COMMON_H_ */
