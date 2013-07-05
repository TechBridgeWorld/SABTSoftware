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
static int game_mode = 0;
char md10_last_dot, last_cell, expected_dot;
//static int mistakes = 0;


void md10_main(void) {

 switch(md10_current_state)
  {
     case STATE_INITIAL:
      play_mp3("MD10","_INT"); // Welcomes and asks to choose a mode A or B
	  game_mode = 0;
      md10_current_state = STATE_SELECT_MODE; 
      got_input = false;
      break;

    case STATE_SELECT_MODE:
	  play_mp3("MD10","MSEL");
	  md10_current_state = STATE_WAIT_INPUT;
	  break;
 
    case STATE_REQUEST_INPUT1:
	  if (game_mode == 1) play_mp3("MD10","PLSA");
	  else if (game_mode == 2) play_mp3("MD10","PLSB");

      md10_current_state = STATE_REQUEST_INPUT2;
      break;

    case STATE_REQUEST_INPUT2:

      md10_current_state = STATE_WAIT_INPUT;
      break;

    case STATE_WAIT_INPUT:
	 
      if(got_input)
      {
        got_input = false;		
        md10_current_state = STATE_PROC_INPUT;
      }
      break;

    case STATE_PROC_INPUT:
      // set entered_letter in valid_letter(), but return 1 or 0
	  
 
		md10_current_state = STATE_REQUEST_INPUT1;
     break;

    case STATE_READ_ENTERED_LETTERS:
      break;

    case STATE_CHECK_IF_CORRECT:
      break;

    case STATE_WRONG_INPUT:
      break;

    case STATE_CORRECT_INPUT:
      break;

    case STATE_DONE_WITH_CURRENT_ANIMAL:
	  play_mp3(NULL,"good");
	  play_mp3(NULL,"nc_wrk");
	  md10_current_state = STATE_PLAY_SOUND; 
      break;

    case STATE_PLAY_SOUND:
	  md10_current_state = STATE_REQUEST_INPUT1;
	  break;
	case STATE_PROMPT:
	  break;

	}
}

void md10_reset(void)
{
  md10_current_state = 0;
  md10_last_dot = 0;
}

void md10_call_mode_yes_answer(void) {
	

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
	
}

void md10_call_mode_right() {
//  play_mp3(MD10,NXT);	
}


