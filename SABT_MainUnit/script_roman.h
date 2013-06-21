/**
 *  @file letter_globals.h
 *  @brief Bit patterns for the Latin script
 *  @author Kory Stiger (kstiger)
 */

#ifndef _SCRIPT_ROMAN_H_
#define _SCRIPT_ROMAN_H_

#include "alphabet.h"
#include "script_common.h"

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

#define ROMAN_SCRIPT_LEN 26

static alphabet_t roman_alphabets[] = {
	{A_BITS, "a"},
	{B_BITS, "b"},
	{C_BITS, "c"},
	{D_BITS, "d"},
	{E_BITS, "e"},
	{F_BITS, "f"},
	{G_BITS, "g"},
	{H_BITS, "h"},
	{I_BITS, "i"},
	{J_BITS, "j"},
	{K_BITS, "k"},
	{L_BITS, "l"},
	{M_BITS, "m"},
	{N_BITS, "n"},
	{O_BITS, "o"},
	{P_BITS, "p"},
	{Q_BITS, "q"},
	{R_BITS, "r"},
	{S_BITS, "s"},
	{T_BITS, "t"},
	{U_BITS, "u"},
	{V_BITS, "v"},
	{W_BITS, "w"},
	{X_BITS, "x"},
	{Y_BITS, "y"},
	{Z_BITS, "z"}
};
 
static script_t script_roman = {
	ROMAN_SCRIPT_LEN,
	"",
	roman_alphabets
};

#endif /* _SCRIPT_ROMAN_H_ */

