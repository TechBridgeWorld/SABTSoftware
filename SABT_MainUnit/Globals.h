/**
 * @file globals.h
 * @brief Global definitions used by the modes and/or main loop
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
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
#include "debug.h"
#include "io.h"
#include "datastructures.h"

#define F_CPU 8000000UL


 // The six dots of the braille tutor. Letters can be created by logical-oring these together;
// e.g., 'b' is {DOT_1 | DOT_2}.
 
#define DOT_1            0b000001
#define DOT_2            0b000010
#define DOT_3            0b000100
#define DOT_4            0b001000
#define DOT_5            0b010000
#define DOT_6            0b100000



 // USED FOR MODE SELECTION

#define MAX_NUMBER_OF_MODES 20
unsigned short number_of_modes;
unsigned short ui_modes[MAX_NUMBER_OF_MODES];

bool ui_is_mode_selected;
char ui_current_mode_number;
int  ui_current_mode_index;




// USED FOR ACCESSING AUDIO FILES

#define MAX_FILENAME_SIZE	13 //8 + 1 + 3 + 1
#define MAX_WORD_LEN		20

bool playing_sound; // true if we're currently playing a sound file
volatile unsigned char *g_file_to_play;
unsigned char g_file_name[FILE_NAME_LEN];
//used throughout the dict_process
unsigned char dict_file_name[FILE_NAME_LEN];





// USED IN MODES

lang_type ui_current_language;
char* lang_fileset;

unsigned char last_dot;
unsigned char cell;
unsigned char cell_pattern;
unsigned char cell_control;

unsigned char next_state;
unsigned char submode;
unsigned char level;

unsigned char max_mistakes;
unsigned char score;
unsigned char mistakes;
unsigned char curr_mistakes;


#define STATE_INITIAL			0
#define STATE_CHOOSE_LEVEL		1
#define STATE_REQUEST_QUESTION	2
#define STATE_GENERATE_QUESTION	3
#define STATE_PROMPT			4
#define STATE_GET_INPUT			5
#define STATE_PROCESS_ANSWER	6
#define	STATE_CHECK_ANSWER		7
#define STATE_REPROMPT			8
#define STATE_EVALUATE_GAME		9
#define STATE_SWITCH_USERS		10

// USED IN MODES
#define SUBMODE_LEARN	1
#define SUBMODE_PLAY	2

#define SUBMODE_ADD		1
#define SUBMODE_SUB		2
#define SUBMODE_MUL		3

#define	LEVEL_EASY		1
#define LEVEL_MEDIUM	2
#define LEVEL_HARD		3



// NOT USED?

//volatile char temp_cnt;
//volatile bool playing_mp3;

 

#endif /* _GLOBALS_H_ */
