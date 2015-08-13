/**
 * @file mode_9.c
 * @brief Code for mode 9 - Maths Practice
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "common.h"
#include "globals.h"
#include "audio.h"
#include "script_common.h"
#include "script_digits.h"
#include "mp3s.h"


// Limits
#define MAX_DIGITS 3

// State variables
static int operand_1 = -1;
static int operand_2 = -1;
static int result = -1;
static int user_answer = -1;
static bool input_ready = false;
static bool input_valid = false;

void mode_9_reset(void) {
    // Global variables
    reset_globals();
    reset_stats();
    operand_1 = operand_2 = result = user_answer = -1;
    input_ready = input_valid = false;
    max_mistakes = 3;
}

void mode_9_generate_question(void) {
    
    // Sets bound for result
    int bound = 1;
    for (int i = 0; i < level; i++)
        bound *= 10;

    do {
        operand_1 = timer_rand() % bound;
        log_msg("[mode_9] Operand 1: %d", operand_1);
        
        operand_2 = timer_rand() % bound;
        log_msg("[mode_9] Operand 2: %d", operand_2);
        
        
        switch (submode) {
            case SUBMODE_ADD:
                result = operand_1 + operand_2;
                break;
            case SUBMODE_SUBTRACT:
                result = operand_1 - operand_2;
                break;
            case SUBMODE_MULTIPLY:
                result = operand_1 * operand_2;
                break;
            default:
                log_msg("[mode_9] Error: submode: %c", submode);
                quit_mode();
                break;
        }
    } while (!(result > 0 && result < bound));
    log_msg("[mode_9] Result: %d", result);
}

void mode_9_play_question() {
    play_direction(MP3_WRITE_NUMBER_);
    play_number(operand_1);
    switch (submode) {
        case SUBMODE_ADD:
            play_direction(MP3_PLUS);
            break;
        case SUBMODE_SUBTRACT:
            play_direction(MP3_MINUS);
            break;
        case SUBMODE_MULTIPLY:
            play_direction(MP3_TIMES);
            break;
        default:
            log_msg("[mode_9] Error: submode: %c", submode);
            quit_mode();
            break;
    }
    play_number(operand_2);
}

void mode_9_play_answer(void) {
    play_feedback(MP3_THE_ANSWER_IS);
    play_number(result);
}

void mode_9_main(void) {
    switch (current_state) {
        case INITIAL:
            last_dot = create_dialog(MP3_CHOOSE_LEVELS_3,
                (DOT_1 | DOT_2 | DOT_3));
            switch (last_dot) {
                
                case '1':
                    log_msg("[mode_9] Submode: Addition");
                    submode = SUBMODE_ADD;
                    current_state = CHOOSE_LEVEL;
                    break;

                case '2':
                    log_msg("[mode_9] Submode: Subtraction");
                    submode = SUBMODE_SUBTRACT;
                    current_state = CHOOSE_LEVEL;
                    break;

                case '3':
                    log_msg("[mode_9] Submode: Multiplication");
                    submode = SUBMODE_MULTIPLY;
                    current_state = CHOOSE_LEVEL;
                    break;

                case CANCEL:
                    log_msg("Quitting to main menu.");                    
                    quit_mode();
                    break;

                default:
                    break;
            }
            break;

        case CHOOSE_LEVEL:
            last_dot = create_dialog(MP3_CHOOSE_LEVELS_3,
                (DOT_1 | DOT_2 | DOT_3));
            switch (last_dot) {
                
                case NO_DOTS:
                    break;

                case '1':
                    log_msg("[mode_9] Level: 1");
                    level = EASY;
                    play_direction(MP3_INSTRUCTIONS_MATH);
                    current_state = GENERATE_QUESTION;
                    break;

                case '2':
                    log_msg("[mode_9] Level: 2");
                    level = MEDIUM;
                    play_direction(MP3_INSTRUCTIONS_MATH);
                    current_state = GENERATE_QUESTION;
                    break;

                case '3':
                    log_msg("[mode_9] Level: 3");
                    level = HARD;
                    play_direction(MP3_INSTRUCTIONS_MATH);
                    current_state = GENERATE_QUESTION;
                    break;

                default:
                    log_msg("[mode_9] Error: last_dot: %c", last_dot);
                    quit_mode();
                    break;
            }
            break;

        case GENERATE_QUESTION:
            mode_9_generate_question();
            current_state = PROMPT;
            break;

        case PROMPT:
            mode_9_play_question();
            current_state = GET_INPUT;
            break;

        case GET_INPUT:
            if (io_user_abort == true) {
                log_msg("[mode_12] User aborted input");
                play_feedback(MP3_HELP_MENU);
                current_state = REPROMPT;
                io_init();
                break;
            }
            if (get_number(&input_valid, &user_answer)) {
                if (input_valid) {
                    log_msg("[mode_9] User answer: %d", user_answer);
                    play_feedback(MP3_YOU_ANSWERED);
                    play_number(user_answer);
                    current_state = CHECK_ANSWER;
                } else
                    log_msg("[mode_9] IO error");
            }
            break;

        case CHECK_ANSWER:
            if (user_answer == result) {     // Correct answer
                mistakes = 0;
                play_feedback(MP3_CORRECT);
                play_tada();
                current_state = GENERATE_QUESTION;
            } else {                        // Wrong answer
                mistakes++;
                play_feedback(MP3_CORRECT);
                if (mistakes >= max_mistakes)
                    mode_9_play_answer();
                play_feedback(MP3_TRY_AGAIN);
                current_state = PROMPT;
            }
            break;

        case REPROMPT:
                last_dot = create_dialog(MP3_WORD_COMMANDS,
                                            ENTER_CANCEL | LEFT_RIGHT | DOT_1
                                            | DOT_2 | DOT_3 | DOT_4
                                            | DOT_5 | DOT_6);
                switch (last_dot) {
                    
                    case NO_DOTS:
                        break;

                    case LEFT:
                        current_state = PROMPT;
                        io_init();
                        break;
                    
                    case RIGHT:
                        mode_9_play_answer();
                        current_state = GENERATE_QUESTION;
                        io_init();
                        break;

                    // Skipping question
                    case ENTER:
                        current_state = GENERATE_QUESTION;
                        break;

                    // Try again
                    case CANCEL:
                        play_feedback(MP3_EXIT_HELP_MENU);
                        play_feedback(MP3_RET_TO_QUESTION);
                        current_state = GET_INPUT;
                        io_init();
                        break;

                        
                    case '1': case '2': case '3': case '4': case '5': case '6':
                        play_feedback(MP3_WORD_COMMANDS);
                        io_init();
                        break;


                    default:
                        break;
                }
            break;

        default:
            log_msg("Invalid state_t %d", current_state);
            quit_mode();
            break;
    }
}