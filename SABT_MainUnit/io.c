/**
 * @file io.c
 * @brief Definitions for common IO library
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "io.h"
#include "common.h"
#include "globals.h"
#include "audio.h"
#include "script_common.h"
#include "letter_globals.h"
#include "script_digits.h"

#include <stdbool.h>

// **************************************
// ********** Global variables **********
// **************************************

// Public IO variables
char io_dot = NO_DOTS;
char io_cell = NO_DOTS;
char io_line[MAX_BUF_SIZE] = "";
glyph_t* io_parsed[MAX_BUF_SIZE] = {NULL};
bool io_user_cancel = false;
bool io_user_abort = false;

// Basic IO state variables
static char io_cell_state = NO_DOTS;
static unsigned int io_line_cell_index = 0;
static bool io_line_ready = false;

// Advanced IO state variables
// Dialog state variables
static bool io_dialog_initialised = false;
static bool io_dialog_dots_enabled[6] =
	{false, false, false, false, false, false};
static bool io_dialog_enter_cancel_enabled = false;
static bool io_dialog_left_right_enabled = false;
static int io_dialog_incorrect_tries = -1;

// **************************************************
// ********** Helper function declarations **********
// **************************************************

// Basic IO helper funcitons
void io_line_next_cell(void);
void io_line_prev_cell(void);
void io_line_clear_cell(void);
void io_line_reset(void);

// Intermediate IO helper functions
bool io_convert_line(void);
bool io_parse_number(int* res);

// Advanced IO helper functions
void io_dialog_init(char control_mask);
void io_dialog_reset(void);
void io_dialog_error(void);


// ******************************
// ********** Basic IO **********
// ******************************

/**
* @brief Resets IO state variables
* @param void
* @return void
*/
void io_init(void) {
	io_dot = NO_DOTS;
	io_cell = NO_DOTS;
	io_user_cancel = false;
	io_user_abort = false;
	io_line_reset();
	io_dialog_reset();
}

/**
* @brief Gets current dot and then sets it to NO_DOTS
* @param void
* @return char - Current dot
*/
char get_dot(void) {
	play_dot(io_dot);
	char ret_val = io_dot;
	io_dot = NO_DOTS;
	return ret_val;
}

/**
*	@brief Gets current cell
* @param void
* @return char - Cell pattern with 2 MSB as controls for ENTER, LEFT, RIGHT
* terminated cell entry, WITH_CANCEL for CANCEL and NO_DOTS when nothing to
* return (wait)
*/
char get_cell(void) {
	char last_dot = get_dot();
	char ret_val = NO_DOTS;

	switch(last_dot) {

		case NO_DOTS:
			return NO_DOTS;
			break;
		
		// If one of the dots, then add to cell state
		case '1': case '2': case '3': case '4':	case '5': case '6':
			io_cell_state = add_dot(io_cell_state, last_dot);
			return NO_DOTS;
			break;

		// If a control is pressed, return cell state with control preserved in
		// 2 MSB of char
		case ENTER: case LEFT: case RIGHT: case CANCEL:
			ret_val = io_cell_state;
			io_cell_state = NO_DOTS;
			switch (last_dot) {
				case ENTER:
					io_user_cancel = false;
					io_user_abort = false;
					sprintf(dbgstr, "[IO] Cell pattern: %x\n\r",
						ret_val | WITH_ENTER);
					PRINTF(dbgstr);
					return ret_val | WITH_ENTER;
					break;
				case LEFT:
					io_user_cancel = false;
					io_user_abort = false;
					return ret_val | WITH_LEFT;
					break;
				case RIGHT:
					io_user_cancel = false;
					io_user_abort = false;
					return ret_val | WITH_RIGHT;
					break;
				case CANCEL:
					if (ret_val == 0x00) {
						if (io_user_cancel == true) {
							io_user_abort = true;
							io_user_cancel = false;
						} else {
							io_user_cancel = true;
							io_user_abort = false;
						}
					}
					return ret_val | WITH_CANCEL;
					break;
				// Should not execute this code
				default:
					sprintf(dbgstr, "[IO] Invalid dot: %x\n\r", last_dot);
					PRINTF(dbgstr);
					quit_mode();
					return NO_DOTS;
					break;
			}
			break;

		// Should not execute this code
		default:
			sprintf(dbgstr, "[IO] Invalid dot: %x\n\r", last_dot);
			PRINTF(dbgstr);
			quit_mode();
			return NO_DOTS;
			break;
	}
}

/**
* @brief Gets a line of raw cells from the user
* @param void
* @return bool - true if io_line is ready for further processing, false
* otherwise
*/
bool get_line(void) {
	if (io_line_ready == false) {
		io_line_reset();
	}

	if (io_user_abort) {
		return false;
	}

	char last_cell = get_cell();
	char pattern = GET_CELL_PATTERN(last_cell);
	char control = GET_CELL_CONTROL(last_cell);

	// If last cell was empty, there's nothing to do so return false and wait
	if (last_cell == NO_DOTS) {
		return false;
	}

	// Otherwise, if cell was recorded, save it to io_line
	if (pattern) {
		io_line[io_line_cell_index] = pattern;
	}
	switch (control) {
		// ENTER - Advance cell, add EOT and return true
		case WITH_ENTER:
			io_line_next_cell();
			io_line[io_line_cell_index] = END_OF_TEXT;
			io_line_ready = false;
			return true;
			break;

		/* RIGHT is used to switch to previous cell because Braille is entered
			this way */
		// RIGHT - Select prev cell
		case WITH_RIGHT:
			play_mp3(lang_fileset, MP3_PREV_CELL);
			io_line_prev_cell();
			if (!pattern) {
				play_pattern(io_line[io_line_cell_index]);
			}
			return false;
			break;

		/* LEFT is used to switch to next cell because Braille is entered
			this way */
		// LEFT - Select next cell
		case WITH_LEFT:
			play_mp3(lang_fileset, MP3_NEXT_CELL);
			io_line_next_cell();
			if (!pattern) {
				play_pattern(io_line[io_line_cell_index]);
			}
			return false;
			break;

		// CANCEL - Clear current cell
		case WITH_CANCEL:
			io_line_clear_cell();
			return false;
			break;

		// Should not execute this code
		default:
			sprintf(dbgstr, "[IO] Invalid control: %x\n\r", control);
			PRINTF(dbgstr);
			quit_mode();
			return false;
			break;
	}
}

// *************************************
// ********** Intermediate IO **********
// *************************************

/**
* @brief Gets a line from the user, converts to glyphs, converts to number
* @param bool* - Address to put valid flag for input
* @param int* - Address to put resultant integer
* @return bool - true once user input is complete and parsed, false if pending
*/
bool get_number(bool* valid, int* res) {
	// Pending completion of user input
	if (!get_line()) {
		return false;
	}

	// If cell sequence has invalid patterns, return false
	if (!io_convert_line()) {
		play_mp3(lang_fileset, MP3_INVALID_PATTERN);
		*valid = false;
		return true;
	}

	// If cell sequence successfully parsed, return true
	if (io_parse_number(res)) {
		*valid = true;
		return true;
	} else {
		*valid = false;
		return true;
	}
}

/**
* @brief Gets a line from the user, converts to glyphs, returns first glyph
* @param glyph_t* res - Pointer to placeholder for pointer
* @return bool - true if ready for further processing
*/
bool get_first_glyph(glyph_t** res) {
	
	// Let user finish input
	if (!get_line()) {
		return false;
	}

	PRINTF("[IO] Line accepted\n\r");

	/*
	If conversion is unsuccessful, return NULL, otherwise return first
	character
	*/
	if (!io_convert_line()) {
		PRINTF("[IO] Line conversion unsuccessful\n\r");
		play_mp3(lang_fileset, MP3_INVALID_PATTERN);
		*res = NULL;
		return true;
	} else {
        PRINTF("io_convert");
        sprintf(dbgstr, "[IO] Returning character: %s\n\r", io_parsed[0]->sound);
        PRINTF(dbgstr);
		*res = io_parsed[0];
		
		return true;
	}
}

bool get_character(bool* valid, char* character){
    // Let user finish input
    if (!get_line()) {
        return false;
    }
    
    PRINTF("[IO] Line accepted\n\r");
    
    /*
     If conversion is unsuccessful, return NULL, otherwise return first
     character
     */
    if (!io_convert_line()) {
        PRINTF("[IO] Line conversion unsuccessful\n\r");
        play_mp3(lang_fileset, MP3_INVALID_PATTERN);
        *valid = false;
        return true;
    } else {
        PRINTF("io_convert");
        sprintf(dbgstr, "[IO] Returning character: %s\n\r", io_parsed[0]->sound);
        PRINTF(dbgstr);
        *character = get_letter_from_bits(io_parsed[0]->pattern);
        if(character == '\0') *valid = false;
        else *valid = true;
        return true;
    }
}

// ********************************
// ********* Advanced IO **********
// ********************************

/**
* @brief Creates a dialog with prompt and specified allowed user inputs
* @param char* prompt - Pointer to filename for prompt to play - prefixed with
* mode fileset
* @param char control_mask - Controls to allow on dialog re: io.h for masks
* @return char - Button corresponding to user input
*/
char create_dialog(char* prompt, char control_mask) {

	// Gets initialised on first call after a successful return or first run
	if (io_dialog_initialised == false) {
		sprintf(dbgstr, "[IO] Creating dialog: %s\n\r", prompt);
		PRINTF(dbgstr);
		io_dialog_init(control_mask);
	}

	char last_dot = get_dot();
	switch (last_dot) {

		// Returns NO_DOTS but also plays prompt when fresh call or after a certain
		// number of incorrect tries
		case NO_DOTS:
			if (io_dialog_incorrect_tries == -1) {
				play_mp3(mode_fileset, prompt);
				io_dialog_incorrect_tries++;
			}
			return NO_DOTS;
			break;

		// Returns dot if enabled, o/w registers error
		case '1': case '2': case '3': case '4': case '5': case '6':
			if (io_dialog_dots_enabled[CHARTOINT(last_dot) - 1] == true) {
				sprintf(dbgstr, "[IO] Returning dot %c\n\r", last_dot);
				PRINTF(dbgstr);
				io_dialog_reset();
				return last_dot;
			} else {
				io_dialog_error();
				return NO_DOTS;
			}
			break;

		// Returns control button if enabled, o/w registers error
		case ENTER: case CANCEL:
			if (io_dialog_enter_cancel_enabled == true) {
				io_dialog_reset();
				return last_dot;
			} else {
				io_dialog_error();
				return NO_DOTS;
			}
			break;

		// Returns control button if enabled, o/w registers error
		case LEFT: case RIGHT:
			if (io_dialog_left_right_enabled == true) {
				io_dot = last_dot;
				io_dialog_reset();
				return last_dot;
			} else {
				io_dialog_error();
				return NO_DOTS;
			}
			break;

		// Should not get here
		default:
			sprintf(dbgstr, "[IO] Invalid dot: %x\n\r", last_dot);
			PRINTF(dbgstr);
			quit_mode();
			return NO_DOTS;
			break;
	}
}

// ***********************************************
// ********** Basic IO helper funcitons **********
// ***********************************************

/**
* @brief Advances to next cell in io_line
* @param void
* @return void
*/
void io_line_next_cell(void) {
	// Next cell only if not at end of buffer (saves space for EOT)
	if (io_line_cell_index + 2 < MAX_BUF_SIZE) {
		io_line_cell_index++;
	} else {
		play_mp3(lang_fileset, MP3_LAST_CELL);
	}
}

/**
* @brief Advances to previous cell in io_line
* @param void
* @return void
*/
void io_line_prev_cell(void) {
	// Previous cell only if not already on first cell
	if (io_line_cell_index > 0) {
		io_line_cell_index--;
	} else {
		play_mp3(lang_fileset, MP3_FIRST_CELL);
	}
}

/**
* @brief Clears current cell in io_line
* @param void
* @return void
*/
void io_line_clear_cell(void) {
	io_line[io_line_cell_index] = NO_DOTS;
}

/**
* @brief Resets io_line state variables
* @param void
* @return void
*/
void io_line_reset(void) {
	for (int i = 0; i < MAX_BUF_SIZE; i++) {
		io_line[i] = NO_DOTS;
	}
	io_line_cell_index = 0;
	io_line_ready = true;
}

// ******************************************************
// ********** Intermediate IO helper functions **********
// ******************************************************


/**
* @brief Converts raw cell patterns from io_line to glyphs in io_parsed
* @param void
* @return bool - true if all raw cells were valid, false otherwise
*/
bool io_convert_line(void) {
	int line_index = 0;
	int parse_index = 0;
	glyph_t* curr_glyph = NULL;
	
	// Iterate through io_line, find matching glyph for each cell pattern as long
	// as it isn't END_OF_TEXT, and add to io_parsed
	for (line_index = 0, parse_index = 0;
		line_index - 2 < MAX_BUF_SIZE;
		line_index++, parse_index++) {
		
		if (io_line[line_index] == END_OF_TEXT) {
			break;
		}
		
		curr_glyph = get_glyph(lang_script, io_line, &line_index);
		if (curr_glyph == NULL) {
			return false;
		} else {
			sprintf(dbgstr, "[IO] Parsed glyph[%d]: %s\n\r", parse_index, curr_glyph->sound);
			PRINTF(dbgstr);
			io_parsed[parse_index] = curr_glyph;
		}
	}

	// If control returns from loop then matching glyphs were found for all
	// raw cells, add a NULL terminator and return true
	io_parsed[line_index] = NULL;
    sprintf(dbgstr, "[IO] line index[%d]: \n\r", line_index);
    PRINTF(dbgstr);
	return true;
}

/**
* @brief Parses io_line buffer for a number and stores it in io_parsed
* @param void
* @return void
*/
bool io_parse_number(int* res) {
	glyph_t* curr_glyph = NULL;
	int i = 0, curr_digit = 0;

	*res = 0;
    curr_glyph = io_parsed[0];
    if (curr_glyph->pattern != NUMSIGN) {
        play_mp3(lang_fileset, MP3_NUMSIGN_MISSED);
        return false;
    }
    if (io_parsed[1] == NULL || is_blank(io_parsed[1])){
        play_mp3(lang_fileset, MP3_NO_NUMBER);
        return false;
    }
	for (i = 1; io_parsed[i] != NULL && is_blank(io_parsed[i]) == false; i++) {
		curr_glyph = io_parsed[i];
		curr_digit = get_digit(curr_glyph);
		if (curr_digit < 0) {
            play_mp3(lang_fileset, MP3_INVALID_PATTERN);
			return false;
        } else {
        *res *= 10;
        *res += curr_digit;
        }
	}
	io_parsed[i] = NULL;
	return true;
}


// **************************************************
// ********** Advanced IO helper functions **********
// **************************************************

/**
	@brief Initialises dialog state variables
	@param char control_mask - The controls to enable on the dialog
	@return void
*/
void io_dialog_init(char control_mask) {
	char last_dot = get_dot();
	PRINTF(dbgstr);
	for (int i = 0; i < 6; i++) {
		if ((control_mask & (1 << i)) != 0) {
			io_dialog_dots_enabled[i] = true;
		}
	}
	if ((control_mask & LEFT_RIGHT) == LEFT_RIGHT) {
		io_dialog_left_right_enabled = true;
	}
	if ((control_mask & ENTER_CANCEL) == ENTER_CANCEL) {
		io_dialog_enter_cancel_enabled = true;
	}
	if (last_dot == LEFT || last_dot == RIGHT) {
		io_dialog_incorrect_tries = 0;
	}
	io_dialog_initialised = true;
}

/**
	@brief Resets dialog state variables
	@param void
	@return void
*/
void io_dialog_reset(void) {
	for (int i = 0; i < 6; i++) {
		io_dialog_dots_enabled[i] = false;
	}
	io_dialog_enter_cancel_enabled = false;
	io_dialog_left_right_enabled = false;
	io_dialog_incorrect_tries = -1;
	io_dialog_initialised = false;
}

/**
	@brief Resets dialog state variables
	@param void
	@return void
*/
void io_dialog_error(void) {
	io_dialog_incorrect_tries++;
	play_mp3(lang_fileset, MP3_INVALID_PATTERN);
	if (io_dialog_incorrect_tries >= MAX_INCORRECT_TRIES) {
		io_dialog_incorrect_tries = -1;
	}
}
