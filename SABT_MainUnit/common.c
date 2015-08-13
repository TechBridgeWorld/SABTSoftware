/**
 * @file common.c
 * @brief Definitions for some common library functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include <stdbool.h>
#include "globals.h"
#include "audio.h"
#include "common.h"
#include "script_common.h"
#include "script_hindi.h"
#include "script_kannada.h"
#include "script_english.h"
#include "script_digits.h"
#include "datastructures.h"
#include "mode_15.h"

language_t set_language() {
  if (current_mode == 7) {
    mode_language = HINDI;
    lang_prefix = "h_";
    this_script = &script_hindi;
  }
  else if (current_mode == 12) {
    mode_language = KANNADA;
    lang_prefix = "k_";
    this_script = &script_kannada;
  }
  else {
    mode_language = ENGLISH;
    lang_prefix = "e_";
    if (current_mode == 8)
        this_script = &script_digits;
    else
        this_script = &script_english;
  }
  log_msg("Language: %s", lang_prefix);
  return mode_language;
}

 void set_mode_prefix(){
    vsprintf(mode_prefix, "m%s", &current_mode);
    log_msg("Mode prefix: %s", mode_prefix);
 }

 void reset_globals(){
    set_language();
    set_mode_prefix();
    last_dot = NO_DOTS;
    submode = SUBMODE_NULL;
    level = DIFFICULTY_NULL;
    cell = cell_pattern = cell_control = 0;
    current_state = INITIAL;
}

void reset_stats(){
    score = mistakes = curr_mistakes = 0;
}

/**
* @brief Sets script globals for a new language
* @param script_t* - Pointer to new script
* @param char* - Pointer to mode fileset string
* @return void
*/
void set_mode_globals(script_t* new_script, char* new_lang_prefix, char* new_mode_fileset) {
    lang_script = new_script;
    new_script->index = -1;
}

/**
 * @brief Generates a pseudo-random integer based on the system timer
 * @param void
 * @return int - Pseudo-random value
 */
int timer_rand(void) {
    #ifdef DEBUGMODE
        return rand();
    #else
        return TCNT1 * 53;
    #endif
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
    // @todo HACK TO FREE MEMORY MALLOCED IN MODE 15. RETEST THIS. 
    if (current_mode == 15) {
        log_msg("Freeing dictionary.");
        free_wordlist(&mode_15_dict);
    }
    is_a_mode_executing = false;
    index_of_current_mode = -1;
    play_system_audio(MP3_MM);
}

/**
* @brief Returns 10^n
* @param int n - Power
* @return int - 10^n
*/
long ten_to_the(int n) {
    int res = 1;
    if (n < 0)
        return 0;
    else {
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
int get_num_of_digits(long number) {
    if (number < 0)
        number = -number;
    int digits = 0;
    while (number != 0) {
        number = number / 10;
        digits++;
    }
    return digits;
}

/**
 * @brief  Given a char, in cell, play the corresponding number
 *         sound file
 *          NOTE: Deprecated
 * @return Void
 */

void play_requested_dot(char play_dot) {
    char req_mp3[10];
    sprintf((char*)req_mp3, "dot_%c", play_dot);
    play_mp3(NULL,req_mp3);
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
