/**
 *  @file script_kannada.h
 *  @brief Bit patterns representing glyphs of the Hindi script
 *  @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_KANNADA_H_
#define _SCRIPT_KANNADA_H_

#include "glyph.h"
#include "script_common.h"

#define SCRIPT_KANNADA_FILESET "KAN_"
#define SCRIPT_KANNADA_LENGTH 50

// Devanagari script bit patterns
// NOTE: Only a subset of all Kannada glyphs is represented here
#define KAN_A 				0b000001
#define KAN_AA				0b011100
#define KAN_I				0b001010
#define KAN_II				0b010100
#define KAN_U				0b100101
#define KAN_UU				0b110011
#define KAN_RU_1 			0b010000	
#define KAN_RU_2			0b010111

#define KAN_E				0b100010
#define KAN_EE				0b010001
#define KAN_AI				0b001100
#define KAN_O				0b101101
#define KAN_OO				0b010101
#define KAN_AU				0b101010
#define KAN_AM				0b110000
#define KAN_AHA				0b100000

#define KAN_KA				0b000101
#define KAN_KHA				0b101000
#define KAN_GA				0b011011
#define KAN_GHA				0b100011
#define	KAN_NYA				0b101100

#define KAN_CHA				0b001001
#define KAN_CHHA			0b100001
#define KAN_JA				0b011010
#define KAN_JHA				0b110100
#define	KAN_NYAA			0b010010

#define KAN_TTA				0b111110
#define KAN_TTHA			0b111010
#define KAN_DDA				0b101011
#define KAN_DDHA			0b111111
#define KAN_GNA				0b111100

#define KAN_TA				0b011110
#define KAN_THA				0b111001
#define KAN_DA 				0b011001
#define KAN_DHA				0b101110
#define KAN_NA  			0b011101
 
#define KAN_PA				0b001111
#define KAN_PHA				0b010110
#define	KAN_BA				0b000011
#define KAN_BHA				0b011000
#define KAN_MA				0b001101

#define KAN_YA				0b111101
#define KAN_RA				0b001111
#define KAN_LA				0b000111
#define KAN_VA				0b100111

#define KAN_SHA				0b101001
#define KAN_SHHA			0b101111
#define KAN_SA 				0b001110
#define KAN_HA				0b010011

#define KAN_DLA				0b111000
#define KAN_KSHA			0b011111

glyph_t glyphs_kannada[SCRIPT_KANNADA_LENGTH];

//Multi-cell - RU

script_t subscript_kan_ru_1;
glyph_t glyphs_kan_ru_1[1];

script_t subscript_kan_ru_1 = {1, 0, "KAN_", glyphs_kan_ru_1};
glyph_t glyphs_kan_ru_1[1] = {{KAN_RU_2, "RU", &(glyphs_kannada[6]), NULL}};

// Kannada script array
glyph_t glyphs_kannada[SCRIPT_KANNADA_LENGTH] = {
	{KAN_A, "A", NULL, NULL},
	{KAN_AA, "AA", NULL, NULL},
	{KAN_I, "I", NULL, NULL},
	{KAN_II, "II", NULL, NULL},
	{KAN_U, "U", NULL, NULL},
	{KAN_UU, "UU", NULL, NULL},
	{KAN_RU_1, MP3_INVALID_PATTERN, NULL, &subscript_kan_ru_1},

	{KAN_E, "E", NULL, NULL},
	{KAN_EE, "EE", NULL, NULL},
	{KAN_AI, "AI", NULL, NULL},
	{KAN_O, "O", NULL, NULL},
	{KAN_OO, "OO", NULL, NULL},
	{KAN_AU, "AU", NULL, NULL},
	{KAN_AM, "AM", NULL, NULL},
	{KAN_AHA, "AHA", NULL, NULL},

	{KAN_KA, "KA", NULL, NULL},
	{KAN_KHA, "KHA", NULL, NULL},
	{KAN_GA, "GA", NULL, NULL},
	{KAN_GHA, "GHA", NULL, NULL},
	{KAN_NYA, "NYA", NULL, NULL},

	{KAN_CHA, "CHA", NULL, NULL},
	{KAN_CHHA, "CHHA", NULL, NULL},
	{KAN_JA, "JA", NULL, NULL},
	{KAN_JHA, "JHA", NULL, NULL},
	{KAN_NYAA, "NYAA", NULL, NULL},

	{KAN_TTA, "TTA", NULL, NULL},
	{KAN_TTHA, "TTHA", NULL, NULL},
	{KAN_DDA, "DDA", NULL, NULL},
	{KAN_DDHA, "DDHA", NULL, NULL},
	{KAN_GNA, "GNA", NULL, NULL},

	{KAN_TA, "TA", NULL, NULL},
	{KAN_THA, "THA", NULL, NULL},
	{KAN_DA, "DA", NULL, NULL},
	{KAN_DHA, "DHA", NULL, NULL},
	{KAN_NA, "NA", NULL, NULL},

	{KAN_PA, "PA", NULL, NULL},
	{KAN_PHA, "PHA", NULL, NULL},
	{KAN_BA, "BA", NULL, NULL},
	{KAN_BHA, "BHA", NULL, NULL},
	{KAN_MA, "MA", NULL, NULL},

	{KAN_YA, "YA", NULL, NULL},
	{KAN_RA, "RA", NULL, NULL},
	{KAN_LA, "LA", NULL, NULL},
	{KAN_VA, "VA", NULL, NULL},

	{KAN_SHA, "SHA", NULL, NULL},
	{KAN_SHHA, "SHHA", NULL, NULL},
	{KAN_SA, "SA", NULL, NULL},
	{KAN_HA, "HA", NULL, NULL},

	{KAN_DLA, "DLA", NULL, NULL},
	{KAN_KSHA, "KSHA", NULL, NULL}
};

script_t script_kannada = {
	SCRIPT_KANNADA_LENGTH,
	0,
	SCRIPT_KANNADA_FILESET,
	glyphs_kannada,
}; 

#endif /* _SCRIPT_KANNADA_H_ */
