/*
 * @file sound_game_mode.c
 *
 * @brief Shell for "sound game modes" (MD 3,11)
 * @author: Edward Cai
 */ 

// Standard libraries
#include <stdbool.h>
#include <stdio.h>

// Custom libraries
#include "globals.h"
#include "Modes.h"
#include "audio.h"
#include "datastructures.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h" 
#include "sound_game_mode.h"

// Int array representing the animals that have been used already.
static int sound_sources_used_list[11];
// Keeps track of number of animals used in current rotation
static int sound_sources_used;
static char* sound_source;     // Name of current animal
// Keeps track of how long the user entered word is
int length_entered_word;
// Used to read back the letters in the current word
int current_word_index;
static char submode = SUBMODE_NULL;
static char next_state = STATE_NULL;
static int incorrect_tries = 0;
static char cell = 0;
static char cell_pattern = 0;
static char cell_control = 0;
static bool scrolled = false;

static glyph_t* user_glyph = NULL;      // inputted by user
static glyph_t* curr_glyph = NULL;      // next letter in sound_source
static word_node_t* user_word = NULL;   // word as it is being inputted
static word_node_t* curr_word = NULL;   // entire word
static script_t* this_script = &script_english;


char **sound_source_list;
char **sound_list;

/*
* @brief Plays sound_source sounds
* @param char* sound_source - sound_source name
* @param bool name - true if name requeste, false if sound requested
* @return void
*/
void play_sound(char* MODE_FILESET, char* sound_source, bool name) {
    char filename[5];
    for (int i = 0; i < MAX_INDEX; i++) {
        if (strcmp(sound_source, sound_source_list[i]) == 0) {
            // Match found
            if (name)
                sprintf(filename, "n_%s", sound_list[i]);
            else
                sprintf(filename, "s_%s", sound_list[i]);
            play_mp3(MODE_FILESET, filename);
        }
    }
}


/**
 * @brief Based off of the internal timer (TCNT1) - we generate
 *        a psuedo-random number. Turn that into a number from 1 - MAX_INDEX
 *        which corresponds to 1 of MAX_INDEX different sound_sources. Check which
 *        sound_sources have been played already to be sure to play all of
 *        the different sound_sources before repeating the list.
 * @return int - number between 1 - MAX_INDEX corresponding to the sound_source file to play
 */
int choose_sound_source() {
    int num = timer_rand() % MAX_INDEX;
    int i;

    while (sound_sources_used_list[num])
        num = timer_rand() % MAX_INDEX;

    sound_sources_used_list[num] = 1;

    for (i = 0; i < MAX_INDEX; i ++)
        log_msg("arr = %i, ", sound_sources_used_list[i]);
    log_msg("cnt = %i", sound_sources_used +1);

    // increment sound_sources_used until we've used all MAX_INDEX sound_sources then reset everything
    sound_sources_used++;
    if (sound_sources_used == MAX_INDEX) {
        sound_sources_used = 0;
        for (i = 0; i < MAX_INDEX; i++)
            sound_sources_used_list[i] = 0;
    }
    return num;
}

void sound_game_reset(script_t* SCRIPT_ADDRESS, char* LANG_FILESET, char* MODE_FILESET, 
                      const char** SOUND_SOURCE_LIST, const char** SOUND_LIST) {
    set_mode_globals(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
    sound_list = (char**) SOUND_LIST;
    sound_source_list = (char**) SOUND_SOURCE_LIST;
    next_state = STATE_MENU;
    submode = SUBMODE_NULL;
    incorrect_tries = 0;
    user_glyph = NULL;
    curr_glyph = NULL;
    user_word = free_word_old(user_word);
    curr_word = free_word_old(curr_word);
    cell = 0;
    cell_pattern = 0;
    cell_control = 0;
    scrolled = false;
    log_msg("[Sound game] Mode reset");
    play_welcome();
    play_submode_choice();
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void sound_game_main(script_t* SCRIPT_ADDRESS, char* LANG_FILESET, char* MODE_FILESET) {
    switch(next_state) {

        case STATE_MENU:
            if (io_user_abort == true) {
                log_msg("[%s] Quitting to main menu", MODE_FILESET);
                quit_mode();
                io_init();
            }
            switch(create_dialog("", DOT_1 | DOT_2 | ENTER_CANCEL)) {

                case '1':
                    log_msg("[%s] Submode: Learn", MODE_FILESET);
                    submode = SUBMODE_LEARN;
                    next_state = STATE_GENQUES;
                    break;

                case '2':
                    log_msg("[%s] Submode: Play", MODE_FILESET);
                    submode = SUBMODE_PLAY;
                    next_state = STATE_GENQUES;


                case WITH_LEFT:
                case WITH_RIGHT:
                case WITH_CANCEL:
                default:
                    break;
            }
            break;



        
        case STATE_GENQUES:
            length_entered_word = 0;
            current_word_index = 0;
            sound_source = sound_source_list[choose_sound_source()];
            user_glyph = NULL;
            curr_glyph = NULL;
            user_word = free_word_old(user_word);
            curr_word = free_word_old(curr_word);
            next_state = STATE_PROMPT;
            break;
            


        case STATE_PROMPT:
            switch(submode) {
                case SUBMODE_PLAY:
                    play_mp3(MODE_FILESET, "PLSA");
                    play_sound(MODE_FILESET, sound_source, false);
                    break;
            
                case SUBMODE_LEARN:
                    play_mp3(MODE_FILESET, "PLSB");
                    play_sound(MODE_FILESET, sound_source, true);
                    break;
                
                default:
                break;  
            }
            next_state = STATE_INPUT;
            break;
        


        case STATE_INPUT:
            if (io_user_abort == true) {
                log_msg("[%s] User aborted input", MODE_FILESET);
                next_state = STATE_REPROMPT;
                io_init();
            }
            cell = get_cell();
            if (cell == NO_DOTS)
                break;
            cell_pattern = GET_CELL_PATTERN(cell);
            cell_control = GET_CELL_CONTROL(cell);
            switch (cell_control) {
                case WITH_ENTER:
                    user_glyph = search_script(this_script, cell_pattern);
                    next_state = STATE_CHECK;
                    log_msg("[%s] Checking answer", MODE_FILESET);
                    break;
                case WITH_LEFT:
                    next_state = STATE_PROMPT;
                    break;
                case WITH_RIGHT:
                    next_state = STATE_REPROMPT;
                    break;
                case WITH_CANCEL:
                    break;
            }
            break;

        case STATE_CHECK:
            curr_glyph = search_script(this_script, get_bits_from_letter(sound_source[length_entered_word]));
            if (glyph_equals(curr_glyph, user_glyph)) { // correct input
                play_glyph(curr_glyph);
                incorrect_tries = 0;
                length_entered_word++;
                user_word = add_glyph_to_word(user_word, user_glyph);
                if (length_entered_word != strlen(sound_source)) { // not done with word
                    play_feedback(MP3_GOOD);
                    next_state = STATE_INPUT;
                }
                else { // finished word
                    play_feedback(MP3_GOOD);
                    play_feedback(MP3_NICE_WORK);
                    switch (submode){
                        case SUBMODE_LEARN:
                            play_sound(MODE_FILESET, sound_source, true);
                            play_direction(MP3_SAYS);
                            play_sound(MODE_FILESET, sound_source, false);
                            break;
                        
                        case SUBMODE_PLAY:
                            play_word(user_word);
                            play_sound(MODE_FILESET, sound_source, true);
                    }
                    next_state = STATE_GENQUES;
                }
            }
            else { // incorrect input
                play_glyph(user_glyph);
                incorrect_tries++;
                log_msg("[%s] User answered incorrectly", MODE_FILESET);
                play_feedback(MP3_NO);
                play_feedback(MP3_TRY_AGAIN);
                play_word(user_word);
                if (incorrect_tries == MAX_INCORRECT_TRIES_1) { // half done with attempts
                    play_direction(MP3_PLEASE_WRITE);
                    play_sound(MODE_FILESET, sound_source, true);
                    curr_word = word_to_glyph_word(this_script, sound_source);
                    play_word(curr_word);
                } else if (incorrect_tries >= MAX_INCORRECT_TRIES_2) { // done with attempts
                    play_glyph(curr_glyph);
                    play_direction(MP3_PLEASE_PRESS);
                    play_dot_sequence(curr_glyph);
                }
                next_state = STATE_INPUT;
            }
            break;
        



        case STATE_REPROMPT:        //@todo find this "skip" file!
            switch(create_dialog("SKIP", ENTER_CANCEL | LEFT_RIGHT)) {
                case NO_DOTS:
                    break;

                case CANCEL:
                    log_msg("[%s] Reissuing prompt", MODE_FILESET);
                    next_state = STATE_PROMPT;
                    scrolled = false;
                    break;

                case ENTER:
                    log_msg("[%s] Skipping sound_source", MODE_FILESET);
                    if (scrolled)
                        next_state = STATE_PROMPT;
                    else
                        next_state = STATE_GENQUES;
                    scrolled = false;
                    break;

                case RIGHT: case LEFT:
                    log_msg("[%s] Next sound_source", MODE_FILESET);
                    length_entered_word = 0;
                    current_word_index = 0;
                    sound_source = sound_source_list[choose_sound_source()];
                    user_word = NULL;
                    switch(submode) {
                        case SUBMODE_PLAY:
                            play_sound(MODE_FILESET, sound_source, false);
                            break;
                        
                        case SUBMODE_LEARN:
                            play_sound(MODE_FILESET, sound_source, true);
                            break;
                        
                        default:
                            break;
                    }
                    scrolled = true;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}
