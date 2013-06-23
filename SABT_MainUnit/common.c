/**
 * @file common.c
 * @brief Definitions for some common library functions
 * @author Vivek Nair
 */

#include "Globals.h"
#include "audio.h"

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

/**
 * @brief Quits the current mode and returns to the main menu
 * @param void
 * @return void
 */
void quit_mode(void) {
	ui_is_mode_selected = false;
  ui_current_mode_index = 0;
  ui_current_mode_number = ui_modes[ui_current_mode_index];
  play_mp3("","MM");
}
