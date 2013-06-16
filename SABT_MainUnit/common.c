/**
 * @file common.c
 * @brief Definitions for some common library functions
 * @author Vivek Nair
 */

#include "Globals.h"

/**
 * @brief Generates a random integer based on the system timer
 * @param None
 * @return int - Random value
 */
int timer_rand() {
	return TCNT1 * 53;
}

/**
 * @brief Adds a dot to a bit pattern representing a cell
 * @param char bits - old bit pattern; char dot - dot to add
 * @return void
 */
char add_dot(char bits, char dot) {
	return (bits | 1 << (CHARTOINT(dot) - 1));
}
