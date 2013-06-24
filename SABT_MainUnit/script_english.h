/**
 *  @file script_english.h
 *  @brief Structures for the English language
 *  @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_ENGLISH_H_
#define _SCRIPT_ENGLISH_H_

#include "alphabet.h"
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

static alphabet_t english_alphabets[] = {
	{A_BITS, "A"},
	{B_BITS, "B"},
	{C_BITS, "C"},
	{D_BITS, "D"},
	{E_BITS, "E"},
	{F_BITS, "F"},
	{G_BITS, "G"},
	{H_BITS, "H"},
	{I_BITS, "I"},
	{J_BITS, "J"},
	{K_BITS, "K"},
	{L_BITS, "L"},
	{M_BITS, "M"},
	{N_BITS, "N"},
	{O_BITS, "O"},
	{P_BITS, "P"},
	{Q_BITS, "Q"},
	{R_BITS, "R"},
	{S_BITS, "S"},
	{T_BITS, "T"},
	{U_BITS, "U"},
	{V_BITS, "V"},
	{W_BITS, "W"},
	{X_BITS, "X"},
	{Y_BITS, "Y"},
	{Z_BITS, "Z"}
};
 
static script_t script_english = {
	SCRIPT_ENGLISH_LENGTH,
	SCRIPT_ENGLISH_FILESET,
	english_alphabets
};

#endif /* _SCRIPT_ENGLISH_H_ */

