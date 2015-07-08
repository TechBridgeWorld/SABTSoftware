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
#define QEST_BETWEEN    0x000
#define QEST_NEXT_NUM   0x004
#define QEST_EVEN_NUM   0x005
#define QEST_ODD_NUM    0x002
#define QEST_TENS_NUM   0x003
#define QEST_HUNDS_NUM  0x007
#define QEST_ROUND_TEN  0x001
#define QEST_ROUND_HUND 0x006

#define FIRST_QUES 0x00
#define FINAL_QUES 0x01

/**
 *  prompts
 */
#define MP3_LVLSEL "LVLS"  //level select

#define MP3_YOU_ANSWERED "UANS"
// Skip prompt
#define MP3_SKIP "SKIP"
#define MP3_THE_ANSWER_IS "TAIS"
#define MP3_INTRO "INST"
#define MP3_AND "AND"
#define MP3_ENTER "ENTR"
#define MP3_SUBMIT "SUMT"
#define MP3_AND_MISTAKES "AMSK"
#define MP3_MISTAKES "MSTK"
#define MP3_BETWEEN "BETW"
#define MP3_NEXT "NEXN"
#define MP3_ODD "ODDN"
#define MP3_EVEN "EVEN"
#define MP3_TEN "TENN"
#define MP3_HUND "HUNN"
#define MP3_TENR "TENR"
#define MP3_HUNR "HUNR"
#define MP3_BONO "BONO"

/**
 *  Limits
 */
#define MAX_DIGITS 4
#define MAX_INCORRECT_TRIES 3
#define QUES_TYPES 7
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
static int md_usr_first_res = -1;
static bool md_input_ready = false;
static bool md_input_valid = false;
static int md_incorrect_tries = 0;
static int md_question = QEST_BETWEEN;
static int md_game_status = FIRST_QUES;


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
    md_question = QEST_BETWEEN;
    md_game_status = FIRST_QUES;
}

void md13_generate_question(){
    switch (md_level) {
        case LEVEL_1:
            md_op_1 = random_between(1,49);
            sprintf(dbgstr, "[MD13] Operand 1: %d\n\r", md_op_1);
            PRINTF(dbgstr);
            md_op_2 = random_between(51,100);
            sprintf(dbgstr, "[MD13] Operand 2: %d\n\r", md_op_2);
            PRINTF(dbgstr);
            break;
        case LEVEL_2:
            md_op_1 = random_between(1,49);
            md_op_1 *= 100;
            sprintf(dbgstr, "[MD13] Operand 1: %d\n\r", md_op_1);
            PRINTF(dbgstr);
            md_op_2 = random_between(51,100);
            md_op_2 *= 100;
            sprintf(dbgstr, "[MD13] Operand 2: %d\n\r", md_op_2);
            PRINTF(dbgstr);
            break;
        default:
            PRINTF("[MODE13]Error generating question");
            break;
    }
    
    
}

void md13_play_question(){
    switch (md_question) {
        case QEST_NEXT_NUM:
            play_mp3(MODE_FILESET, MP3_NEXT);
            play_number(md_usr_first_res);
            break;
            
        case QEST_BETWEEN:
            play_mp3(MODE_FILESET, MP3_BETWEEN);
            play_number(md_op_1);
            play_mp3(MODE_FILESET, MP3_AND);
            play_number(md_op_2);
            break;
            
        case QEST_EVEN_NUM:
            play_mp3(MODE_FILESET, MP3_EVEN);
            play_number(md_op_1);
            play_mp3(MODE_FILESET, MP3_AND);
            play_number(md_op_2);
            break;
            
        case QEST_ODD_NUM:
            play_mp3(MODE_FILESET, MP3_ODD);
            play_number(md_op_1);
            play_mp3(MODE_FILESET, MP3_AND);
            play_number(md_op_2);
            break;
            
        case QEST_TENS_NUM:
            play_mp3(MODE_FILESET, MP3_TEN);
            play_number(md_usr_first_res);
            break;
            
        case QEST_HUNDS_NUM:
            play_mp3(MODE_FILESET, MP3_HUND);
            play_number(md_usr_first_res);
            break;
        
        case QEST_ROUND_TEN:
			play_mp3(MODE_FILESET,MP3_ENTER);
			play_number(md_usr_first_res);
            play_mp3(MODE_FILESET, MP3_TENR);
            
            break;
            
        case QEST_ROUND_HUND:
			play_mp3(MODE_FILESET,MP3_ENTER);
			play_number(md_usr_first_res);
            play_mp3(MODE_FILESET, MP3_HUNR);
            
            break;
            
        default:
            break;
    }
    
}

void md13_play_answer(){
    int answer;
    switch (md_question) {
        case QEST_BETWEEN:
            answer = random_between(md_op_1, md_op_2);
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            break;
        
        case QEST_NEXT_NUM:
            answer = md_usr_first_res + 1;
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            break;
            
        case QEST_EVEN_NUM:
            answer = random_between(md_op_1, md_op_2);
            while (answer % 2 == 1) {
                answer = random_between(md_op_1, md_op_2);
            }
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            break;
            
        case QEST_ODD_NUM:
            answer = random_between(md_op_1, md_op_2);
            while (answer % 2 == 0) {
                answer = random_between(md_op_1, md_op_2);
            }
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            break;
            
        case QEST_TENS_NUM:
            answer = md_usr_first_res / 10;
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            break;
            
        case QEST_HUNDS_NUM:
            answer = md_usr_first_res / 100;
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            break;
            
        case QEST_ROUND_TEN:
            if (md_usr_first_res % 10 <= 5)
            {
                answer = md_usr_first_res / 10 * 10;
            } else
            {
                answer = md_usr_first_res / 10 * 10 + 10;
            }
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            
            break;
            
        case QEST_ROUND_HUND:
            if (md_usr_first_res % 100 <= 50)
            {
                answer = md_usr_first_res / 100 * 100;
            } else
            {
                answer = md_usr_first_res / 100 * 100 + 100;
            }
            play_mp3(MODE_FILESET, MP3_THE_ANSWER_IS);
            play_number(answer);
            
            break;
            
        default:
            break;
    }
}

void choose_ques(){
    md_question = random_between(1,QUES_TYPES);
	char buf[10];
	sprintf(buf,"[mode]q%d",md_question );
	PRINTF(buf);
    return;
}

bool check_answer(){
    switch (md_question) {
        case QEST_BETWEEN:
            if ((md_usr_res > md_op_1 && md_usr_res < md_op_2) ||
                (md_usr_res > md_op_2 && md_usr_res < md_op_1)) return true;
            else return false;
            
        case QEST_NEXT_NUM:
            if (md_usr_res == md_usr_first_res + 1)   return true;
            else return false;
            
        case QEST_EVEN_NUM:
            if (((md_usr_res > md_op_1 && md_usr_res < md_op_2) ||
                (md_usr_res > md_op_2 && md_usr_res < md_op_1)) &&
                (md_usr_res % 2 == 0))  return true;
            else return false;
            
        case QEST_ODD_NUM:
            if (((md_usr_res > md_op_1 && md_usr_res < md_op_2) ||
                (md_usr_res > md_op_2 && md_usr_res < md_op_1)) &&
                (md_usr_res % 2 == 1)) return true;
            else return false;
            
        case QEST_TENS_NUM:
            if (md_usr_res == md_usr_first_res / 10) return true;
            else return false;
            
        case QEST_HUNDS_NUM:
            if (md_usr_res == md_usr_first_res / 100) return true;
			else return false;
            
        case QEST_ROUND_TEN:
            if (md_usr_first_res % 10 <= 5)
            {
                if (md_usr_res == md_usr_first_res / 10 * 10) return true;
                else return false;
            } else
            {
                if (md_usr_res == md_usr_first_res / 10 * 10 + 10) return true;
                else return false;
            }
        case QEST_ROUND_HUND:
            if (md_usr_first_res % 100 <= 50)
            {
                if (md_usr_res == md_usr_first_res / 100 * 100) return true;
                else return false;
            } else
            {
                if (md_usr_res == md_usr_first_res / 100 * 100 + 100) return true;
                else return false;
            }
        default:
			PRINTF("[MODE]check answer\r\n");
			return false;
            break;
    }
}

void md13_main(void) {
    switch (md_next_state) {
		case STATE_INTRO:
            md_last_dot = create_dialog(MP3_INTRO,
                                        ( DOT_1 | DOT_2 ));
            switch (md_last_dot) {
                    
                case NO_DOTS:
                    break;
                    
                case '1':
                    PRINTF("[MD13] Level: 2\n\r");
                    md_level = LEVEL_1;
                    play_mp3(MODE_FILESET, MP3_SUBMIT);
                    md_next_state = STATE_GENQUES;
                    break;
                    
                case '2':
                    PRINTF("[MD13] Level: 3\n\r");
                    md_level = LEVEL_2;
                    play_mp3(MODE_FILESET, MP3_SUBMIT);
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
            md_game_status = FIRST_QUES;
            md_question = QEST_BETWEEN;
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
                    if (md_game_status == FIRST_QUES) md_usr_first_res = md_usr_res;
                } else {
                    PRINTF("[MD13] IO error\n\r");
                }
            }
            break;
            
        case STATE_CHECKANS:
            if (check_answer())
            {
                // Correct answer
                play_mp3(LANG_FILESET, MP3_CORRECT);
                if (md_game_status == FIRST_QUES)
                {
                    md_incorrect_tries = 0;
                    md_next_state = STATE_FOLLOWUP;
                }
                else
                {
                    play_mp3(SYS_FILESET, MP3_TADA);
                    md_next_state = STATE_GENQUES;
                }
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
            
        case STATE_FOLLOWUP:
            md_last_dot = create_dialog(MP3_BONO, ENTER_CANCEL);
            switch (md_last_dot) {
                case NO_DOTS:
                    break;
                case ENTER:
                    md_game_status++;
                    choose_ques();
                    md_next_state = STATE_PROMPT;
                    break;
                case CANCEL:
                    md_game_status++;
                    md_next_state = STATE_GENQUES;
                    break;
                default:
                    PRINTF("[MODE]Follow up\r\n");
                    break;
            }
            break;
            
        case STATE_REPROMPT:
            md_last_dot = create_dialog(MP3_SKIP,
                                        ENTER_CANCEL | LEFT_RIGHT);
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
                    md_next_state = STATE_GENQUES;
                    io_init();
                    break;
                    
                    // Skipping question
                case ENTER:
                    md_next_state = STATE_GENQUES;
                    break;
                    
                    // Restart the mode
                case CANCEL:
                    md_next_state = STATE_INTRO;
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