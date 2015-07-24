/**
 * @file MD6.c
 * @brief  Mode 6 code - Free Play
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "globals.h"
#include "audio.h"
#include "common.h"
#include "script_common.h"
#include "MD6.h"
 #include "mp3s.h"

/* Change this script header for new script */
#include "script_english.h"

#define MAX_INCORRECT_TRIES 3

// Change these script pointers for new script
static script_t* this_script = &script_english;
static char* this_lang_fileset = script_english.fileset;
static char this_mode_fileset[5] = "MD6_";

// State variables
static char next_state = MD6_STATE_INITIAL;
static glyph_t *this_glyph = NULL;
static char incorrect_tries = 0;
static char cell = 0;
static char cell_pattern = 0;
static char cell_control = 0;


void md6_reset(void) {
	set_mode_globals(this_script, this_lang_fileset, this_mode_fileset);
	mode_fileset = this_mode_fileset;
	this_glyph = NULL;
	incorrect_tries = 0;
	cell = 0;
	cell_pattern = 0;
	cell_control = 0;
	play_welcome();
	next_state = MD6_STATE_INPUT;
	log_msg("[MD6] Mode reset\n\r");
}

void md6_main(void) {
  switch (next_state) {
    

    // Let the user input a cell
    case MD6_STATE_INPUT:
	  cell = get_cell();
	  if (cell == NO_DOTS) {
		  break;
	  }
	  cell_pattern = GET_CELL_PATTERN(cell);
	  cell_control = GET_CELL_CONTROL(cell);
	  switch (cell_control) {
		  case WITH_ENTER:
		  this_glyph = search_script(this_script, cell_pattern);
		  next_state = MD6_STATE_CHECK;
		  break;
		  case WITH_LEFT:
		  break;
		  case WITH_RIGHT:
		  break;
		  case WITH_CANCEL:
		  break;
	  }
	  break;

    // If user presses ENTER, then check dot sequence for valid letter
    // and provide feedback
    case MD6_STATE_CHECK:
	  if (this_glyph == NULL) {
		play_mp3(this_lang_fileset, MP3_INVALID_PATTERN);
	  } else {
		play_glyph(this_glyph);
	  }
      next_state = MD6_STATE_INPUT;
      break;
  }
}