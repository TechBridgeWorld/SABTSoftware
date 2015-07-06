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

/**
 *  Used to set global fileset variables
 */
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD13"

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

/**
 *  Different levels
 */
#define LEVEL_NULL      0x0000
#define LEVEL_1			0x0001
#define LEVEL_2			0x0002
#define LEVEL_3			0x0003
#define LEVEL_4         0x0004
#define LEVEL_5         0x0005

/**
 *  prompts
 */
#define MP3_LVLSEL "LVLS"  //level select

#define MP3_YOU_ANSWERED "UANS"
// Skip prompt
#define MP3_SKIP "SKIP"
#define MP3_THE_ANSWER_IS "TAIS"
#define MP3_INTRO "INST"
#define MP3_NUMBERS_ARE "NUMR"
#define MP3_AND "AND"
#define MP3_ENTER "ENTR"
#define MP3_SUBMIT "SUMT"
#define MP3_AND_MISTAKES "AMSK"
#define MP3_MISTAKES "MSTK"

/**
 *  Limits
 */
#define MAX_DIGITS 4
#define MAX_INCORRECT_TRIES 3

/**
 *  State variables
 */
static char md_next_state = STATE_NULL;
static char md_last_dot = NO_DOTS;
static int md_level = 0;
static int md_op_1 = -1;
static int md_op_2 = -1;
static int md_res = -1;
static int md_usr_res = -1;
static bool md_input_ready = false;
static bool md_input_valid = false;
static int md_incorrect_tries = 0;

void md13_reset(void) {
    PRINTF("*** MD13 - Math Problems ***\n\r");
    
    // Global variables
    set_mode_globals(&script_digits, LANG_FILESET, MODE_FILESET);
    
    // State variables
    md_next_state = STATE_INTRO;
    md_last_dot = NO_DOTS;
    md_level = LEVEL_NULL;
    md_op_1 = -1;
    md_op_2 = -1;
    md_res = -1;
    md_usr_res = -1;
    md_input_ready = false;
    md_input_valid = false;
}

int md13_generate_number(){
    int res = 0;
    int curt_digit = 0;
    for (int i = 0;i < md_level ; i++) {
        curt_digit = timer_rand() % 10;
        res = res * 10 + curt_digit;
    }
    return res;
}

void md13_generate_question(){
    md_op_1 = md13_generate_number();
    sprintf(dbgstr, "[MD13] Operand 1: %d\n\r", md_op_1);
    PRINTF(dbgstr);
    md_op_2 = md13_generate_number();
    sprintf(dbgstr, "[MD13] Operand 2: %d\n\r", md_op_2);
    PRINTF(dbgstr);
}

void md13_play_question(){
    play_mp3(MODE_FILESET, MP3_NUMBERS_ARE);
    play_number(md_op_1);
    play_mp3(MODE_FILESET, MP3_AND);
    play_number(md_op_2);
	play_mp3(MODE_FILESET, MP3_ENTER);
}

void md13_play_answer(){
    int answer = random_between(md_op_1, md_op_2);
    play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
    play_number(answer);
}

void md13_main(void) {
    switch (md_next_state) {
		case STATE_INTRO:
			play_mp3(MODE_FILESET, MP3_INTRO);
			play_mp3(MODE_FILESET, MP3_SUBMIT);
			md_next_state = STATE_LVLSEL;
			break;
        case STATE_LVLSEL:
            md_last_dot = create_dialog(MP3_LVLSEL,
                                        (DOT_1 | DOT_2 | DOT_3 | DOT_4 ));
            switch (md_last_dot) {
                    
                case NO_DOTS:
                    break;
                    
                case '1':
                    PRINTF("[MD13] Level: 1\n\r");
                    md_level = LEVEL_1;
                    //play_mp3(MODE_FILESET, MP3_SKIP);
                    md_next_state = STATE_GENQUES;
                    break;
                    
                case '2':
                    PRINTF("[MD13] Level: 2\n\r");
                    md_level = LEVEL_2;
                    //play_mp3(MODE_FILESET, MP3_SKIP);
                    md_next_state = STATE_GENQUES;
                    break;
                    
                case '3':
                    PRINTF("[MD13] Level: 3\n\r");
                    md_level = LEVEL_3;
                    //play_mp3(MODE_FILESET, MP3_SKIP);
                    md_next_state = STATE_GENQUES;
                    break;
                
                case '4':
                    PRINTF("[MD13] Level: 4\n\r");
                    md_level = LEVEL_4;
                    //play_mp3(MODE_FILESET, MP3_SKIP);
                    md_next_state = STATE_GENQUES;
                    break;
                    
                default:
                    PRINTF("[MD13] Error: md_last_dot: ");
                    SENDBYTE(md_last_dot);
                    NEWLINE;
                    quit_mode();
                    break;
            }
            break;
        
        case STATE_GENQUES:
            md13_generate_question();
            md_next_state = STATE_PROMPT;
            md_incorrect_tries = 0;
            break;
            
        case STATE_PROMPT:
            md13_play_question();
            md_next_state = STATE_INPUT;
            break;
            
        case STATE_INPUT:
            if (io_user_abort == true) {
                PRINTF("[MD13] User aborted input\n\r");
                md_next_state = STATE_REPROMPT;
                io_init();
                break;
            }
            if (get_number(&md_input_valid, &md_usr_res)) {
                if (md_input_valid) {
                    sprintf(dbgstr, "[MD13] User answer: %d\n\r", md_usr_res);
                    PRINTF(dbgstr);
                    play_mp3(MODE_FILESET, MP3_YOU_ANSWERED);
                    play_number(md_usr_res);
                    md_next_state = STATE_CHECKANS;
                } else {
                    PRINTF("[MD13] IO error\n\r");
                }
            }
            break;
            
        case STATE_CHECKANS:
            if ((md_usr_res >= md_op_1 && md_usr_res <= md_op_2) ||
                 (md_usr_res >= md_op_2 && md_usr_res <= md_op_1))
            {
                // Correct answer
                play_mp3(LANG_FILESET, MP3_CORRECT);
                play_mp3(SYS_FILESET, MP3_TADA);
                md_next_state = STATE_LVLSEL;
            }
            else
            {
                // Wrong answer
                md_incorrect_tries += 1;
                play_mp3(LANG_FILESET, MP3_INCORRECT);
                play_mp3(MODE_FILESET, MP3_AND_MISTAKES);
                play_number(md_incorrect_tries);
                play_mp3(MODE_FILESET, MP3_MISTAKES);
                if (md_incorrect_tries >= MAX_INCORRECT_TRIES)
                {
                    md_next_state = STATE_REPROMPT;
                }
                else
				{
					play_mp3(LANG_FILESET, MP3_TRY_AGAIN);
					md_next_state = STATE_PROMPT;
				}
            }
            break;
            
        case STATE_REPROMPT:
            md_last_dot = create_dialog(MP3_SKIP,
                                        ENTER_CANCEL | LEFT_RIGHT);
            switch (md_last_dot) {
                    
                case NO_DOTS:
                    break;
                    
                    // Playing answer
                case RIGHT:
                    md13_play_question();
                    md_next_state = STATE_GENQUES;
                    break;
                case LEFT:
                    md13_play_answer();
                    md_next_state = STATE_GENQUES;
                    break;
                    
                    // Skipping question
                case ENTER:
                    md_next_state = STATE_GENQUES;
                    break;
                    
                    // Try again
                case CANCEL:
                    md_next_state = STATE_PROMPT;
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            PRINTF("[MD13] Error: next_state: ");
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