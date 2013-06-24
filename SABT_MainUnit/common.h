/**
 * @file common.h
 * @brief Declarations for some common functions and constants
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _COMMON_H_
#define _COMMON_H_

//Primary interface board constants
#define ENTER			0b11111111
#define CANCEL		0b11111110
#define LEFT			0b11111101
#define RIGHT			0b11111100
#define VOL_UP		0b11111011
#define VOL_DOWN	0b11111010

//Used for cyclic traversal of lists
#define PLUS_ONE_MOD(X, BOUND) ((X + 1 > BOUND - 1) ? (0) : (X + 1))	
#define MINUS_ONE_MOD(X, BOUND) ((X - 1 < 0) ? (BOUND - 1) : (X - 1))

int timer_rand();
char add_dot(char bits, char dot);
void quit_mode(void);

#endif /* _COMMON_H_ */
