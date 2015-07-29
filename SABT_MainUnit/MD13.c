/**
 * @file MD13.c
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
 *  States
 */
#define STATE_NULL 0x00
#define STATE_LVLSEL 0x01
#define STATE_GENQUES 0x02
#define STATE_PROMPT 0x03
#define STATE_INPUT 0x04
#define STATE_CHECKANS 0x05
#define STATE_REPROMPT 0x06
#define STATE_INTRO 0x07
#define STATE_FOLLOWUP 0x08

/**
 *  Different levels
 */
#define LEVEL_NULL      0x0000
#define LEVEL_1			0x0001
#define LEVEL_2			0x0002

/**
 *  Question types
 */
#define QEST_BETWEEN    0x001
#define QEST_EVEN_NUM   0x002
#define QEST_ODD_NUM    0x000
#define QEST_TENS_NUM   0x004
#define QEST_HUNDS_NUM  0x006
#define QEST_ROUND_TEN  0x003
#define QEST_ROUND_HUND 0x005

#define NOT_ENTERED 0x00
#define ENTERED 0x01

/*
 *  Limits
 */
#define MAX_DIGITS 4
#define MAX_INCORRECT_TRIES 3
#define MAX_ANSWER_NUM 10
#define QUES_TYPES 7
#define EASY_QUES  5


/**
 *  State variables
 */
static char md_next_state = STATE_NULL;
static char md_last_dot = NO_DOTS;
static int md_level = 0;
static long md_op_1 = -1;
static long md_op_2 = -1;
static int md_usr_res = -1;
static bool md_input_ready = false;
static bool md_input_valid = false;
static int md_incorrect_tries = 0;
static int md_question = QEST_BETWEEN;
static int md_score = 0;
static int md_answer[MAX_ANSWER_NUM];
static int md_entered[MAX_ANSWER_NUM];
static int md_num_answer = 0;
static int md_ques_index[QUES_TYPES];
static int md_ques_i;
static int md_max_q;

void init_array(int* array, int array_len){
    for (int i = 0; i < array_len; i++) {
        array[i] = 0;
    }
}

void init_index(int bound){
    for (int i = 0; i < bound; i++) {
        md_ques_index[i] = i;
    }
}

void md13_reset(void) {
    log_msg("*** MD13 - Math Problems ***\n\r");
    
    // Global variables
    set_mode_globals(&script_digits, NULL, NULL);
    
    // State variables
    md_next_state = STATE_INTRO;
    md_last_dot = NO_DOTS;
    md_level = LEVEL_NULL;
    md_op_1 = -1;
    md_op_2 = -1;
    md_usr_res = -1;
    md_input_ready = false;
    md_input_valid = false;
    md_incorrect_tries = 0;
    md_question = QEST_BETWEEN;
    md_score = 0;
    init_array(md_answer, MAX_ANSWER_NUM);
    init_array(md_entered, MAX_ANSWER_NUM);
    md_num_answer = 0;
    md_ques_i = 0;
    md_max_q = 0;
    
}

void md13_generate_op2(){
    switch (md_question) {
        case QEST_EVEN_NUM:
            md_op_1 = (md_op_1 % 2 == 0) ? md_op_1 : md_op_1 + 1;
            md_op_2 = (md_score > 3) ? (md_op_1 + 10) : (md_op_1 + md_score * 2 + 4);
            break;
        case QEST_ODD_NUM:
            md_op_1 = (md_op_1 % 2 == 1) ? md_op_1 : md_op_1 + 1;
            md_op_2 = (md_score > 3) ? (md_op_1 + 10) : (md_op_1 + md_score * 2 + 4);
            break;
        case QEST_ROUND_HUND: case QEST_HUNDS_NUM: case QEST_ROUND_TEN: case QEST_TENS_NUM:
            md_op_2 = md_op_1 / 10 * 10 + random_between(1,9);
            break;
        default:
            md_op_2 = (md_score > 3) ? (md_op_1 + 5): (md_op_1 + md_score + 2);
            break;
    }
    log_msg("[MD13] Operand 1: %ld\n\r", md_op_1);
    
    log_msg("[MD13] Operand 2: %ld\n\r", md_op_2);
    
}

void md13_generate_number(){
	int base = 0;
    switch (md_level) {
        case LEVEL_1:
            base = (md_score >= 3) ? (60) : (md_score * 20 + 1);
            md_op_1 = random_between(base,base + 20);
            md13_generate_op2();
            break;
        case LEVEL_2:
			base = (base < 9500) ? ((md_score + 1) * 500) : 9500;
            md_op_1 = random_between(base, base + 30);
            md13_generate_op2();
            break;
        default:
            log_msg("[MODE13]Error generating question");
            break;
    }
}

void md13_play_question(){
    switch (md_question) {
        case QEST_BETWEEN:
			if (md_op_2 - md_op_1 == 2)
                play_direction(MP3_ENTER_NUM_BETWEEN);
            else
				play_direction(MP3_ENTER_NUMS_BETWEEN);
            play_number(md_op_1);
            ////_delay_ms(1000);
            play_direction(MP3_AND);
			////_delay_ms(1000);
            play_number(md_op_2);
            break;
            
        case QEST_EVEN_NUM:
            play_direction(MP3_ENTER_EVENS_BTWN);
            play_number(md_op_1);
            ////_delay_ms(1000);
            play_direction(MP3_AND);
            ////_delay_ms(1000);
            play_number(md_op_2);
            break;
            
        case QEST_ODD_NUM:
            play_direction(MP3_ENTER_ODDS_BTWN);
            play_number(md_op_1);
            //_delay_ms(1000);
            play_direction(MP3_AND);
            //_delay_ms(1000);
            play_number(md_op_2);
            break;
            
        case QEST_TENS_NUM:
            play_direction(MP3_HOW_MANY_TENS);
            play_number(md_op_2);
            break;
            
        case QEST_HUNDS_NUM:
            play_direction(MP3_HOW_MANY_HUNDREDS);
            play_number(md_op_2);
            break;
        
        case QEST_ROUND_TEN:
            play_direction(MP3_ROUND_TO_TENS);
            play_number(md_op_2);
            break;
            
        case QEST_ROUND_HUND:
            play_direction(MP3_ROUND_TO_HUNDREDS);
            play_number(md_op_2);
            break;
            
        default:
            break;
    }
    
}

void md13_generate_answer(){
    int step;
	int i;
    switch (md_question) {
        case QEST_BETWEEN:
            step = 1;
            for (i = 0; i < md_op_2 - md_op_1 - 1 ; i++) {
                md_answer[i] = md_op_1 + i + 1;
            }
            md_num_answer = i;
            break;
            
        case QEST_EVEN_NUM:
            step = 2;
            md_answer[0] = (md_op_1 % 2 == 0) ? (md_op_1 + 2) : (md_op_1 + 1);
            i = 0;
            while (md_answer[i] + step < md_op_2){
                md_answer[i + 1] = md_answer[i] + step;
                i++;
            }
            md_num_answer = i + 1;
            break;
        case QEST_ODD_NUM:
            step = 2;
            md_answer[0] = (md_op_1 % 2 == 1) ? (md_op_1 + 2) : md_op_1 + 1;
            i = 0;
            while (md_answer[i] < md_op_2 - step){
                md_answer[i + 1] = md_answer[i] + step;
                i++;
            }
            md_num_answer = i + 1;
            break;
        case QEST_TENS_NUM:
            md_answer[0] = md_op_2 / 10;
            md_num_answer = 1;
            break;
        case QEST_HUNDS_NUM:
            md_answer[0] = md_op_2 / 100;
            md_num_answer = 1;
            break;
        case QEST_ROUND_TEN:
            md_answer[0] = (md_op_2 % 10 < 5) ? (md_op_2 / 10 * 10) : (md_op_2 / 10 * 10 + 10);
            md_num_answer = 1;
			break;
        case QEST_ROUND_HUND:
            md_answer[0] = (md_op_2 % 100 < 50) ? (md_op_2 / 100 * 100) : (md_op_2 / 100 * 100 + 100);
            md_num_answer = 1;
			break;
        default:
            break;
    }
}

void md13_play_answer(){
    play_feedback(MP3_THE_ANSWER_IS);
    for(int i = 0; i < md_num_answer; i++){
        play_number(md_answer[i]);
        play_silence(1000);
    }
}

void md13_play_input(int flag){
    bool have_played = false;
    for(int i = 0; i < md_num_answer; i++){
        if (flag == ENTERED){
            if (md_entered[i] == 1) {
                play_number(md_answer[i]);
                play_silence(750);
                have_played = true;
            }
        } else if (flag == NOT_ENTERED ){
            if (md_entered[i] == 0) {
                play_number(md_answer[i]);
                play_silence(750);
            }
        }
    }
    if ((!have_played) && (flag == ENTERED))
        play_feedback(MP3_NO_ANSWER_SO_FAR);
}

bool md13_place_number(int number){
    for (int i = 0; i < md_num_answer; i++) {
        log_msg("[mode13]answer:%d entered:%d\n\r", md_answer[i],number);
        
        if (number == md_answer[i]){
            md_entered[i] = 1;
            return true;
        }
    }
    return false;
}

bool md13_all_entered(){
    for (int i = 0; i < md_num_answer; i++) {
        if (md_entered[i] == 0) return false;
    }
    return true;
}

void md13_choose_ques(){
    if (md_ques_i == md_max_q) {
        shuffle(md_max_q, md_ques_index);
        md_ques_i = 0;
    }
    md_question = md_ques_index[md_ques_i];
    md_ques_i++;
   	log_msg("[mode]q%d",md_question );
	
    return;
}

void play_mistake(){
    play_feedback(MP3_YOU_HAVE);
    play_number(MAX_INCORRECT_TRIES - md_incorrect_tries);
    if (MAX_INCORRECT_TRIES - md_incorrect_tries == 1)
        play_feedback(MP3_MISTAKE_REMAINING);
    else
        play_feedback(MP3_MISTAKES_REMAINING);
}

void md13_main(void) {
    switch (md_next_state) {
		case STATE_INTRO:
            play_welcome();
            md_next_state = STATE_LVLSEL;
            break;
        case STATE_LVLSEL:
            md_last_dot = create_dialog(MP3_CHOOSE_LEVELS_2,
                                        ( DOT_1 | DOT_2 ));
            switch (md_last_dot) {
                    
                case NO_DOTS:
                    break;
                    
                case '1':
                    log_msg("[MD13] Level: 1\n\r");
                    md_level = LEVEL_1;
                    play_direction(MP3_INSTRUCTIONS_MATH);
                    md_max_q = EASY_QUES;
                    init_index(md_max_q);
                    shuffle(md_max_q, md_ques_index);
                    md_next_state = STATE_GENQUES;
                    break;
                    
                case '2':
                    log_msg("[MD13] Level: 2\n\r");
                    md_level = LEVEL_2;
                    play_direction(MP3_INSTRUCTIONS_MATH);
                    md_max_q = QUES_TYPES;
                    init_index(md_max_q);
                    shuffle(md_max_q, md_ques_index);
                    md_next_state = STATE_GENQUES;
                    break;
                
                    
                default:
                    log_msg("[MD13] Error: md_last_dot: ");
                    SENDBYTE(md_last_dot);
                    NEWLINE;
                    quit_mode();
                    break;
            }
            break;
        
        case STATE_GENQUES:
            init_array(md_answer, MAX_ANSWER_NUM);
            init_array(md_entered, MAX_ANSWER_NUM);
            md13_generate_number();
            md13_generate_answer();
            md_incorrect_tries = 0;
            log_msg("[mode13]num answer: %d answer:%d\n\r", md_num_answer, md_answer[0]);
            
            md_next_state = STATE_PROMPT;
            break;
            
        case STATE_PROMPT:
			io_init();
            ////_delay_ms(100);
            md13_play_question();
            md_next_state = STATE_INPUT;
            break;
            
        case STATE_INPUT:
            if (io_user_abort == true) {
                log_msg("[MD13] User aborted input\n\r");
                md_next_state = STATE_REPROMPT;
                play_feedback(MP3_HELP_MENU);
                io_init();
                break;
            }
			set_mode_globals(&script_digits, NULL, NULL);
             if (get_number(&md_input_valid, &md_usr_res)) {
                if (md_input_valid) {
//                    log_msg("[MD13] User answer: %d\n\r", md_usr_res);
//                    
                    play_feedback(MP3_YOU_ANSWERED);
                    play_number(md_usr_res);
                    md_next_state = STATE_CHECKANS;
                } else {
                    log_msg("[MD13] IO error\n\r");
                }
            }
            break;
            
        case STATE_CHECKANS:
            if (md13_place_number(md_usr_res)){
                // Correct answer
                log_msg("[md13] correct answer\n\r");
                if (!md13_all_entered()){
                    play_feedback(MP3_GOOD_NEXT);
                    md_next_state = STATE_INPUT;
                } else{
                    md_score++;
                    play_feedback(MP3_GOOD);
                    play_feedback(MP3_YOUR_SCORE_IS);
                    play_number(md_score);
                    play_tada();
					md13_choose_ques();
                    md_next_state = STATE_GENQUES;
                }
            }
            else
            {
                // Wrong answer
                md_incorrect_tries += 1;
                play_feedback(MP3_INCORRECT);
                if (md_incorrect_tries >= MAX_INCORRECT_TRIES) {
                    play_feedback(MP3_YOU_MISSED);
                    md13_play_input(NOT_ENTERED);
					md_score = (md_score == 0) ? 0 : md_score - 1;
                    play_feedback(MP3_YOUR_SCORE_IS);
					play_number(md_score);
                    md13_choose_ques();
                    md_next_state = STATE_GENQUES;
                }
                else
				{
                    play_mistake();
					md_next_state = STATE_PROMPT;
				}
            }
            break;
            
        case STATE_REPROMPT:
            md_last_dot = create_dialog(MP3_HELP_MENU_13,
                                        ENTER_CANCEL | LEFT_RIGHT | DOT_1
                                        | DOT_2 | DOT_3 | DOT_4
                                        | DOT_5 | DOT_6);
            switch (md_last_dot) {
                    
                case NO_DOTS:
                    break;
           
                    // Playing answer
                case LEFT:
                    md_next_state = STATE_PROMPT;
                    io_init();
                    break;
                case RIGHT:
                    md13_play_answer();
                    md13_choose_ques();
                    md_next_state = STATE_GENQUES;
                    io_init();
                    break;
                    
                    // Current input
                case ENTER:
                    play_feedback(MP3_YOU_ANSWERED);
                    md13_play_input(ENTERED);
                    play_feedback(MP3_TRY_AGAIN);
                    md_next_state = STATE_INPUT;
                    io_init();
                    break;

                    
                    // EXIT input
                case CANCEL:
                    play_feedback(MP3_EXIT_HELP_MENU);
                    play_feedback(MP3_RET_TO_QUESTION);
                    md_next_state = STATE_INPUT;
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
            log_msg("[MD13] Error: next_state: ");
            SENDBYTE(md_next_state);
            NEWLINE;
            quit_mode();
            break;
    }
}

void md13_call_mode_yes_answer(void) {
    
}

void md13_call_mode_no_answer(void) {
    
}

void md13_call_mode_left(void) {
    
}

void md13_call_mode_right(void) {
    
}

void md13_input_dot(char this_dot) {
    
}

void md13_input_cell(char this_cell) {
    
}