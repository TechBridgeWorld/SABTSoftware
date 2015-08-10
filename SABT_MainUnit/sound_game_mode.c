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
static bool scrolled = false;
char* mode_prefix;

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
                sprintf(filename, "N%s", sound_list[i]);
            else
                sprintf(filename, "S%s", sound_list[i]);
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
    reset_globals();
    reset_stats();
    mode_prefix = get_mode_prefix();
    max_mistakes = 6;
    user_glyph = curr_glyph = NULL;
    sound_list = (char**) SOUND_LIST;
    sound_source_list = (char**) SOUND_SOURCE_LIST;
    next_state = CHOOSE_LEVEL;
    user_word = free_word_old(user_word);
    curr_word = free_word_old(curr_word);
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

        case CHOOSE_LEVEL:
            if (io_user_abort == true) {
                log_msg("[%s] Quitting to main menu", mode_prefix);
                quit_mode();
                io_init();
            }
            switch(create_dialog(NULL, DOT_1 | DOT_2 | ENTER_CANCEL)) {

                case '1':
                    log_msg("[%s] Submode: Learn", mode_prefix);
                    submode = LEARN;
                    next_state = GENERATE_QUESTION;
                    break;

                case '2':
                    log_msg("[%s] Submode: Play", mode_prefix);
                    submode = PLAY;
                    next_state = GENERATE_QUESTION;
                case CANCEL:
                    log_msg("[%s] Quitting to main menu.", mode_prefix);                
                    quit_mode();
                    break;

                default:
                    break;
            }
            break;



        
        case GENERATE_QUESTION:
            length_entered_word = 0;
            current_word_index = 0;
            sound_source = sound_source_list[choose_sound_source()];
            user_glyph = NULL;
            curr_glyph = NULL;
            user_word = free_word_old(user_word);
            curr_word = free_word_old(curr_word);
            next_state = PROMPT;
            break;
            


        case PROMPT:
            switch(submode) {
                case LEARN:
                    play_mp3(mode_prefix, MP3_SPELL_WORD_BY_NAME);
                    play_sound(mode_prefix, sound_source, true);
                    break;
                
                case PLAY:
                    play_mp3(mode_prefix, MP3_SPELL_WORD_BY_SOUND);
                    play_sound(mode_prefix, sound_source, false);
                    break;
            
                default:
                break;  
            }
            next_state = GET_INPUT;
            break;
        


        case GET_INPUT:
            if (io_user_abort == true) {
                log_msg("[%s] User aborted input", mode_prefix);
                play_mp3(mode_prefix, MP3_SKIP_THIS);
                next_state = REPROMPT;
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
                    next_state = CHECK_ANSWER;
                    log_msg("[%s] Checking answer", mode_prefix);
                    break;
                case WITH_LEFT:
                    next_state = PROMPT;
                    break;
                case WITH_RIGHT:
                    play_mp3(mode_prefix, MP3_SKIP_THIS);
                    next_state = REPROMPT;
                    break;
                case WITH_CANCEL:
                    break;
            }
            break;


            

        case CHECK_ANSWER:
            curr_glyph = search_script(this_script, get_bits_from_letter(sound_source[length_entered_word]));
            if (glyph_equals(curr_glyph, user_glyph)) { // correct input
                play_glyph(curr_glyph);
                mistakes = 0;
                length_entered_word++;
                user_word = add_glyph_to_word(user_word, user_glyph);
                if (length_entered_word != strlen(sound_source)) { // not done with word
                    play_feedback(MP3_GOOD);
                    next_state = GET_INPUT;
                }
                else { // finished word
                    play_feedback(MP3_GOOD);
                    play_feedback(MP3_NICE_WORK);
                    switch (submode){
                        case LEARN:
                            play_sound(mode_prefix, sound_source, true);
                            play_direction(MP3_SAYS);
                            play_sound(mode_prefix, sound_source, false);
                            break;
                        
                        case PLAY:
                            play_word(user_word);
                            play_sound(mode_prefix, sound_source, true);

                        default:
                            log_msg("Invalid submode_t %d", submode);
                            quit_mode();

                    }
                    next_state = GENERATE_QUESTION;
                }
            }
            else { // incorrect input
                play_glyph(user_glyph);
                mistakes++;
                log_msg("[%s] User answered incorrectly", mode_prefix);
                play_feedback(MP3_NO);
                play_feedback(MP3_TRY_AGAIN);
                play_word(user_word);
                if (mistakes == (max_mistakes / 2)) {
                    play_direction(MP3_PLEASE_WRITE);
                    play_glyph(curr_glyph);
                    if (mistakes >= max_mistakes) { // done with attempts
                        play_direction(MP3_PLEASE_PRESS);
                        play_dot_sequence(curr_glyph);
                    }
                }
                next_state = GET_INPUT;
            }
            break;
        



        case REPROMPT:
            switch(create_dialog(NULL, ENTER_CANCEL | LEFT_RIGHT)) {
                case NO_DOTS:
                    break;

                case CANCEL:
                    log_msg("[%s] Reissuing prompt", mode_prefix);
                    next_state = PROMPT;
                    scrolled = false;
                    break;

                case ENTER:
                    log_msg("[%s] Skipping sound_source", mode_prefix);
                    if (scrolled)
                        next_state = PROMPT;
                    else
                        next_state = GENERATE_QUESTION;
                    scrolled = false;
                    break;

                case RIGHT: case LEFT:
                    log_msg("[%s] Next sound_source", mode_prefix);
                    length_entered_word = 0;
                    current_word_index = 0;
                    sound_source = sound_source_list[choose_sound_source()];
                    user_word = NULL;
                    switch(submode) {
                        case LEARN:
                            play_sound(mode_prefix, sound_source, true);
                            break;

                        case PLAY:
                            play_sound(mode_prefix, sound_source, false);
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
            log_msg("Invalid state_t %d", next_state);
            quit_mode();
            break;
    }
}
