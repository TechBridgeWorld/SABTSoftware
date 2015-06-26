/**
 * @file io.c
 * @brief Declarations for common IO functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _IO_H_
#define _IO_H_

#include "glyph.h"

// General constants
#define MAX_INCORRECT_TRIES 3
#define MAX_BUF_SIZE 				256
#define GET_CELL_CONTROL(c) (c & 0b11000000)
#define GET_CELL_PATTERN(c) (c & 0b00111111)

// Button and control flow mappings
#define NO_DOTS 						0b00000000
#define ENTER								0b01000001
#define CANCEL 							0b01000010
#define LEFT								0b01000100
#define RIGHT								0b01001000
#define END_OF_TEXT					0b11111111

// Cell return control masks
#define WITH_ENTER					0b11000000
#define WITH_LEFT						0b10000000
#define WITH_RIGHT					0b01000000
#define WITH_CANCEL 				0b00000000	// Becuase we don't care about the cell
 																				// pattern in this case - also, I ran
 																				// out of bits for control buttons...

// Dialog control enable masks
#define ENTER_CANCEL				0b10000000
#define LEFT_RIGHT					0b01000000
#define DOT_1								0b00000001
#define DOT_2								0b00000010
#define DOT_3								0b00000100
#define DOT_4								0b00001000
#define DOT_5								0b00010000
#define DOT_6								0b00100000

// UI public variables
extern char io_dot;
extern char io_cell;
extern char io_line[MAX_BUF_SIZE];
extern glyph_t* io_parsed[MAX_BUF_SIZE];
extern bool io_user_abort;

// Basic IO functions
void io_init(void);
char get_dot(void);
char get_cell(void);
bool get_line(void);

// Intermediate IO functions
bool get_number(bool* valid, int* res);
bool get_character(bool* valid, char* character);
bool get_first_glyph(glyph_t** res);
bool parse_letter(void);
bool parse_digit(void);
bool parse_symbol(void);
bool parse_word(void);
bool parse_string(void);

// Advanced IO functions
char create_dialog(char* prompt, char control_mask);

#endif /* _IO_H_ */
