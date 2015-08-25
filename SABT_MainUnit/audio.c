/**
 * @file audio.c
 * @brief Audio library
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vs1053.h"
#include "datastructures.h"
#include "globals.h"
#include "common.h"
#include "io.h"
#include "script_common.h"
#include "fat32.h"
#include "mp3s.h"
#include "audio.h"

/* Maximum number of MP3s that can be queued at a given time */
#define MAX_PLAYLIST_SIZE 64

/** Number parsers; macro refers to number's position away from decimal point */
#define PLACE_ONES 0x01
#define PLACE_TENS 0x02
#define PLACE_HUNDREDS 0x03
#define PLACE_THOUSANDS 0x04
#define PLACE_TEN_THOUSANDS 0x05
// ints on this platform are 2 bytes long - largest signed int is 32,768

bool playlist_empty = true;

/** Playlist supports queuing some files at a time with filenames of up to 13
    characters */
static char playlist[MAX_PLAYLIST_SIZE][MAX_FILENAME_SIZE];
static short playlist_size = 0;
static short playlist_index = 0;

/**
 *  @brief Tries to queue the requested MP3 file to the playlist
 *  @param char* fileset - (optional) Pointer to fileset
 *  @param char* mp3 - Pointer to MP3 filename
 *  @return bool - True if file was added, false if queue is full or error
 */

bool play_mp3(char* fileset, char* mp3) {
    if (mp3 == NULL) {
        log_msg("[Audio] Error: Cannot play NULL MP3");
        return false;
    }

    //Return false if playlist is full
    if (playlist_size == MAX_PLAYLIST_SIZE) {
        log_msg("[Audio] Playlist full");
        return false;
    }
    
    playlist_size++;

    //Truncate to 8 chars if the string is longer than that
    if (strlen(mp3) > 8)
        mp3[8] = '\0';

    // add file to playlist
    if (fileset != NULL)
        sprintf(playlist[playlist_size - 1], "%s%s.mp3", fileset, mp3);
    else
        sprintf(playlist[playlist_size - 1], "%s.mp3", mp3);

    playlist_empty = false;
    return true;
}


/**
* @brief Clears MP3 playlist
* @param void
* @return void
*/
void clear_playlist(void) {
    playlist_empty = true;
    playlist_size = 0;
    playlist_index = 0;
}

/**
 * @brief Plays next queued MP3 file. Only called when queue is not empty.
 *      is called repeatedly till queue is empty
 * @param void
 * @return void
 */
void play_next_mp3(void) {
    //Only called when the playlist is not empty
    if (playlist_empty == true) {
        log_msg("[Audio] Error: Playlist empty");
        return;
    }

    log_msg("[Audio] Playing: %s", playlist[playlist_index]);
    #ifndef DEBUGMODE
    play_mp3_file((unsigned char*)playlist[playlist_index]);
    #endif
    playlist_index++;

    //If playlist is now empty, reset variables 
    if (playlist_index == playlist_size)
        clear_playlist();
}


//////// @todo: eliminate these

/**
 * @brief Play sound file corresponding to an glyph, checks for NULL arg
 * @param glyph_t* this_glyph - Pointer to glyph to play
 * @return void
 */
void play_glyph(glyph_t *this_glyph) {
    if (this_glyph != NULL)
        play_lang_audio(this_glyph->sound);
}

/**
 * @brief Play sound file corresponding to an word, checks for NULL arg
 * @param word_node_t* this_word - Pointer to word to play
 * @return void
 */
void play_word(word_node_t *this_word) {
    while (this_word != NULL) {
        play_glyph(this_word->data);
        this_word = this_word->next;
    }
}



/**
 * @brief Plays the dot sequence for a given bit pattern
 * @param char pattern - Bit pattern to play
 * @return void
 */
void play_cell(cell_t pattern) {
    if (pattern == 0) {
        play_lang_audio(MP3_BLANK); // won't work for Hindi?
        return;
    }

    // iterate through the six dots in the cell/six bools in the bitfield
    char dot[2];
    for (int i = 0; pattern != 0; i++, pattern = pattern >> 1) {
        if (pattern & 0x01) {
            #ifdef DEBUGMODE
                sprintf(dot, "%d", i+1);
                play_dot(dot[0]);
            #else
                play_dot(itoa((i+1), dot, 10)[0]);
            #endif
        }
    }
}

/**
 * @brief Play dot sequence corresponding to a glyph, checks for NULL arg
 * @param glyph_t *this_glyph - glyph to play dot sequence for
 * @return void
 */
void play_dot_sequence(glyph_t *this_glyph) {
    char pattern; 
    if (this_glyph != NULL) {
        pattern = this_glyph->pattern;
        play_cell(pattern);

        // for multi-cell letters
        if (this_glyph->next != NULL) {
            // Plays all the next glyphs in the linked list
            log_msg("[Audio] Playing next pattern: %s",
                this_glyph->next->sound);
            // play "ENTER" so user knows to press enter btwn multiple-cell letters
            play_lang_audio(MP3_DOT_E);
            play_dot_sequence(this_glyph->next);
        }
    } else
        play_lang_audio(MP3_INVALID_PATTERN);
}

/** 
    @brief Plays number
    @param int number - Number to be played. Must be under between -9,999 and
    9,999
    @return void
*/
void play_number(long number) {
    int curr_digit = -1;
    int digits = -1;
    char mp3[5] = "";
    bool played_and = false;


    // If number is just 0, play #0 and return
    if (number == 0) {
        play_lang_audio(MP3_ZERO);
        return;
    }

    if (number < 0) {
        // Say "Negative" and take absolute value
        play_lang_audio(MP3_NEGATIVE);
        number = 0 - number;
    }

    // Count number of digits
    digits = get_num_of_digits(number);
        
    while (number != 0) {
        // Extract current digit and adjust number
        curr_digit = number / ten_to_the(digits - 1);

        if (curr_digit != 0) {
            sprintf(mp3, "#%d", curr_digit);
            played_and = false;

            switch (digits) {
                case PLACE_ONES:
                    play_lang_audio(mp3);
                    break;

                case PLACE_TENS:
                    if (curr_digit == 1) {
                        // If teen, play teen and return immediately
                        curr_digit = number;
                        sprintf(mp3, "#%d", curr_digit);
                        play_lang_audio(mp3);
                        return;
                    } else {
                        // Is a multiple of ten
                        sprintf(mp3, "#%d0", curr_digit);
                        play_lang_audio(mp3);
                    }
                    break;

                case PLACE_HUNDREDS:
                    play_lang_audio(mp3);
                    play_lang_audio(MP3_HUNDRED);
                    break;
                
                case PLACE_THOUSANDS:
                    play_lang_audio(mp3);
                    play_lang_audio(MP3_THOUSAND);
                    break;
                    
                case PLACE_TEN_THOUSANDS:
                    if (curr_digit == 1) {  // Play teen and return immediately
                        curr_digit = number / ten_to_the(digits - 2);
                        number -= curr_digit * ten_to_the(digits - 2);
                        digits -= 2;
                        sprintf(mp3, "#%d", curr_digit);
                        play_lang_audio(mp3);
                    }
                    else {
                        // Is a multiple of ten
                        sprintf(mp3, "#%d0", curr_digit);
                        play_lang_audio(mp3);
                        number -= curr_digit * ten_to_the(digits - 1);
                        digits--;
                        curr_digit = number / ten_to_the(digits - 1);
                        sprintf(mp3, "#%d", curr_digit);
                        play_lang_audio(mp3);
                    }
                    play_lang_audio(MP3_THOUSAND);
                    break;
                    
                default:
                    log_msg("[Audio] Error: Number greater than 5 digits");
                    quit_mode();
            }
        }
        else {
            if (!played_and) {
                play_feedback(MP3_AND);
                played_and = true;
            }
        }
        number -= curr_digit * ten_to_the(digits - 1);  
        digits--;
    }
}

/**
* @brief Plays a NULL-terminated array of glyphs
* @param glyph_t* - Pointer to line
* @return void
*/
void play_line(glyph_t** line) {
    glyph_t* curr_glyph = NULL;
    for (int i = 0; i < MAX_BUF_SIZE; i++) {
        curr_glyph = line[i];
        if (curr_glyph)
            play_glyph(curr_glyph);
        else
            return;
    }
}

void play_string(char* word, int word_len){
    for (int i = 0; i < word_len; i++){
        if (word[i] != '\0') {
            char buf[10];
            sprintf(buf, "%c", word[i]);
            play_mp3(MP3_ENGLISH, buf);
        }
        else
            play_mp3(MP3_ENGLISH, MP3_BLANK);
    }
}

/*
* @brief Higher-level functions for modes to call to
* play different kinds of audio files to users.
*/

// Plays mode-specific audio (m#_<dir>.mp3)
void play_mode_audio(char* dir) {
    char md[5];
    sprintf(md, "m%d_", current_mode);
    play_mp3(md, dir);
}

// Plays mode-specific audio (m#_<dir>.mp3)
void play_lang_audio(char* dir) {
    char md[3];
    switch (mode_language) {
        case HINDI:
            sprintf(md, "h_");
            break;
        case KANNADA:
            sprintf(md, "k_");
            break;
        case ENGLISH:
        default:
            sprintf(md, "e_");
            break;
    }
    play_mp3(md, dir);
}

// Plays the requested system audio (s_<dir>.mp3)
void play_system_audio(char* dir) {
    play_mp3(MP3_SYSTEM, dir);
}

// Plays the mode's welcome mp3 (m#_welc.mp3)
void play_welcome() {
    log_msg("Welcome to mode %d", current_mode);
    char md[5];
    sprintf(md, "m%d_", current_mode);
    play_mp3(md, MP3_WELCOME);
}

// Plays the mode's submode choice mp3 (m#_subm.mp3)
void play_submode_choice() {
    log_msg("Playing submode choice.");
    char md[5];
    sprintf(md, "m%d_", current_mode);
    play_mp3(md, MP3_SUBMODE);
}

// Plays the requested directions (d_<dir>.mp3)
void play_direction(char* dir) {
    play_mp3(MP3_DIRECTIONS, dir);
}

// Plays the requested feedback (f_<dir>.mp3)
void play_feedback(char* dir) {
    play_mp3(MP3_FEEDBACK, dir);
}

// Plays the tada noise (s_tada.mp3)
void play_tada() {
    play_mp3(MP3_SYSTEM, MP3_TADA);
}

/**
 * @brief Plays a specified amount of slience
 * @param int milliseconds - Length of slience in milliseconds
 *   currently can be 250, 500, 750 or 1000
 * @return void
 */
void play_silence(int milliseconds) {
    switch (milliseconds) {
        case 250:
            play_system_audio("s025");
            break;
        case 500:
            play_system_audio("s050");
            break;
        case 750:
            play_system_audio("s075");
            break;
        case 1000:
            play_system_audio("s100");
            break;
        default:
            break;
    }
}

/**
 * @brief Queues dot sound file
 * @param char* fileset - Language fileset for dot numbers
 * @param char dot - Dot to play
 * @return void
 */
void play_dot(char dot) {
    char dotname[6];
    switch (dot) {
        case '1': case '2': case '3': case '4': case '5': case '6':
            break;
        case ENTER:
            dot = 'e';
            break;
        case CANCEL:
            dot = 'c';
            break;
        case LEFT: case RIGHT: case NO_DOTS:
            return;
        default:
            log_msg("[Audio] Invalid dot: %c", dot);
            break;
    }
    sprintf(dotname, "dot_%c", dot);
    play_lang_audio(dotname);
}