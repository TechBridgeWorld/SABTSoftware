/**
 *  @file script_english.h
 *  @brief Structures for the English language
 *  @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_ENGLISH_H_
#define _SCRIPT_ENGLISH_H_

#include "glyph.h"
#include "script_common.h"

#define SCRIPT_ENGLISH_FILESET "ENG_"
#define SCRIPT_ENGLISH_LENGTH 26 // 26 Alphabets

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

glyph_t english_glyphs[SCRIPT_ENGLISH_LENGTH] = {
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
	{ENG_Z, true, "Z", NULL}
};
 
script_t script_english = {
	SCRIPT_ENGLISH_LENGTH,
	SCRIPT_ENGLISH_FILESET,
	english_glyphs
};

#endif /* _SCRIPT_ENGLISH_H_ */

