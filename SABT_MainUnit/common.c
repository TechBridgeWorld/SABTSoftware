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
#include "datastructures.h"
#include "MD15.h"

 int get_current_mode(){
    return ui_current_mode_number;
 }

 lang_type set_language() {
  if (ui_current_mode_number == 7) {
    log_msg("Language: Hindi\n\r");
    ui_current_language = HINDI;
  }
  else if (ui_current_mode_number == 12) {
    log_msg("Language: Kannada\n\r");
    ui_current_language = KANNADA;
  }
  else {
    log_msg("Language: English\n\r");
    ui_current_language = ENGLISH;
  }
  return ui_current_language;
}

 char* get_lang_prefix(){
    switch (ui_current_language) {
        case HINDI:
            return "h_"; // @warning: in some cases the Hindi mode just uses English
        case KANNADA:
            return "k_";
        case ENGLISH:
            return "e_";
        default:
            log_msg("No language currently set.\n\r");
            return NULL;
    }
 }

 char* get_mode_prefix(){
    static char mode_prefix[4];
    mode_prefix[0] = 'M';
    mode_prefix[1] = get_current_mode();
    mode_prefix[2] = '_';
    mode_prefix[3] = '\n';
    return mode_prefix;
 }

 void reset_globals(){
    ui_current_language = set_language();
    lang_fileset = get_lang_prefix();
    last_dot = NO_DOTS;
    submode = level = 0;
    cell = cell_pattern = cell_control = 0;
    next_state = INITIAL;
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
void set_mode_globals(script_t* new_script, char* new_lang_fileset, char* new_mode_fileset) {
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
    if (ui_current_mode_number == 15) {
        log_msg("Freeing dictionary.");
        free_wordlist(&md15_dict);
    }
    ui_is_mode_selected = false;
    ui_current_mode_index = -1;
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
