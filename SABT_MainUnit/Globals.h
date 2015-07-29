/**
 * @file Globals.h
 * @brief global definitions for the MCU
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
#define MAX_NUMBER_OF_MODES 20

unsigned short number_of_modes;
unsigned short ui_modes[MAX_NUMBER_OF_MODES];

bool ui_is_mode_selected;
char ui_current_mode_number;
int ui_current_mode_index;

lang_type ui_current_language;

//indicates whether or not we are currently playing a sound file
bool playing_sound;

volatile unsigned char *g_file_to_play;
unsigned char g_file_name[FILE_NAME_LEN];
//used throughout the dict_process
unsigned char dict_file_name[FILE_NAME_LEN];

volatile char temp_cnt;

volatile bool playing_mp3;

#define MAX_FILENAME_SIZE 13 //8 + 1 + 3 + 1
 
#define CHARTOINT(c)     ((c) - '0')

#endif /* _GLOBALS_H_ */
