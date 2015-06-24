/**
 * @file common.c
 * @brief Definitions for some common library functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "globals.h"
#include "audio.h"
#include "common.h"
#include "script_common.h"
#include "datastructures.h"

#include <stdbool.h>

/**
* @brief Sets script globals for a new language
* @param script_t* - Pointer to new script
* @param char* - Pointer to mode fileset string
* @return void
*/
void set_mode_globals(script_t* new_script, char* new_lang_fileset,
	char* new_mode_fileset) {
	lang_script = new_script;
	lang_fileset = new_lang_fileset;
	mode_fileset = new_mode_fileset;
	new_script->index = -1;
}

/**
 * @brief Generates a pseudo-random integer based on the system timer
 * @param void
 * @return int - Pseudo-random value
 */
int timer_rand(void) {
	return TCNT1 * 53;
}

/**
 * @brief Adds a dot to a bit pattern representing a cell
 * @param char bits - Old bit pattern
 * @param char dot - Dot to add
 * @return char - New bit pattern
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
  ui_current_mode_index = -1;
  play_mp3("SYS_","MM");
}

/**
* @brief Returns 10^n
* @param int n - Power
* @return int - 10^n
*/
int ten_to_the(int n) {
	int res = 1;
	if (n < 0) {
		return 0;
	} else {
		while (n > 0) {
			res *= 10;
			n--;

		}
		return res;
	}
}

/**
* @brief Gets number of digits
* @param int - Number to determine number of digits for
* @return int - Number of digits
*/
int get_num_of_digits(int number) {
	if (number < 0) {
		number = -number;
	}
	int digits = 0;
	while (number != 0) {
		number = number / 10;
		digits++;
	}
	return digits;
}

/**
 * @brief  Given a char, in last_cell, play the corresponding number
 *         sound file
 *			NOTE: Deprecated
 * @return Void
 */

void play_requested_dot(char play_dot)
{
  // This will hold formatted file to access
  char req_mp3[10];
  sprintf((char*)req_mp3, "dot_%c", play_dot);
  play_mp3(NULL,req_mp3);
}

/**
 * @brief Returns an integer between i and j-1
 * @param: i and j
 * return: a random number between them
 * (including i, not including j)
 */
int random_between(int i, int j) {
	int range = j - i;
	return i + (timer_rand() % range);
}

/**
* @brief Performs a Fisher-Yates shuffle on the
* an int array of length len.
* @param a length, an int array
* @return void
*/
void shuffle(int len, int* int_array) {
	int random_i, temp;
	for (int i = 0; i < len; i++) {
		random_i = random_between(i, len);
		temp = int_array[i];
		int_array[i] = int_array[random_i];
		int_array[random_i] = temp;
	}
}

/**
* @brief Uses selection sort to unshuffle
* an int array. Undoes shuffle.
* @param a length, an int array
* @return void
*/
void unshuffle(int len, int* int_array) {
	for (int i = 0; i < len; i++) {
		int min = i;
		for (int j = i+1; j < len; j++) {
			if (int_array[j] < int_array[min])
				min = j;
		}
		if (i != min) {
			int temp = int_array[i];
			int_array[i] = int_array[min];
			int_array[min] = temp;
		}
	}
}


/**
 * @brief Initialize an index array
 */
void init_index_arr(int *index_arr, int arr_len){
    for (int i = 0; i < arr_len; i++){
        index_arr[i] = i;
    }
}

void init_char_arr(char* arr, int len){
    for (int i = 0; i < len; i++){
        arr[i] = '\0';
    }
}


