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

//#define NUMBER_OF_MODES 3
#define F_CPU 8000000UL

volatile unsigned char number_of_modes;
volatile unsigned char UI_Modes[10];

volatile unsigned char *gFileToPlay;
unsigned char fileName[13];

volatile char tempCnt;

volatile bool playing_mp3;

//TODO remove
void set_last_dot(char dot);
void set_last_dot2(char dot);
void set_last_dot3(char dot);

#define PRINTF(msg) \
  USART_transmitStringToPC((unsigned char*)msg);

#endif /* _GLOBALS_H_ */
