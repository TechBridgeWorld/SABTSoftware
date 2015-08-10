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

char used_number[6] = {0, 0, 0, 0, 0, 0};

/**
 * @brief based off of the internal timer (TCNT1) - we generate
 *        a psuedo-random number. Turn that into a number from 1 - 6
 *        return the charachter of that number;
 * @return char - charachter representation of a number from 1 - 6
 */
char random_number_as_char() {  // @todo use library for this
    int num = timer_rand() % 6;
    int i;

    //while you are looking at a full section of the array
    while (used_number[num]) {
        num = timer_rand() % 6;
    }

    used_number[num] = 1;
    used_num_cnt ++;

    //if you find that you have used all of the letters, clear both the array and the count
    if (used_num_cnt == 6) {
        for (i = 0; i < 6; i ++)
            used_number[i] = 0;
        used_num_cnt = 0;
    }

    //change from range 0-5 to 1-6
    num += 1;

    // Return the number as a character
    if (num >= 1 && num <= 6)
        return '0' + num;

    // Default behavior, could not generate a valid char
    log_msg("Failed to generate random number!");
    return '0';
}

/**
 * @brief Reset mode 1 to starting state
 *        Should be useful for error handling
 * @return Void
 */
void md1_reset(void) {
    reset_globals();
    reset_stats();
    used_num_cnt = 0;
    next_state = STATE_INITIAL;
}

/**
 * @brief main code for Mode 1
 * Prompts the user to press a certain random dot, gives feedback based on
 * whether they pressed the correct dot
 * @return Void
 */
void md1_main(void)
{
    switch(next_state) {
        case STATE_INITIAL:
            play_welcome();
            next_state = STATE_GENERATE_QUESTION;

        case STATE_GENERATE_QUESTION:
            expected_dot = random_number_as_char();
            next_state = STATE_PROMPT;

        case STATE_PROMPT:
            play_direction(MP3_FIND_DOT);
            play_dot(expected_dot);
            next_state = STATE_GET_INPUT;
            break;

        case STATE_GET_INPUT:
            if (last_dot != 0)
                next_state = STATE_CHECK_ANSWER;
            break;

        case STATE_CHECK_ANSWER:
            if (last_dot != expected_dot) {
                play_feedback(MP3_INCORRECT);
                play_dot(expected_dot);
                last_dot = 0;
                next_state = STATE_GET_INPUT;
            }
            else {
                play_feedback(MP3_CORRECT);
                play_tada();
                last_dot = 0;
                next_state = STATE_GENERATE_QUESTION;
            }
            break;
        default:
            break;
    }
}

/**
 * @brief in mode 1, enter button just replays the current prompt
 * @return Void
 */
void md1_call_mode_yes_answer(void) {
    next_state = STATE_PROMPT;
}

void md1_call_mode_no_answer(void) {}

/**
 * @brief register dot input
 *        Sets the program to STATE_PROC_INPUT
 * @param this_dot the dot being input
 * @return Void
 */
void md1_input_dot(char this_dot) {
    last_dot = this_dot;
    next_state = STATE_CHECK_ANSWER;
}

/**
 * @brief register cell input
 * @param this_cell the cell being input
 * @return void
 */
void md1_input_cell(char this_cell) {}
