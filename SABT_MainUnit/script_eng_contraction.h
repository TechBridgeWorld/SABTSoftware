/**
 *  @file script_eng_contraction.h
 *  @brief Structures for the english contractions
 *  @author Poornima Kaniarasu (pkaniara)
 */

#ifndef _SCRIPT_ENG_CONTRACTION_H_
#define _SCRIPT_ENG_CONTRACTION_H_

#include "glyph.h"
#include "script_common.h"

#define SCRIPT_CONTRACTION_FILESET "CON" // Followed by 0,1, ..., 6 depending on the pattern used
#define SCRIPT_CONTRACTION_LENGTH 36 // 10 contraction symbols + 26 english alphabets
#define SCRIPT_CONTRACTION_PATTERN_LENTGH 7 // 6 glyphs used to form contractions + 0 for single cell contractions

// English Alphabets - bit pattern

#define ENG_A 0b000001
#define ENG_B 0b000011
#define ENG_C 0b001001
#define ENG_D 0b011001
#define ENG_E 0b010001
#define ENG_F 0b001011
#define ENG_G 0b011011
#define ENG_H 0b010011
#define ENG_I 0b001010
#define ENG_J 0b011010
#define ENG_K 0b000101
#define ENG_L 0b000111
#define ENG_M 0b001101
#define ENG_N 0b011101
#define ENG_O 0b010101
#define ENG_P 0b001111
#define ENG_Q 0b011111
#define ENG_R 0b010111
#define ENG_S 0b001110
#define ENG_T 0b011110
#define ENG_U 0b100101
#define ENG_V 0b100111
#define ENG_W 0b111010
#define ENG_X 0b101101
#define ENG_Y 0b111101
#define ENG_Z 0b110101

// English Contraction symbols

#define CON_S1 0b111101
#define CON_S2 0b111111
#define CON_S3 0b111011
#define CON_S4 0b011101
#define CON_S5 0b011111
#define CON_S6 0b100001
#define CON_S7 0b100101
#define CON_S8 0b100111
#define CON_S9 0b100011
#define CON_S10 0b110011

// English Contraction forming patterns

#define CON_P1 0b000010
#define CON_P2 0b000110
#define CON_P3 0b000111
#define CON_P4 0b000101
#define CON_P5 0b000011
#define CON_P6 0b000001


glyph_t contraction_glyphs[SCRIPT_CONTRACTION_LENGTH] = {
	{ENG_A, true, "A", NULL},
	{ENG_B, true, "B", NULL},
	{ENG_C, true, "C", NULL},
	{ENG_D, true, "D", NULL},
	{ENG_E, true, "E", NULL},
	{ENG_F, true, "F", NULL},
	{ENG_G, true, "G", NULL},
	{ENG_H, true, "H", NULL},
	{ENG_I, true, "I", NULL},
	{ENG_J, true, "J", NULL},
	{ENG_K, true, "K", NULL},
	{ENG_L, true, "L", NULL},
	{ENG_M, true, "M", NULL},
	{ENG_N, true, "N", NULL},
	{ENG_O, true, "O", NULL},
	{ENG_P, true, "P", NULL},
	{ENG_Q, true, "Q", NULL},
	{ENG_R, true, "R", NULL},
	{ENG_S, true, "S", NULL},
	{ENG_T, true, "T", NULL},
	{ENG_U, true, "U", NULL},
	{ENG_V, true, "V", NULL},
	{ENG_W, true, "W", NULL},
	{ENG_X, true, "X", NULL},
	{ENG_Y, true, "Y", NULL},
	{ENG_Z, true, "Z", NULL},
	{CON_S1, true, "S1", NULL},
	{CON_S2, true, "S2", NULL},
	{CON_S3, true, "S3", NULL},
	{CON_S4, true, "S4", NULL},
    {CON_S5, true, "S5", NULL},
	{CON_S6, true, "S6", NULL},
	{CON_S7, true, "S7", NULL},
	{CON_S8, true, "S8", NULL},
	{CON_S9, true, "S9", NULL},
	{CON_S10, true, "S10", NULL},
};

glyph_t contraction_pattern[SCRIPT_CONTRACTION_PATTERN_LENTGH] = {
	{0,	false,  "P0", NULL},						
	{CON_P1, false, "P1", NULL},
	{CON_P2, false, "P2", NULL},
	{CON_P3, false, "P3", NULL},
	{CON_P4, false, "P4", NULL},
	{CON_P5, false, "P5", NULL},
	{CON_P6, false, "P6", NULL}
};

// Patterns used to form braille cells  

script_t script_eng_contraction = {
	SCRIPT_CONTRACTION_LENGTH,
	SCRIPT_CONTRACTION_FILESET,
	contraction_glyphs
};

#endif /* _SCRIPT_ENG_CONTRACTION_H_ */

