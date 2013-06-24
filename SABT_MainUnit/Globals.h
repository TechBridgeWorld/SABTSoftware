/**
 * @file Globals.h
 * @brief global definitions for the MCU
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h> 
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "FAT32.h"
#include "USART_PC.h"
#include "SD_routines.h"
#include "VS1053.h"
#include "SPI.h"
#include "USART_Keypad.h"
#include "UI_Handle.h"
#include "PC_Handle.h"

#define F_CPU 8000000UL
#define MAX_NUMBER_OF_MODES	10
 
unsigned short number_of_modes;
unsigned short ui_modes[MAX_NUMBER_OF_MODES];

char dbgstr[64];

//indicates whether or not we are currently playing a sound file
bool playing_sound;

volatile unsigned char *g_file_to_play;
unsigned char g_file_name[FILE_NAME_LEN];
//used throughout the dict_process
unsigned char dict_file_name[FILE_NAME_LEN];

volatile char temp_cnt;

volatile bool playing_mp3;

#define PRINTF(msg) \
  usart_transmit_string_to_pc((unsigned char*)msg);

#define SENDBYTE(msg) \
  usart_transmit_byte_to_pc((unsigned char)msg);

#define MAX_FILENAME_SIZE 13 //8 + 1 + 3 + 1
  
#define NEWLINE	PRINTF("\n\r");

#define CHARTOINT(c)     ((c) - '0')

#endif /* _GLOBALS_H_ */
