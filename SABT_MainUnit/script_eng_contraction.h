/**
 *  @file script_eng_contraction.h
 *  @brief Structures for the english contractions
 *  @author Poornima Kaniarasu (pkaniara)
 */

#ifndef _SCRIPT_ENG_CONTRACTION_H_
#define _SCRIPT_ENG_CONTRACTION_H_

#include "glyph.h"
#include "script_common.h"

#define SCRIPT_CONTRACTION_FILESET "ENG_" // Followed by 0,1, ..., 6 depending on the pattern used
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

#define CON_S1 0b101111
#define CON_S2 0b111111
#define CON_S3 0b110111
#define CON_S4 0b101110
#define CON_S5 0b111110
#define CON_S6 0b100001
#define CON_S7 0b101001
#define CON_S8 0b111001
#define CON_S9 0b110001
#define CON_S10 0b110011

// English Contraction forming patterns

#define CON_P1 0b010000
#define CON_P2 0b011000
#define CON_P3 0b111000
#define CON_P4 0b101000
#define CON_P5 0b110000
#define CON_P6 0b100000


glyph_t contraction_glyphs[SCRIPT_CONTRACTION_LENGTH] = {
	{ENG_A, "A", NULL, NULL},
	{ENG_B, "B", NULL, NULL},
	{ENG_C, "C", NULL, NULL},
	{ENG_D, "D", NULL, NULL},
	{ENG_E, "E", NULL, NULL},
	{ENG_F, "F", NULL, NULL},
	{ENG_G, "G", NULL, NULL},
	{ENG_H, "H", NULL, NULL},
	{ENG_I, "I", NULL, NULL},
	{ENG_J, "J", NULL, NULL},
	{ENG_K, "K", NULL, NULL},
	{ENG_L, "L", NULL, NULL},
	{ENG_M, "M", NULL, NULL},
	{ENG_N, "N", NULL, NULL},
	{ENG_O, "O", NULL, NULL},
	{ENG_P, "P", NULL, NULL},
	{ENG_Q, "Q", NULL, NULL},
	{ENG_R, "R", NULL, NULL},
	{ENG_S, "S", NULL, NULL},
	{ENG_T, "T", NULL, NULL},
	{ENG_U, "U", NULL, NULL},
	{ENG_V, "V", NULL, NULL},
	{ENG_W, "W", NULL, NULL},
	{ENG_X, "X", NULL, NULL},
	{ENG_Y, "Y", NULL, NULL},
	{ENG_Z, "Z", NULL, NULL},
	{CON_S1, "S1", NULL, NULL},
	{CON_S2, "S2", NULL, NULL},
	{CON_S3, "S3", NULL, NULL},
	{CON_S4, "S4", NULL, NULL},
    {CON_S5, "S5", NULL, NULL},
	{CON_S6, "S6", NULL, NULL},
	{CON_S7, "S7", NULL, NULL},
	{CON_S8, "S8", NULL, NULL},
	{CON_S9, "S9", NULL, NULL},
	{CON_S10, "S10", NULL, NULL},
};

glyph_t contraction_pattern[SCRIPT_CONTRACTION_PATTERN_LENTGH] = {
	{0, "P0", NULL, NULL},						
	{CON_P1, "P1", NULL, NULL},
	{CON_P2, "P2", NULL, NULL},
	{CON_P3, "P3", NULL, NULL},
	{CON_P4, "P4", NULL, NULL},
	{CON_P5, "P5", NULL, NULL},
	{CON_P6, "P6", NULL, NULL}
};

// Patterns used to form braille cells  

script_t script_eng_contraction = {
	SCRIPT_CONTRACTION_LENGTH,
	0,
	SCRIPT_CONTRACTION_FILESET,
	contraction_glyphs
};

#endif /* _SCRIPT_ENG_CONTRACTION_H_ */

