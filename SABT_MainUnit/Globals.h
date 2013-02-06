/**
 * @file Globals.h
 * @brief global definitions for the MCU
 * @author Nick LaGrow (nlagrow)
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

//#define NUMBER_OF_MODES 3

volatile unsigned char Number_of_modes;
volatile unsigned char UI_Modes[10];

volatile unsigned char *gFileToPlay;
unsigned char fileName[13];

volatile char tempCnt;
/*
 * MP3 files. 
 * INT.MP3 - Introductory word.
 * MD1.MP3 - Mode 1 introduction MP3
 * MD1.MP3 - Mode 2 introduction MP3
 * ERR1.MP3 - Invalid mode, please press mode button select a valid mode
 *
 * MD1\WC.MP3
 * MD1\1.MP3
 * MD1\2.MP3
 * MD1\3.MP3
 * MD1\4.MP3
 * MD1\5.MP3
 * MD1\6.MP3
 */

// vars for print statements
unsigned char debug_buffer[256];

// debug print statement
#define DPRINTF(msg, ...) \
  sprintf(debug_buffer, msg, ##__VA_ARGS__); \
  USART_transmitStringToPC(&debug_buffer);

#endif /* _GLOBALS_H_ */
