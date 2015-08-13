/**
 * @file mode_13.c
 * @brief Code for mode 13 - Number Game
 * @author Susan Zuo(tzuo)
 */

#include "common.h"
#include "globals.h"
#include "audio.h"
#include "script_common.h"
#include "script_digits.h"
#include "mp3s.h"

/**
 *  Question types
 */
#define QUES_TYPES 7

#define QEST_BETWEEN    0
#define QEST_EVEN_NUM   1
#define QEST_ODD_NUM    2
#define QEST_TENS_NUM   3
#define QEST_HUNDS_NUM  4
#define QEST_ROUND_TEN  5
#define QEST_ROUND_HUND 6

#define NOT_ENTERED 0
#define ENTERED     1

/*
 *  Limits
 */
#define MAX_DIGITS 4
#define MAX_ANSWER_NUM 10
#define EASY_QUES  5

/**
 *  State variables
 */
static long operand_1 = -1;
static long operand_2 = -1;
static int user_answer = -1;
static bool input_ready = false;
static bool input_valid = false;
static int question = QEST_BETWEEN;
static int answer[MAX_ANSWER_NUM];
static int entered[MAX_ANSWER_NUM];
static int num_answer = 0;
static int ques_index[QUES_TYPES];
static int ques_i;
static int max_q;

void init_array(int* array, int array_len){
    for (int i = 0; i < array_len; i++)
        array[i] = 0;
}

void init_index(int bound){
    for (int i = 0; i < bound; i++)
        ques_index[i] = i;
}

void mode_13_reset() {
    log_msg("*** mode_13 - Math Problems ***");
    
    // Global variables
    reset_globals();
    reset_stats();
    operand_1 = operand_2 = user_answer = -1;
    num_answer = ques_i = max_q = 0;
    input_ready = input_valid = false;
    question = QEST_BETWEEN;
    init_array(answer, MAX_ANSWER_NUM);
    init_array(entered, MAX_ANSWER_NUM);
    max_mistakes = 3;

    
}

void mode_13_generate_op2(){
    switch (question) {
        case QEST_EVEN_NUM:
            operand_1 = (operand_1 % 2 == 0) ? operand_1 : operand_1 + 1;
            operand_2 = (score > 3) ? (operand_1 + 10) : (operand_1 + score * 2 + 4);
            break;

        case QEST_ODD_NUM:
            operand_1 = (operand_1 % 2 == 1) ? operand_1 : operand_1 + 1;
            operand_2 = (score > 3) ? (operand_1 + 10) : (operand_1 + score * 2 + 4);
            break;

        case QEST_ROUND_HUND: case QEST_HUNDS_NUM: case QEST_ROUND_TEN: case QEST_TENS_NUM:
            operand_2 = operand_1 / 10 * 10 + random_between(1,9);
            break;

        default:
            operand_2 = (score > 3) ? (operand_1 + 5): (operand_1 + score + 2);
            break;
    }
    log_msg("[mode_13] Operand 1: %ld; Operand 2: %ld", operand_1, operand_2);
    
}

void mode_13_generate_number(){
	int base = 0;
    switch (level) {
        case EASY:
            base = (score >= 3) ? (60) : (score * 20 + 1);
            operand_1 = random_between(base,base + 20);
            mode_13_generate_op2();
            break;

        case HARD:
			base = (base < 9500) ? ((score + 1) * 500) : 9500;
            operand_1 = random_between(base, base + 30);
            mode_13_generate_op2();
            break;

        default:
            log_msg("[MODE13] Error generating question");
            break;
    }
}

void mode_13_play_question(){
    switch (question) {
        case QEST_BETWEEN:
			if (operand_2 - operand_1 == 2)
                play_direction(MP3_ENTER_NUM_BETWEEN);
            else
				play_direction(MP3_ENTER_NUMS_BETWEEN);
            play_number(operand_1);
            play_direction(MP3_AND);
            play_number(operand_2);
            break;
            
        case QEST_EVEN_NUM:
            play_direction(MP3_ENTER_EVENS_BTWN);
            play_number(operand_1);
            play_direction(MP3_AND);
            play_number(operand_2);
            break;
            
        case QEST_ODD_NUM:
            play_direction(MP3_ENTER_ODDS_BTWN);
            play_number(operand_1);
            play_direction(MP3_AND);
            play_number(operand_2);
            break;
            
        case QEST_TENS_NUM:
            play_direction(MP3_HOW_MANY_TENS);
            play_number(operand_2);
            break;
            
        case QEST_HUNDS_NUM:
            play_direction(MP3_HOW_MANY_HUNDREDS);
            play_number(operand_2);
            break;
        
        case QEST_ROUND_TEN:
            play_direction(MP3_ROUND_TO_TENS);
            play_number(operand_2);
            break;
            
        case QEST_ROUND_HUND:
            play_direction(MP3_ROUND_TO_HUNDREDS);
            play_number(operand_2);
            break;
            
        default:
            break;
    }
    
}

void mode_13_generate_answer() {
    int i, step;
    switch (question) {
        case QEST_BETWEEN:
            step = 1;
            for (i = 0; i < operand_2 - operand_1 - 1 ; i++) {
                answer[i] = operand_1 + i + 1;
            }
            num_answer = i;
            break;
            
        case QEST_EVEN_NUM:
            step = 2;
            answer[0] = (operand_1 % 2 == 0) ? (operand_1 + 2) : (operand_1 + 1);
            for (i = 0; answer[i] + step < operand_2; i++)
                answer[i + 1] = answer[i] + step;
            num_answer = i + 1;
            break;

        case QEST_ODD_NUM:
            step = 2;
            answer[0] = (operand_1 % 2 == 1) ? (operand_1 + 2) : operand_1 + 1;
            for (i = 0; answer[i] < operand_2 - step; i++)
                answer[i + 1] = answer[i] + step;
            num_answer = i + 1;
            break;

        case QEST_TENS_NUM:
            answer[0] = operand_2 / 10;
            num_answer = 1;
            break;

        case QEST_HUNDS_NUM:
            answer[0] = operand_2 / 100;
            num_answer = 1;
            break;

        case QEST_ROUND_TEN:
            answer[0] = (operand_2 % 10 < 5) ? (operand_2 / 10 * 10) : (operand_2 / 10 * 10 + 10);
            num_answer = 1;
			break;

        case QEST_ROUND_HUND:
            answer[0] = (operand_2 % 100 < 50) ? (operand_2 / 100 * 100) : (operand_2 / 100 * 100 + 100);
            num_answer = 1;
			break;

        default:
            break;
    }
}

void mode_13_play_answer(){
    play_feedback(MP3_THE_ANSWER_IS);
    for (int i = 0; i < num_answer; i++){
        play_number(answer[i]);
        play_silence(1000);
    }
}

void mode_13_play_input(int flag){
    bool have_played = false;
    for (int i = 0; i < num_answer; i++) {
        if (flag == ENTERED && entered[i] == 1) {
            play_number(answer[i]);
            play_silence(750);
            have_played = true;
        }
        else if (flag == NOT_ENTERED && entered[i] == 0) {
            play_number(answer[i]);
            play_silence(750);
        }
    }
    if ((!have_played) && (flag == ENTERED))
        play_feedback(MP3_NO_ANSWER_SO_FAR);
}

bool place_number(int number){
    for (int i = 0; i < num_answer; i++) {
        log_msg("[Mode 13] Answer:%d Entered:%d", answer[i],number);
        
        if (number == answer[i]) {
            entered[i] = 1;
            return true;
        }
    }
    return false;
}

bool all_entered() {
    for (int i = 0; i < num_answer; i++)
        if (entered[i] == 0) return false;
    return true;
}

void mode_13_choose_ques() {
    if (ques_i == max_q) {
        shuffle(max_q, ques_index);
        ques_i = 0;
    }
    question = ques_index[ques_i];
    ques_i++;
   	log_msg("[Mode 13] q%d", question);
    return;
}

void play_mistake(){
    play_feedback(MP3_YOU_HAVE);
    play_number(max_mistakes - mistakes);
    if (max_mistakes - mistakes == 1)
        play_feedback(MP3_MISTAKE_REMAINING);
    else
        play_feedback(MP3_MISTAKES_REMAINING);
}

void mode_13_main() {
    switch (current_state) {
		case INITIAL:
            play_welcome();
            current_state = CHOOSE_LEVEL;
            break;

        case CHOOSE_LEVEL:
            last_dot = create_dialog(MP3_CHOOSE_LEVELS_2,
                                        ( DOT_1 | DOT_2 | CANCEL));
            switch (last_dot) {
                case '1':
                    log_msg("[mode_13] Level 1");
                    level = EASY;
                    play_direction(MP3_INSTRUCTIONS_MATH);
                    max_q = EASY_QUES;
                    init_index(max_q);
                    shuffle(max_q, ques_index);
                    current_state = GENERATE_QUESTION;
                    break;
                    
                case '2':
                    log_msg("[mode_13] Level 2");
                    level = HARD;
                    play_direction(MP3_INSTRUCTIONS_MATH);
                    max_q = QUES_TYPES;
                    init_index(max_q);
                    shuffle(max_q, ques_index);
                    current_state = GENERATE_QUESTION;
                    break;
                
                case CANCEL:
                    log_msg("[mode_13] Quitting to main menu.");                    
                    quit_mode();
                    break;
                    
                default:
                    break;
            }
            break;
        
        case GENERATE_QUESTION:
            init_array(answer, MAX_ANSWER_NUM);
            init_array(entered, MAX_ANSWER_NUM);
            mode_13_generate_number();
            mode_13_generate_answer();
            mistakes = 0;
            log_msg("[Mode 13] num answer: %d answer: %d", num_answer, answer[0]);
            current_state = PROMPT;
            break;
            
        case PROMPT:
			io_init();
            mode_13_play_question();
            current_state = GET_INPUT;
            break;
            
        case GET_INPUT:
            if (io_user_abort == true) {
                log_msg("[mode_13] User aborted input");
                current_state = REPROMPT;
                play_feedback(MP3_HELP_MENU);
                io_init();
                break;
            }
		//	set_mode_globals(&script_digits, NULL, NULL); Why would we reset the script here???
             if (get_number(&input_valid, &user_answer)) {
                if (input_valid) {
                    play_feedback(MP3_YOU_ANSWERED);
                    play_number(user_answer);
                    current_state = CHECK_ANSWER;
                }
                else
                    log_msg("[mode_13] IO error");
            }
            break;
            
        case CHECK_ANSWER:
            if (place_number(user_answer)) {     // Correct answer
                log_msg("[mode_13] Correct answer");
                if (!all_entered()) {
                    play_feedback(MP3_GOOD_NEXT);
                    current_state = GET_INPUT;
                }
                else {
                    score++;
                    play_feedback(MP3_GOOD);
                    play_feedback(MP3_YOUR_SCORE_IS);
                    play_number(score);
                    play_tada();
					mode_13_choose_ques();
                    current_state = GENERATE_QUESTION;
                }
            }
            else {
                // Wrong answer
                mistakes += 1;
                play_feedback(MP3_INCORRECT);
                if (mistakes >= max_mistakes) {
                    play_feedback(MP3_YOU_MISSED);
                    mode_13_play_input(NOT_ENTERED);
					score = (score == 0) ? 0 : score - 1;
                    play_feedback(MP3_YOUR_SCORE_IS);
					play_number(score);
                    mode_13_choose_ques();
                    current_state = GENERATE_QUESTION;
                }
                else {
                    play_mistake();
					current_state = PROMPT;
				}
            }
            break;
            
        case REPROMPT:
            last_dot = create_dialog(MP3_HELP_MENU_13,
                                        ENTER_CANCEL | LEFT_RIGHT | DOT_1
                                        | DOT_2 | DOT_3 | DOT_4
                                        | DOT_5 | DOT_6);
            switch (last_dot) {
                    
                case NO_DOTS:
                    break;
           
                case LEFT:  // play answer
                    current_state = PROMPT;
                    io_init();
                    break;

                case RIGHT:
                    mode_13_play_answer();
                    mode_13_choose_ques();
                    current_state = GENERATE_QUESTION;
                    io_init();
                    break;
                    
                case ENTER: // current input
                    play_feedback(MP3_YOU_ANSWERED);
                    mode_13_play_input(ENTERED);
                    play_feedback(MP3_TRY_AGAIN);
                    current_state = GET_INPUT;
                    io_init();
                    break;


                case CANCEL:    // exit input
                    play_feedback(MP3_EXIT_HELP_MENU);
                    play_feedback(MP3_RET_TO_QUESTION);
                    current_state = GET_INPUT;
					io_init();
                    break;
					
				case '1': case '2': case '3': case '4': case '5': case '6':
                    play_direction(MP3_HELP_MENU_13);
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