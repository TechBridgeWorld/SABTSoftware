/**
 * @file MD10.c
 * @brief Mode 10 code - Learn and practice contracted Braille
 * @author Poornima Kaniarasu (pkaniara)
 */

#include "Globals.h"
#include "Modes.h"
#include "audio.h"
#include "common.h"

int md10_current_state, md10_prev_state = 0;
char md10_last_dot, last_cell, expected_dot;
char *sub_mode[NUM_SUB_MODES] = {"PCON","PABR","DICT"};
static int game_mode = 0;
static int word_num_inset = 0;
static int set = 0;
//static int mistakes = 0;


void md10_main(void) {

 switch(md10_current_state)
  {
     case MD10_STATE_INITIAL:
      play_mp3("MD10","_INT"); // Welcomes and asks to choose a mode (*Practice Contractions *Practice abbrevations *Dictation Mode)
	  play_mp3("MD10","MSEL");
	  game_mode = 0;
      md10_current_state = STATE_SELECT_MODE; 
      got_input = false;
      break;

    case MD10_STATE_SELECT_MODE:
	  break;
 
    case MD10_STATE_REQUEST_INPUT:
	  switch(game_mode){
	  	case 0:
		    play_mp3("MD10","");
			play_mp3("MD10","_WRT"); // to write <word><set>_<num> please press

			char buf[8];
			sprintf(buf,"word%d_%d",set,word_num_inset);			
			play_mp3(NULL,buf);
			play_mp3(NULL,"PRESS");
		
			md10_current_state = MD10_STATE_PRESS;
			break;
		case 1:
			play_mp3("MD10","_PL2");
			break;
		case 2:
			play_mp3("MD10","_PL3");
			break;
	  }
	  md10_current_state = MD10_STATE_WAIT_INPUT;
	  break;

    case MD10_STATE_WAIT_INPUT:	 
      if(got_input)
      {
        got_input = false;		
        md10_current_state = STATE_PROC_INPUT;
      }
      break;

    case MD10_STATE_PROC_INPUT:
	    case 0:
        // set entered_letter in valid_letter(), but return 1 or 0
		md10_current_state = MD10_STATE_REQUEST_INPUT;
     break;

    case STATE_READ_ENTERED_LETTERS:
      break;

    case STATE_CHECK_IF_CORRECT:
      break;

    case STATE_WRONG_INPUT:
      break;

    case STATE_CORRECT_INPUT:
      break;

    case STATE_PLAY_SOUND:
	  md10_current_state = MD10_STATE_REQUEST_INPUT;
	  break;

	case STATE_PROMPT:
	  break;

	case MD10_STATE_PRESS:
		curr_button += 1;

        char bits = get_bits_from_letters();
        char curr_bit = (bits >> (CHARTOINT(curr_button) - 1)) & 1;
        //get the bits for each depending on button count - and play sound if bit is set

        if(curr_bit){
	        play_requested_dot(curr_button);
      	}

      	if(CHARTOINT(curr_button) == NUM_BUT)
      	{
        	md3_current_state = STATE_WAIT_INPUT;
        	curr_button = '0'; 
      	}
	}
}

void md10_reset(void)
{
  md10_current_state = 0;
  md10_last_dot = 0;
}

void md10_call_mode_yes_answer(void) {
	if (md10_current_state == MD10_STATE_SELECT_MODE)
	{
		md10_current_state = MD10_STATE_REQUEST_INPUT;
	}
}

void md10_call_mode_no_answer(void) {
//  md10_current_state = 
}

void md10_input_dot(char this_dot) {
  md10_last_dot = this_dot;
  play_requested_dot(this_dot);	
}

void md10_input_cell(char this_cell) {
  if(md10_last_dot != 0)
  {
    last_cell = this_cell;
    got_input = true;
  }
}

void md10_call_mode_left() {
	if (md10_current_state == MD10_STATE_SELECT_MODE)
	{
		game_mode += 1;
		if (game_mode > NUM_SUB_MODES-1) game_mode = 0;
		play_mp3("MD10",sub_mode[game_mode]);
	}
    else 
	{
		//skip 	
	}		

}

void md10_call_mode_right() {
	if (md10_current_state == MD10_STATE_SELECT_MODE)
	{
		game_mode -= 1;
		if (game_mode < 0) game_mode = NUM_SUB_MODES-1;
		play_mp3("MD10",sub_mode[game_mode]);
	}
    else 
	{
		play_mp3("MD10","NXT");	
	}
}
