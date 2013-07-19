/**
 * @file MD10.c
 * @brief Mode 10 code - Learn and practice contracted Braille
 * @author Poornima Kaniarasu (pkaniara)
 */

#include "Globals.h"
#include "Modes.h"
#include "audio.h"
#include "common.h"
#include "script_eng_contraction.h"

int md10_current_state, md10_prev_state = 0;
char md10_last_dot, last_cell, expected_dot;
char *sub_mode[NUM_SUB_MODES] = {"PCON","PABR","DICT"};
static int game_mode = 0;
static int word_num_inset = 0;
static int set = 0;
//static int mistakes = 0;
struct dir_Structure *location;
static char buf[8];

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
 
 	case MD10_STATE_SUBMODE_INIT:
	  switch(game_mode){
		case 0:
			play_mp3("MD10","WEL1");
			break;
		case 1:
			play_mp3("MD10","WEL2");
			break;
		case 2:
			play_mp3("MD10","WEL3");
			break;
		}
		md10_current_state = MD10_STATE_REQUEST_INPUT;
		break;

    case MD10_STATE_REQUEST_INPUT:
	  switch(game_mode){
	  	case 0:		    
			play_mp3("MD10","_WRT"); // to write <word><set>_<num> please press
			sprintf(buf,"CON%d_W%d",set,word_num_inset);
			PRINTF(buf);
			location = find_files(GET_FILE,(unsigned char*)buf);
			if (!location) return;			
			play_mp3(NULL,buf);
			play_mp3(NULL,"PRESS");
		//	location? PRINTF("YES") : PRINTF("NO");		
			md10_current_state = MD10_STATE_CELL1;
			break;
		case 1:
			play_mp3("MD10","_PL2");
			break;
		case 2:
			play_mp3("MD10","_PL3");
			break;
	  }
//	  md10_current_state = MD10_STATE_WAIT_INPUT;
	  break;
	case MD10_STATE_CELL1:
	  if(buf[3] != '0'){
		play_dot_sequence(&contraction_pattern[CHARTOINT(buf[3])]);
		play_mp3("MD10","PRES");
		md10_current_state = MD10_STATE_WAIT_INPUT1;
		}
	  break;
		
    case MD10_STATE_WAIT_INPUT1:
	  PRINTF("waiting for second cell1 input");
	  char cell1 = get_cell(); 
      if(cell1)
      {
	    
        if(1){// check if the bits are same as the pattern contraction_pattern[CHARTOINT(pat)].pattern){
			play_mp3(NULL,"GOOD");
			play_mp3("MD10","_NXT");
			md10_current_state = MD10_STATE_CELL2;
		}
		else{
			play_mp3(NULL,"no");
		}
      }
	  else return;
      break;

	
	case MD10_STATE_CELL2:
	  PRINTF("Please input cell 2 content");
	  int sym;
	  if (strlen(buf) == 7){
	  	sym = 10*CHARTOINT(buf[5])+CHARTOINT(buf[6]);
	  }
	  else {
	  	sym = CHARTOINT(buf[5]);
	  }

   	  play_dot_sequence(&contraction_glyphs[sym]);
	  play_mp3("MD10","PRES");
	  md10_current_state = MD10_STATE_WAIT_INPUT2;
	  break;

	case MD10_STATE_WAIT_INPUT2:
	  PRINTF("waiting for second cell2 input");
	  char cell2 = get_cell(); 
      if(cell2)
      {
        if(cell2){// check if the bits are same as the pattern contraction_pattern[CHARTOINT(pat)].pattern){
			play_mp3(NULL,"GOOD");
			md10_current_state = MD10_STATE_REQUEST_INPUT;
		}
		else{
			play_mp3(NULL,"no");
		}
      }
	  else return;
      break;
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
		play_mp3("MD10","NXT");	 	
	}		

}

void md10_call_mode_right() {
	if (md10_current_state == MD10_STATE_SELECT_MODE)
	{
		game_mode -= 1;
		if (game_mode < 0) game_mode = NUM_SUB_MODES-1;
		play_mp3("MD10",sub_mode[game_mode]);
	}
}
