/**
 *  @file script_hindi.h
 *  @brief Bit patterns representing glyphs of the Hindi script
 *  @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_HINDI_H_
#define _SCRIPT_HINDI_H_

#include "glyph.h"
#include "script_common.h"

#define SCRIPT_HINDI_FILESET "HIN_"
#define SCRIPT_HINDI_LENGTH 46

// Devanagari script bit patterns
// NOTE: Only a subset of all Hindi# glyphs is represented here
#define DEV_A 				0b000001
#define DEV_A_				0b011100
#define DEV_I					0b001010
#define DEV_I_				0b010100
#define DEV_U					0b100101
#define DEV_U_				0b110011
#define DEV_E					0b010001
#define DEV_AI				0b001100
#define DEV_O					0b010101
#define DEV_AU				0b101010
#define DEV_A_N				0b110000
#define DEV_AH_				0b100000
#define DEV__AM_			0b000100
#define DEV_KA				0b000101
#define DEV_KHA				0b101000
#define DEV_GA				0b011011
#define DEV_GHA				0b100011
#define	DEV__NA				0b101100
#define DEV_CA				0b001001
#define DEV_CHA				0b100001
#define DEV_JA				0b011010
#define DEV_JHA				0b110100
#define	DEV___NA			0b010010
#define DEV_T_A				0b011110
#define DEV_T_HA			0b111010
#define DEV_D_A				0b101011
#define DEV_D_HA			0b111111
#define DEV_N_A			  0b111100
#define DEV_TA				0b011110
#define DEV_THA				0b111001
#define DEV_DA 				0b011001
#define DEV_DHA				0b101110
#define DEV_NA  			0b011101
#define DEV_PA				0b001111
#define DEV_PHA				0b010110
#define	DEV_BA				0b000011
#define DEV_BHA				0b011000
#define DEV_MA				0b001101
#define DEV_YA				0b111101
#define DEV_RA				0b010111
#define DEV_LA				0b000111
#define DEV_VA				0b100111
#define DEV__SA				0b101001
#define DEV_S_A				0b101111
#define DEV_SA 				0b001110
#define DEV_HA				0b010011

// Devanagari script array
glyph_t glyphs_hindi[SCRIPT_HINDI_LENGTH] = {
	{DEV_A, true, "A", NULL},
	{DEV_A_, true, "A_", NULL},
	{DEV_I, true, "I", NULL},
	{DEV_I_, true, "I_", NULL},
	{DEV_U, true, "U", NULL},
	{DEV_U_, true, "U_", NULL},
	{DEV_E, true, "E", NULL},
	{DEV_AI, true, "AI", NULL},
	{DEV_O, true, "O", NULL},
	{DEV_AU, true, "AU", NULL},
	{DEV_A_N, true, "A_N", NULL},
	{DEV_AH_, true, "AH_", NULL},
	{DEV__AM_, true, "_AM_", NULL},
	{DEV_KA, true, "KA", NULL},
	{DEV_KHA, true, "KHA", NULL},
	{DEV_GA, true, "GA", NULL},
	{DEV_GHA, true, "GHA", NULL},
	{DEV__NA, true, "_NA", NULL},
	{DEV_CA, true, "CA", NULL},
	{DEV_CHA, true, "CHA", NULL},
	{DEV_JA, true, "JA", NULL},
	{DEV_JHA, true, "JHA", NULL},
	{DEV___NA, true, "__NA", NULL},
	{DEV_T_A, true, "T_A", NULL},
	{DEV_T_HA, true, "T_HA", NULL},
	{DEV_D_A, true, "D_A", NULL},
	{DEV_D_HA, true, "D_HA", NULL},
	{DEV_N_A, true, "N_A", NULL},
	{DEV_TA, true, "TA", NULL},
	{DEV_THA, true, "THA", NULL},
	{DEV_DA, true, "DA", NULL},
	{DEV_DHA, true, "DHA", NULL},
	{DEV_NA, true, "NA", NULL},
	{DEV_PA, true, "PA", NULL},
	{DEV_PHA, true, "PHA", NULL},
	{DEV_BA, true, "BA", NULL},
	{DEV_BHA, true, "BHA", NULL},
	{DEV_MA, true, "MA", NULL},
	{DEV_YA, true, "YA", NULL},
	{DEV_RA, true, "RA", NULL},
	{DEV_LA, true, "LA", NULL},
	{DEV_VA, true, "VA", NULL},
	{DEV__SA, true, "_SA", NULL},
	{DEV_S_A, true, "S_A", NULL},
	{DEV_SA, true, "SA", NULL},
	{DEV_HA, true, "HA", NULL}
};

script_t script_hindi = {
	SCRIPT_HINDI_LENGTH,
	SCRIPT_HINDI_FILESET,
	glyphs_hindi
}; 

#endif /* _SCRIPT_HINDI_H_ */
