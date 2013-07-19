/**
 * @file MD6.c
 * @brief  Mode 6 code - Free Play
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "globals.h"
#include "audio.h"
#include "common.h"
#include "script_common.h"

/* Change this script header for new script */
#include "script_english.h"

// State definitions
#define MD6_STATE_INITIAL   0b00000000  // Initial state
#define MD6_STATE_INPUT     0b00000001  // Waits for dot input from user
#define MD6_STATE_CHECK     0b00000010  // Verifies input, gives feedback,
                                    // returns to input wait loop

#define MAX_INCORRECT_TRIES 3

// Change these script pointers for new script
static script_t* this_script = &script_english;
static char* this_lang_fileset = script_english.fileset;
static char this_mode_fileset[5] = "MD6_";

// State variables
static char next_state = MD6_STATE_INITIAL;
static char button_bits = 0b00000000;
static char last_dot = 0;
static char last_cell = 0;
static glyph_t *this_glyph = NULL;
static char incorrect_tries = 0;

void md6_reset_vars() {
  set_mode_globals(this_script, this_lang_fileset, this_mode_fileset);
  mode_fileset = this_mode_fileset;
  button_bits = 0x00;
  last_dot = 0;
  last_cell = 0;
  this_glyph = NULL;
  incorrect_tries = 0;
  PRINTF("State reset\n\r");
}

void md6_main(void) {
  switch (next_state) {
    
    // Initialises mode
    case MD6_STATE_INITIAL:
      PRINTF("*** MD6 Free Play ***\n\r");
      md6_reset_vars();
      play_mp3(mode_fileset, "INT");
      next_state = MD6_STATE_INPUT;
      break;

    // If last button pressed is ENTER, check the dots input so far
    // otherwise continue to accept more dots
    case MD6_STATE_INPUT:
      if (last_dot != 0) { 
        switch (last_dot) {
          case ENTER:
            next_state = MD6_STATE_CHECK;
            break;

          case CANCEL:
            play_mp3(lang_fileset, "CANC");
            button_bits = 0x00;
            break;

          case '1': case '2': case '3': case '4': case '5': case '6':
            button_bits = add_dot(button_bits, last_dot);
            play_dot(last_dot);
            break;

          default:
            incorrect_tries++;
            play_mp3(lang_fileset, MP3_INVALID_PATTERN);
            if (incorrect_tries >= MAX_INCORRECT_TRIES) {
              incorrect_tries = 0;
              next_state = MD6_STATE_INITIAL;
            }
        }
        last_dot = 0;
      }
      break;

    // If user presses ENTER, then check dot sequence for valid letter
    // and provide feedback
    case MD6_STATE_CHECK:
      this_glyph = search_script(NULL, button_bits);
      play_glyph(this_glyph);
      next_state = MD6_STATE_INPUT;
      button_bits = 0x00;
      break;
  }
}

void md6_reset(void) {
  next_state = MD6_STATE_INITIAL;
}

void md6_call_mode_yes_answer(void) {
  last_dot = ENTER;
}

void md6_call_mode_no_answer(void) {
  last_dot = CANCEL;
}

void md6_input_dot(char this_dot) {
  last_dot = this_dot;
}

void md6_input_cell(char this_cell) {
  if (last_dot != 0) {
    last_cell = this_cell;
  }
}

void md6_call_mode_left(void) {
  last_dot = LEFT;
}

void md6_call_mode_right(void) {
  last_dot = RIGHT;
}
