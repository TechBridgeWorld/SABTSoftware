

/*
 * @file learn_letter_mode.c
 *
 * @brief code for helper functions "learning letter modes" (MD 2,7,8,12)
 * @author: Edward Cai
 * Edited by Marjorie Carlson
 *
 * This file provides the functionality for all the modes for learning letters 
 * (English, Hindi, etc.) At the menu, the user chooses between "learning" mode
 * (1), which tells them the correct dots, and which iterates through the alphabet
 * in order; and "practicing" mode (2), which does not prompt them with the correct
 * dots, and which shuffles the alphabet first (as well as after each time the
 * student completes the entire alphabet.)
 *
 * Students must input the correct dots for the prompted letter and then press
 * enter, at which point the mode will check their answer and respond appropriately.
 * They may replay the question by hitting cancel or left, and skip to the next
 * letter by hitting right arrow.
 *
 * The states are: MENU -> GENQUES (gets the next letter) -> PROMPT (plays
 * the letter, and its dot sequence if in learn mode) -> INPUT (gets the inputted
 * cell; if it's a cell terminated by enter, goes to CHECK; if input is cancel or
 * left arrow, goes back to PROMPT to replay question; if input is right arrow, goes
 * back to GENQUES to skip to the next letter. CHECK checks whether the input was
 * correct; if the input was correct, then it plays tada and goes to GENQUES *if*
 * the inputted glyph was the last glyph in the current letter; if there are more
 * glyphs in this letter, it goes back to input to get them. If the input was
 * incorrect, it goes back to the first glyph in this letter and returns to PROMPT
 * for another attempt, unless the user is out of tries, in which case it plays
 * the dot sequence and returns to INPUT for another attempt.
 * 
 */ 

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Custom libraries
#include "audio.h"
#include "io.h"
#include "common.h"
#include "globals.h"
#include "debug.h"
#include "script_common.h"
#include "learn_letter_mode.h"
#include "mp3s.h"

// State variables
static char mode_name[5];
static bool should_shuffle = false;
static glyph_t* curr_glyph = NULL;
static glyph_t* user_glyph = NULL;
static language_t language;

void learn_letter_reset() {
    reset_globals();
    reset_stats();
    log_msg("Mode reset");
    max_mistakes = 3;
}

void play_letter_instructions() {
    if (language == KANNADA)
        play_direction(MP3_INSTRUCTIONS_KANNADA);
    else
        play_direction(MP3_INSTRUCTIONS_LETTER);
}

void play_next_letter_prompt() {
    if (language == KANNADA)
        play_direction(MP3_NEXT_LETTER_K);
    else
        play_direction(MP3_NEXT_LETTER);
}

void play_dot_prompt() {
    if (language == KANNADA)
        play_direction(MP3_PRESS_DOTS_K);
    else
        play_direction(MP3_PRESS_DOTS);
}

void play_next_cell_prompt() {
    if (language == KANNADA)
        play_direction(MP3_NEXT_CELL_K);
    else
        play_direction(MP3_NEXT_CELL);
}

void play_right() {
    if (language == KANNADA)
        play_feedback(MP3_CORRECT_K);
    else
        play_feedback(MP3_CORRECT);
    play_tada();
}

void play_wrong() {
    if (language == KANNADA) {
        play_feedback(MP3_INCORRECT_K);
        play_feedback(MP3_TRY_AGAIN_K);
    }
    else {
        play_feedback(MP3_INCORRECT);
        play_feedback(MP3_TRY_AGAIN);
    }
}

void learn_letter_main() {
    switch (current_state) {

        case INITIAL:
            play_welcome();
            play_submode_choice();
            current_state = CHOOSE_LEVEL;
            break;

        case CHOOSE_LEVEL:

            switch(create_dialog(NULL, DOT_1 | DOT_2 | ENTER_CANCEL)) {
                case '1':
                    log_msg("Submode: Learn");
                    play_letter_instructions();
                    submode = SUBMODE_LEARN;
                    unshuffle(this_script->num_letters, this_script->letters);
                    current_state = GENERATE_QUESTION;
                    break;

                case '2':
                    log_msg("Submode: Play");
                    play_letter_instructions();
                    shuffle(this_script->num_letters, this_script->letters);
                    should_shuffle = true;
                    current_state = GENERATE_QUESTION;
                    break;

                case CANCEL:
                    log_msg("Quitting to main menu");                    
                    quit_mode();
                    break;

                case ENTER:
                    log_msg("Re-issuing main menu prompt");
                    play_submode_choice();
                    current_state = CHOOSE_LEVEL;
                    break;

                default:
                    break;
            }
            break;


        case GENERATE_QUESTION:
            curr_glyph = get_next_letter(this_script, should_shuffle);
            log_msg("State: GENERATE_QUESTION. Next glyph: %s", mode_name, curr_glyph->sound);
            play_next_letter_prompt();
            current_state = PROMPT;
            break;


        case PROMPT:
            log_msg("State: PROMPT.",mode_name);
            play_glyph(curr_glyph);
            switch(submode) {
                case SUBMODE_LEARN:
                    play_dot_prompt();
                    play_dot_sequence(curr_glyph);
                    break;

                case SUBMODE_PLAY:
                default:
                    break;
            }
            current_state = GET_INPUT;
            break;

        case GET_INPUT:
            if (io_user_abort == true) {
                log_msg("User aborted input");
                current_state = PROMPT;
                io_init();
            }

            cell = get_cell();
            if (cell == NO_DOTS)
                break;
            cell_pattern = GET_CELL_PATTERN(cell);
            cell_control = GET_CELL_CONTROL(cell);
            log_msg("Pattern %d, control %d.", cell_pattern, cell_control);
            
            switch (cell_control) {
                case WITH_ENTER:
                    user_glyph = search_script(this_script, cell_pattern);
                    current_state = CHECK_ANSWER;
                    log_msg("Checking answer.");
                    break;
                case WITH_LEFT:
                    current_state = PROMPT;
                    break;
                case WITH_RIGHT:
                    current_state = GENERATE_QUESTION;
                    break;
                case WITH_CANCEL:
                default:
                    break;

            }
            break;

        case CHECK_ANSWER:
            log_msg("State: CHECK_ANSWER");
            
            if (glyph_equals(curr_glyph, user_glyph)) {
                if (curr_glyph -> next == NULL) {
                    mistakes = 0;
                    log_msg("User answered correctly");
                    play_right();
                    current_state = GENERATE_QUESTION;
                }
                else {
                    curr_glyph = curr_glyph->next;
                    play_next_cell_prompt();                    
                    if (submode == SUBMODE_LEARN)
                        play_dot_sequence(curr_glyph);
                    else
                        play_glyph(curr_glyph);
                    current_state = GET_INPUT;
                }
            }
            else {
                mistakes++;
                log_msg("User answered incorrectly");
                play_wrong();
                curr_glyph = get_root(this_script, curr_glyph);  
                current_state = PROMPT;
                if (mistakes >= max_mistakes) {
                    play_glyph(curr_glyph);
                    play_dot_prompt();
                    play_dot_sequence(curr_glyph);
                    current_state = GET_INPUT;
                }
            }
        break;

        default:
            log_msg("Invalid state_t %d", current_state);
            quit_mode();
            break;
    }
}