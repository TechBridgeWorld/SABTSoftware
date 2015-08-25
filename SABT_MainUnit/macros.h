/**
 * @file macros.h
 * @brief The most basic macros and enums
 * used everywhere.
 * @author Marjorie Carlson (marjorie@cmu.edu)
 */

#ifndef _MACROS_H_
#define _MACROS_H_

#define F_CPU			8000000UL
#define MAX_BUF_SIZE    256

typedef struct glyph glyph_t; // forward declaration of glyph_t in datastructures.h.


 // The six dots of the braille tutor. Letters can be created by logical-oring these together;
// e.g., 'b' is {DOT_1 | DOT_2}.
 
#define DOT_1            0b000001
#define DOT_2            0b000010
#define DOT_3            0b000100
#define DOT_4            0b001000
#define DOT_5            0b010000
#define DOT_6            0b100000

// the control keys
#define NO_DOTS         0b00000000
#define ENTER           0b01000001
#define CANCEL          0b01000010
#define LEFT            0b01000100
#define RIGHT           0b01001000
#define END_OF_TEXT     0b11111111

// masks and macros for getting cells & control keys

#define GET_CELL_CONTROL(c)     (c & 0b11000000)
#define GET_CELL_PATTERN(c)     (c & 0b00111111)

// Cell return control masks
#define WITH_ENTER      0b011000000  // extra 0 added so it doesn't convert as negative in x86
#define WITH_LEFT       0b010000000
#define WITH_RIGHT      0b01000000
#define WITH_CANCEL     0b00000000  // Because we don't care about the cell
                                    // pattern in this case - also, I ran
                                    // out of bits for control buttons...

// Dialog control enable masks
#define ENTER_CANCEL     0b10000000
#define LEFT_RIGHT       0b01000000


// The basic enums used to track state, submode, level and language

// Each mode is a state machine that cycles through some subset of these modes.
// One state is executed in each iteration through the SABT's main loop.
typedef enum {
	INITIAL,
	CHOOSE_SUBMODE,
	CHOOSE_LEVEL,
	REQUEST_QUESTION,
	GENERATE_QUESTION,
	PROMPT,
	GET_INPUT,
	PROCESS_ANSWER,
	CHECK_ANSWER,
	REPROMPT,
	GAME_OVER,
	SWITCH_USERS,
} state_t;

// Some modes can operate in various submodes. submode_t enumerates every possible submode.
// Generally, a mode will only have {learn, play} or {add, subtract, multiply}, but these are
// grouped in one type so that we only have to track one submode variable for each mode. 
typedef enum {
	SUBMODE_NULL,
	SUBMODE_LEARN,
	SUBMODE_PLAY,
	SUBMODE_ADD,
	SUBMODE_SUBTRACT,
	SUBMODE_MULTIPLY
} submode_t;

// Some modes have 2-3 difficulty levels.
typedef enum {
	DIFFICULTY_NULL,
	EASY,
	MEDIUM,
	HARD
} difficulty_t;

// Each mode is in one language.
typedef enum {
	LANGUAGE_NULL,
	ENGLISH,
	HINDI,
	KANNADA
} language_t;

typedef unsigned char uint8_t;   // used to store counts and other tiny integers

#endif