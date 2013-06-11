/**
 * @file MD6.c
 * @brief  Mode 6 code - Free Play
 * @author Vivek Nair (viveknai)
 */

#include "Globals.h"
#include "Modes.h"
#include "audio.h"

/* Change this script header for new script */
#include "script_roman.h"

// State definitions
#define MD6_STATE_INITIAL     0b00000000  // Initial state
#define MD6_STATE_ACCEPT_DOTS 0b00000001  // Waits for dot input from user
#define MD6_STATE_CHECK_DOTS  0b00000010  // Verifies input, gives feedback,
                                          //  returns to input wait loop

/* Change this script pointer for new script */
static script_t* this_script = &script_roman;

static char next_state = MD6_STATE_INITIAL;
static char button_bits = 0b00000000;
static char last_dot = 0;
static char last_cell = 0;
static alphabet_t *this_alpha = NULL;



/**
 * @brief Implements core state machine for free play mode
 * @return void
 */
void md6_main(void) {
  switch (next_state) {
    
    case MD6_STATE_INITIAL:
      PRINTF("MD6 Free Play\n\r");
      request_to_play_mp3_file("MD6INT.MP3");
      next_state = MD6_STATE_ACCEPT_DOTS;
      break;

    case MD6_STATE_ACCEPT_DOTS:
      if (last_dot != 0) {
        // If last button pressed is ENTER, check the dots input so far
        //  otherwise continue to accept more dots
        if (last_dot == ENTER) {
          next_state = MD6_STATE_CHECK_DOTS;
        } else if ((last_dot >= '1') && (last_dot <= '6')) {
          button_bits |= 1 << (CHARTOINT(last_dot) - 1);
          play_dot(NULL, last_dot);
        }
        last_dot = 0;
      }
      break;

    case MD6_STATE_CHECK_DOTS:
      // If user presses ENTER, then check dot sequence for valid letter
      //  and provide feedback
      this_alpha = get_alphabet_by_bits(button_bits, this_script);
      play_alphabet(NULL, this_alpha);
      next_state = MD6_STATE_ACCEPT_DOTS;
	  button_bits = 0;
      break;
  }
}

/**
 * @brief Resets free play mode
 * @return void
 */
void md6_reset(void) {
  next_state = MD6_STATE_INITIAL;
  last_dot = 0;
}

/**
 * @brief Implements ENTER button functionality for free play mode
 * @return void
 */
void md6_call_mode_yes_answer(void) {
  // If no input received, replay prompty, otherwise process as ENTER
  if (button_bits == 0)
    next_state = MD6_STATE_INITIAL;
  else
    last_dot = ENTER;
}

/**
 * @brief Implements CANCEL button functionality for free play mode
 * @return void
 */
void md6_call_mode_no_answer(void) {
  // Does nothing
}

/**
 * @brief Implements dot input functionality for free play mode
 * @return void
 */
void md6_input_dot(char this_dot) {
  last_dot = this_dot;
}

/**
 * @brief Implements cell input functionality for free play mode
 * @return void
 */
void md6_input_cell(char this_cell) {
  if (last_dot != 0) {
    last_cell = this_cell;
  }
}
