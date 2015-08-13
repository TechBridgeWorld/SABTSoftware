/**
 * @file mode_1.c
 * @brief Mode 1 code - Dot Practice
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "globals.h"
#include "mode_1.h"
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
    char this_dot = '0' + dots[dot_index];
    dot_index = (dot_index + 1) % 6;
    return this_dot;
}

/**
 * @brief Reset mode 1 to starting state
 *        Should be useful for error handling
 * @return Void
 */
void mode_1_reset() {
    reset_globals();
    reset_stats();
    for (int i = 0; i < 6; i++)
        dots[i] = i + 1;
    dot_index = 0;
}

/**
 * @brief main code for Mode 1
 * Prompts the user to press a certain random dot, gives feedback based on
 * whether they pressed the correct dot
 * @return Void
 */
void mode_1_main()
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
            last_dot = get_dot();
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
                last_dot = 0;
                current_state = PROMPT;
            }
            break;

        default:
            log_msg("Invalid state_t %d", current_state);
            quit_mode();
            break;
    }
}