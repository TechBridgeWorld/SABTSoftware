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
#define SCRIPT_ENGLISH_LENGTH 26

#define A_BITS 0b00000001
#define B_BITS 0b00000011
#define C_BITS 0b00001001
#define D_BITS 0b00011001
#define E_BITS 0b00010001
#define F_BITS 0b00001011
#define G_BITS 0b00011011
#define H_BITS 0b00010011
#define I_BITS 0b00001010
#define J_BITS 0b00011010
#define K_BITS 0b00000101
#define L_BITS 0b00000111
#define M_BITS 0b00001101
#define N_BITS 0b00011101
#define O_BITS 0b00010101
#define P_BITS 0b00001111
#define Q_BITS 0b00011111
#define R_BITS 0b00010111
#define S_BITS 0b00001110
#define T_BITS 0b00011110
#define U_BITS 0b00100101
#define V_BITS 0b00100111
#define W_BITS 0b00111010
#define X_BITS 0b00101101
#define Y_BITS 0b00111101
#define Z_BITS 0b00110101

glyph_t english_glyphs[] = {
	{A_BITS, "A", NULL},
	{B_BITS, "B", NULL},
	{C_BITS, "C", NULL},
	{D_BITS, "D", NULL},
	{E_BITS, "E", NULL},
	{F_BITS, "F", NULL},
	{G_BITS, "G", NULL},
	{H_BITS, "H", NULL},
	{I_BITS, "I", NULL},
	{J_BITS, "J", NULL},
	{K_BITS, "K", NULL},
	{L_BITS, "L", NULL},
	{M_BITS, "M", NULL},
	{N_BITS, "N", NULL},
	{O_BITS, "O", NULL},
	{P_BITS, "P", NULL},
	{Q_BITS, "Q", NULL},
	{R_BITS, "R", NULL},
	{S_BITS, "S", NULL},
	{T_BITS, "T", NULL},
	{U_BITS, "U", NULL},
	{V_BITS, "V", NULL},
	{W_BITS, "W", NULL},
	{X_BITS, "X", NULL},
	{Y_BITS, "Y", NULL},
	{Z_BITS, "Z", NULL}
};
 
script_t script_english = {
	SCRIPT_ENGLISH_LENGTH,
	SCRIPT_ENGLISH_FILESET,
	english_glyphs
};

#endif /* _SCRIPT_ENGLISH_H_ */

