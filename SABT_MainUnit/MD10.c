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
 #include "mp3s.h"


int md10_current_state, md10_prev_state = 0;
char md10_last_dot, last_cell, expected_dot;
char *sub_mode[NUM_SUB_MODES] = {"PCON","PABR","DICT"};
static int game_mode = 0;
static int word_num_inset = 20;
static int set = 0;
struct dir_Structure *location;
static char cell;
static char fname[12];
static glyph_t* g1;
static glyph_t* g2;

static char cell1_pattern;
static char cell2_pattern;
static char cell_control;

void md10_main(void) {

 switch(md10_current_state) {
     case MD10_STATE_INITIAL:
     	play_welcome();
     	play_submode_choice();
	  game_mode = 0;
	  lang_fileset = script_eng_contraction.fileset;
	  log_msg(lang_fileset);
      md10_current_state = MD10_STATE_SELECT_MODE; 
      got_input = false;
      break;

    case MD10_STATE_SELECT_MODE:
	  break;
 
 	case MD10_STATE_SUBMODE_INIT:
	  switch(game_mode){
		case 0:
			break;
		case 1:
			// Welcomes the user to the submode 2
			play_direction(MP3_UNDER_DEVEL);
			break;
		case 2:
			// Welcomes the user to the submode 1
			play_direction(MP3_UNDER_DEVEL);
			break;
		}
		md10_current_state = MD10_STATE_REQUEST_WRITE;
		break;

	case MD10_STATE_REQUEST_WRITE:
		play_direction(MP3_PLEASE_WRITE);
		md10_current_state = MD10_STATE_REQUEST_INPUT;
		if (word_num_inset == 36){
			set = set + 1;
			word_num_inset = 0;
		}
	    else
			word_num_inset = word_num_inset + 1;
		break;

    case MD10_STATE_REQUEST_INPUT:
	  switch(game_mode){
	  	case 0:
		    // to write <word><set>_<num> please press
	  		sprintf(fname, "c_%d_w_%d.mp3", set, word_num_inset-1);
			log_msg("%s\n\r", fname);
			
			bool e = convert_file_name ((unsigned char*)fname); //convert file_name into FAT format
  			if(e) return;
			location = find_files(GET_FILE,(unsigned char*)fname);
			
			if (!location) {
			    if (word_num_inset == 36){
					set = set + 1;
					word_num_inset = 0;
				}
				else {
					word_num_inset = word_num_inset + 1;
				}
				return;
			}
			play_mp3(NULL, fname);
			play_direction(MP3_PRESS_DOTS);
					
			md10_current_state = MD10_STATE_SPELL_PATTERN;
			break;
		case 1:
			break;
		case 2:
			break;
	  }
	  break;

	case MD10_STATE_SPELL_PATTERN:
	  if(fname[1] != '0'){
	    g1 = &contraction_pattern[CHARTOINT(fname[0])];   // Stores the preceding pattern for the cell
		play_dot_sequence(g1);
		play_direction(MP3_NEXT_CELL);
		}
	  else g1 = NULL;
	  int sym;
	  if (strlen(fname) == 6)
	  	sym = 10*CHARTOINT(fname[4]) + CHARTOINT(fname[5]);
	  else
	  	sym = CHARTOINT(fname[4]);			// Stores the alphabet/symbol glyph
	  g2 = &contraction_glyphs[sym-1];
   	  play_dot_sequence(g2);
	  md10_current_state = MD10_STATE_CELL1;
	  io_init();
	  break;
		
    case MD10_STATE_CELL1:
	  if (!g1){
	    md10_current_state = MD10_STATE_CELL2;
		break;
      }
	  cell = get_cell();
	  cell1_pattern = GET_CELL_PATTERN(cell);
	  cell_control = GET_CELL_CONTROL(cell);
	  if (cell1_pattern == NO_DOTS)
	  	break;
	  switch (cell_control) {
	    case WITH_ENTER:
		  log_msg("ENTER");
		  md10_current_state = MD10_STATE_CHECK;		  
		  break;
		case WITH_LEFT:
		  log_msg("LEFT");
		  play_direction(MP3_NEXT_CELL);
		  md10_current_state = MD10_STATE_CELL2;
		  break;
		case WITH_RIGHT:
		  log_msg("RIGHT");
		  play_mp3(MP3_ENGLISH, MP3_BLANK);
		  cell1_pattern = NO_DOTS;		  
		  break;
		case WITH_CANCEL:
		  log_msg("User pressed CANCEL\n\r");
		  break;
	  }
	  break;

	
	case MD10_STATE_CELL2:		
	  cell = get_cell();
	  cell2_pattern = GET_CELL_PATTERN(cell);
	  cell_control = GET_CELL_CONTROL(cell);

	  if (cell2_pattern == NO_DOTS)
		  break;

	  switch (cell_control) {
	    case WITH_ENTER:
		  log_msg("ENTER");
		  md10_current_state = MD10_STATE_CHECK;		  
		  break;
		case WITH_LEFT:
		  log_msg("LEFT");
		  break;
		case WITH_RIGHT:
		  log_msg("RIGHT");
		  if (cell2_pattern && g1){		
		  	play_mode_audio(MP3_CELL_1); 
		  	play_pattern((unsigned char)cell1_pattern);
		  	play_mode_audio(MP3_CELL_2); 
		  	cell2_pattern = NO_DOTS;
		  } else {
		  play_mp3(MP3_ENGLISH, MP3_BLANK);
			cell1_pattern = NO_DOTS;
			cell2_pattern = NO_DOTS;
			md10_current_state = MD10_STATE_CELL1;
		  }		  
		  break;
		case WITH_CANCEL:
		  log_msg("User pressed CANCEL");
		  break;
	  }
	  break;

	case MD10_STATE_CHECK:
	  if(g1==NULL || cell1_pattern == g1->pattern){
	      if(cell2_pattern == g2->pattern){
	      	play_feedback(MP3_GOOD);
			md10_current_state = MD10_STATE_REQUEST_WRITE;
		  }
	      else{
	      	play_feedback(MP3_NO);
	      	play_feedback(MP3_TRY_AGAIN);
			cell1_pattern = NO_DOTS;
			cell2_pattern = NO_DOTS;
			md10_current_state = MD10_STATE_REQUEST_INPUT;
		 }
	  }        
      break;
	}
}

void md10_reset(void)
{
  game_mode = 0;
  word_num_inset = 0;
  set = 0;
  md10_current_state = MD10_STATE_INITIAL;
  cell1_pattern = NO_DOTS;
  cell2_pattern = NO_DOTS;
  g1 = NULL;
  g2 = NULL;
}

void md10_call_mode_yes_answer(void) {
	if (md10_current_state == MD10_STATE_SELECT_MODE)
		md10_current_state = MD10_STATE_SUBMODE_INIT;
}

void md10_call_mode_no_answer(void) {	
    md10_reset(); // Pressing CANCEL exits to submode menu
}

void md10_input_dot(char this_dot) {
  md10_last_dot = this_dot;
  play_requested_dot(this_dot);	
}

void md10_input_cell(char this_cell) {
  if(md10_last_dot != 0) {
    last_cell = this_cell;
    got_input = true;
  }
}

void md10_call_mode_left() {
	if (md10_current_state == MD10_STATE_SELECT_MODE) {
		game_mode += 1;
		if (game_mode > NUM_SUB_MODES-1) game_mode = 0;
		play_mp3("MD10",sub_mode[game_mode]);
	}		
}

void md10_call_mode_right() {
	if (md10_current_state == MD10_STATE_SELECT_MODE) {
		game_mode -= 1;
		if (game_mode < 0) game_mode = NUM_SUB_MODES-1;
		play_mp3("MD10",sub_mode[game_mode]);
	}
}
