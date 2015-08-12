/**
 * @file globals.h
 * @brief Global variables accessible to any mode
 * @author Marjorie Carlson (marjorie@cmu.edu)
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#ifdef DEBUGMODE
#else
    #include <avr/io.h>
    #include <avr/pgmspace.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>
#endif
 
#include <stdint.h>
#include <stdbool.h> 
#include <stdio.h>

#include "FAT32.h"
#include "USART_PC.h"
#include "SD_routines.h"
#include "VS1053.h"
#include "SPI.h"
#include "USART_Keypad.h"
#include "UI_Handle.h"
#include "PC_Handle.h"
#include "macros.h"
#include "io.h"
#include "datastructures.h"
#include "debug.h"


// These values are stored globally for the sake of efficiency, and are modified and
// used by the modes. The reset function of each mode should call reset_globals and
// reset_statsso that the values are reset to their defaults when each mode is begun.
// @todo Reset stats when a mode is started, not manually in each mode?

language_t ui_current_language;		// an enum designating current language, so correct feedback mp3s are played
char* lang_fileset;					// the prefix string of that language, e.g. "e_". @todo Needn't be global

unsigned char last_dot;			// the last dot pressed
unsigned char cell;				// bitmap representing the entire cell + control button last pressed
unsigned char cell_pattern;		// bitmap representing the braille cell itself -- the last six bits of cell
unsigned char cell_control;		// bitmap representing control button pressed -- the first two bits of the cell

// these keep track of the state, submode and difficulty level selected in the current mode.
state_t 	 current_state;
submode_t 	 submode;
difficulty_t level;

unsigned char score;		// num of right answers so far
unsigned char max_mistakes;	// num mistakes allowed before the correct answer given
unsigned char mistakes;		// total mistakes made so far since the mode began
unsigned char curr_mistakes; // mistakes made in current question




// @todo: diff between these and last_cell, etc.?
extern char io_dot;
extern char io_cell;
extern char io_line[MAX_BUF_SIZE];
extern glyph_t* io_parsed[MAX_BUF_SIZE];
extern bool io_user_abort;


// Used to keep track of all modes and to determine which mode is currently selected
#define MAX_NUMBER_OF_MODES 20
unsigned short number_of_modes;
unsigned short ui_modes[MAX_NUMBER_OF_MODES];

bool ui_is_mode_selected;
char ui_current_mode_number; // this is the current mode; e.g., if MD4 is running, this will be 4
int  ui_current_mode_index;




// USED FOR ACCESSING AUDIO FILES

#define MAX_FILENAME_SIZE	13 		// 8 + 1 + 3 + 1

bool playing_sound; // true if we're currently playing a sound file
volatile unsigned char *g_file_to_play;
unsigned char g_file_name[FILE_NAME_LEN];
//used throughout the dict_process
unsigned char dict_file_name[FILE_NAME_LEN];




#endif /* _GLOBALS_H_ */
