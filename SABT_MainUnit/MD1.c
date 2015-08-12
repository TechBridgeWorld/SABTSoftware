/**
 * @file MD1.c
 * @brief Mode 1 code - Dot Practice
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "globals.h"
#include "MD1.h"
#include "audio.h"
#include "script_common.h"
#include "script_english.h"
#include "common.h"

int dots[6];
int dot_index;

char choose_dot(){
    if (dot_index == 0) {
        shuffle(6, dots);
        for (int i = 0; i < 6; i++)
            log_msg("Dot %d = %d", i, dots[i]);
    }
    char this_dot = dots[dot_index];
    log_msg("Choosing dot %d = %d", dot_index, this_dot);
    dot_index = (dot_index + 1) % 6;
    return this_dot;
}

/**
 * @brief Reset mode 1 to starting state
 *        Should be useful for error handling
 * @return Void
 */
void md1_reset(void) {
    reset_globals();
    reset_stats();
    for (int i = 0; i < 6; i++) {
        dots[i] = i + 1;
        log_msg("Dot %d = %d", i, i + 1);
    }
    dot_index = 0;
    current_state = INITIAL;
}

/**
 * @brief main code for Mode 1
 * Prompts the user to press a certain random dot, gives feedback based on
 * whether they pressed the correct dot
 * @return Void
 */
void md1_main(void)
{
    switch(current_state) {
        case INITIAL:
            play_welcome();
            current_state = GENERATE_QUESTION;

        case GENERATE_QUESTION:
            expected_dot = choose_dot();
            current_state = PROMPT;

        case PROMPT:
            play_direction(MP3_FIND_DOT);
            play_dot(expected_dot);
            current_state = GET_INPUT;
            break;

        case GET_INPUT:
            if (last_dot != 0)
                current_state = CHECK_ANSWER;
            break;

        case CHECK_ANSWER:
            if (last_dot == expected_dot) {
                play_feedback(MP3_CORRECT);
                play_tada();
                last_dot = 0;
                current_state = GENERATE_QUESTION;
            }
            else {
                play_feedback(MP3_INCORRECT);
                play_dot(expected_dot);
                last_dot = 0;
                current_state = GET_INPUT;
            }
            break;

        default:
            log_msg("Invalid state_t %d", current_state);
            quit_mode();
            break;
    }
}

/**
 * @brief in mode 1, enter button just replays the current prompt
 * @return Void
 */
void md1_call_mode_yes_answer(void) {
    current_state = PROMPT;
}

void md1_call_mode_no_answer(void) {}

/**
 * @brief register dot input
 *        Sets the program to PROC_INPUT
 * @param this_dot the dot being input
 * @return Void
 */
void md1_input_dot(char this_dot) {
    last_dot = this_dot;
    current_state = CHECK_ANSWER;
}

/**
 * @brief register cell input
 * @param this_cell the cell being input
 * @return void
 */
void md1_input_cell(char this_cell) {}
