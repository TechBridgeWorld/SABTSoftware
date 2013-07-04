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
#define DEV_A 				0b00000001
#define DEV_A_				0b00011100
#define DEV_I					0b00001010
#define DEV_I_				0b00010100
#define DEV_U					0b00100101
#define DEV_U_				0b00110011
#define DEV_E					0b00010001
#define DEV_AI				0b00001100
#define DEV_O					0b00010101
#define DEV_AU				0b00101010
#define DEV_A_N				0b00110000
#define DEV_AH_				0b00100000
#define DEV__AM_			0b00000100
#define DEV_KA				0b00000101
#define DEV_KHA				0b00101000
#define DEV_GA				0b00011011
#define DEV_GHA				0b00100011
#define	DEV__NA				0b00101100
#define DEV_CA				0b00001001
#define DEV_CHA				0b00100001
#define DEV_JA				0b00011010
#define DEV_JHA				0b00110100
#define	DEV___NA			0b00010010
#define DEV_T_A				0b00011110
#define DEV_T_HA			0b00111010
#define DEV_D_A				0b00101011
#define DEV_D_HA			0b00111111
#define DEV_N_A			  0b00111100
#define DEV_TA				0b00011110
#define DEV_THA				0b00111001
#define DEV_DA 				0b00011001
#define DEV_DHA				0b00101110
#define DEV_NA  			0b00011101
#define DEV_PA				0b00001111
#define DEV_PHA				0b00010110
#define	DEV_BA				0b00000011
#define DEV_BHA				0b00011000
#define DEV_MA				0b00001101
#define DEV_YA				0b00111101
#define DEV_RA				0b00010111
#define DEV_LA				0b00000111
#define DEV_VA				0b00100111
#define DEV__SA				0b00101001
#define DEV_S_A				0b00101111
#define DEV_SA 				0b00001110
#define DEV_HA				0b00010011

// Devanagari script array
glyph_t glyphs_hindi[] = {
	{DEV_A, "A", NULL},
	{DEV_A_, "A_", NULL},
	{DEV_I, "I", NULL},
	{DEV_I_, "I_", NULL},
	{DEV_U, "U", NULL},
	{DEV_U_, "U_", NULL},
	{DEV_E, "E", NULL},
	{DEV_AI, "AI", NULL},
	{DEV_O, "O", NULL},
	{DEV_AU, "AU", NULL},
	{DEV_A_N, "A_N", NULL},
	{DEV_AH_, "AH_", NULL},
	{DEV__AM_, "_AM_", NULL},
	{DEV_KA, "KA", NULL},
	{DEV_KHA, "KHA", NULL},
	{DEV_GA, "GA", NULL},
	{DEV_GHA, "GHA", NULL},
	{DEV__NA, "_NA", NULL},
	{DEV_CA, "CA", NULL},
	{DEV_CHA, "CHA", NULL},
	{DEV_JA, "JA", NULL},
	{DEV_JHA, "JHA", NULL},
	{DEV___NA, "__NA", NULL},
	{DEV_T_A, "T_A", NULL},
	{DEV_T_HA, "T_HA", NULL},
	{DEV_D_A, "D_A", NULL},
	{DEV_D_HA, "D_HA", NULL},
	{DEV_N_A, "N_A", NULL},
	{DEV_TA, "TA", NULL},
	{DEV_THA, "THA", NULL},
	{DEV_DA, "DA", NULL},
	{DEV_DHA, "DHA", NULL},
	{DEV_NA, "NA", NULL},
	{DEV_PA, "PA", NULL},
	{DEV_PHA, "PHA", NULL},
	{DEV_BA, "BA", NULL},
	{DEV_BHA, "BHA", NULL},
	{DEV_MA, "MA", NULL},
	{DEV_YA, "YA", NULL},
	{DEV_RA, "RA", NULL},
	{DEV_LA, "LA", NULL},
	{DEV_VA, "VA", NULL},
	{DEV__SA, "_SA", NULL},
	{DEV_S_A, "S_A", NULL},
	{DEV_SA, "SA", NULL},
	{DEV_HA, "HA", NULL}
};

script_t script_hindi = {
	SCRIPT_HINDI_LENGTH,
	SCRIPT_HINDI_FILESET,
	glyphs_hindi
}; 

#endif /* _SCRIPT_HINDI_H_ */
