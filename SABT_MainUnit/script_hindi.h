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
#define SCRIPT_HINDI_LENGTH 55

// Devanagari script bit patterns
// NOTE: Only a subset of all Hindi glyphs is represented here
#define HIN_A 				0b000001
#define HIN_AA				0b011100
#define HIN_I				  0b001010
#define HIN_II				0b010100
#define HIN_U				  0b100101
#define HIN_UU				0b110011
#define HIN_RU_1  		0b010000	
#define HIN_RU_2			0b010111

#define HIN_EE				0b010001
#define HIN_AI				0b001100
#define HIN_OO				0b010101
#define HIN_AU				0b101010
#define HIN_AM				0b110000
#define HIN_AHA				0b100000

#define HIN_KA				0b000101
#define HIN_KHA				0b101000
#define HIN_GA				0b011011
#define HIN_GHA				0b100011
#define HIN_NNA				0b101100

#define HIN_CHA				0b001001
#define HIN_CHHA			0b100001
#define HIN_JA				0b011010
#define HIN_JHA				0b110100
#define	HIN_NYAA			0b010010

#define HIN_TTA				0b111110
#define HIN_TTHA			0b111010
#define HIN_DDA				0b101011
#define HIN_DDHA			0b111111
#define HIN_GNA				0b111100

#define HIN_TA				0b011110
#define HIN_THA				0b111001
#define HIN_DA 				0b011001
#define HIN_DHA				0b101110
#define HIN_NA  			0b011101
 
#define HIN_PA				0b001111
#define HIN_PHA				0b010110
#define	HIN_BA				0b000011
#define HIN_BHA				0b011000
#define HIN_MA				0b001101

#define HIN_YA				0b111101
#define HIN_RA				0b010111 //changed from 0b001111 as that is PA (6/1/15)
#define HIN_LA				0b000111
#define HIN_VA				0b100111

#define HIN_SHA				0b101001
#define HIN_SHHA			0b101111
#define HIN_SA 				0b001110
#define HIN_HA				0b010011

#define HIN_KSHA			0b011111
#define HIN_TRA_1			0b010000
#define HIN_TRA_2			0b011110
#define HIN_TRA_3			0b010111
#define HIN_JNA				0b110001
#define HIN_SHRA_1		0b001000
#define HIN_SHRA_2		0b101001
#define HIN_SHRA_3		0b010111


glyph_t glyphs_hindi[SCRIPT_HINDI_LENGTH];

//Multi-cell - RU

// Hindi script array
glyph_t glyphs_hindi[SCRIPT_HINDI_LENGTH] = {
	{HIN_A, "A", NULL, NULL},
	{HIN_AA, "AA", NULL, NULL},
	{HIN_I, "I", NULL, NULL},
	{HIN_II, "II", NULL, NULL},
	{HIN_U, "U", NULL, NULL},
	{HIN_UU, "UU", NULL, NULL},
	{HIN_RU_1, "RU", NULL, &glyphs_hindi[7]},
	{HIN_RU_2, "RU_2", &glyphs_hindi[6] ,NULL},

	{HIN_EE, "EE", NULL, NULL},
	{HIN_AI, "AI", NULL, NULL},
	{HIN_OO, "OO", NULL, NULL},
	{HIN_AU, "AU", NULL, NULL},
	{HIN_AM, "AM", NULL, NULL},
	{HIN_AHA, "AHA", NULL, NULL},

	{HIN_KA, "KA", NULL, NULL},
	{HIN_KHA, "KHA", NULL, NULL},
	{HIN_GA, "GA", NULL, NULL},
	{HIN_GHA, "GHA", NULL, NULL},
	{HIN_NNA, "NNA", NULL, NULL},

	{HIN_CHA, "CHA", NULL, NULL},
	{HIN_CHHA, "CHHA", NULL, NULL},
	{HIN_JA, "JA", NULL, NULL},
	{HIN_JHA, "JHA", NULL, NULL},
	{HIN_NYAA, "NYAA", NULL, NULL},

	{HIN_TTA, "TTA", NULL, NULL},
	{HIN_TTHA, "TTHA", NULL, NULL},
	{HIN_DDA, "DDA", NULL, NULL},
	{HIN_DDHA, "DDHA", NULL, NULL},
	{HIN_GNA, "GNA", NULL, NULL},

	{HIN_TA, "TA", NULL, NULL},
	{HIN_THA, "THA", NULL, NULL},
	{HIN_DA, "DA", NULL, NULL},
	{HIN_DHA, "DHA", NULL, NULL},
	{HIN_NA, "NA", NULL, NULL},

	{HIN_PA, "PA", NULL, NULL},
	{HIN_PHA, "PHA", NULL, NULL},
	{HIN_BA, "BA", NULL, NULL},
	{HIN_BHA, "BHA", NULL, NULL},
	{HIN_MA, "MA", NULL, NULL},

	{HIN_YA, "YA", NULL, NULL},
	{HIN_RA, "RA", NULL, NULL},
	{HIN_LA, "LA", NULL, NULL},
	{HIN_VA, "VA", NULL, NULL},

	{HIN_SHA, "SHA", NULL, NULL},
	{HIN_SHHA, "SHHA", NULL, NULL},
	{HIN_SA, "SA", NULL, NULL},
	{HIN_HA, "HA", NULL, NULL},

	{HIN_KSHA, "KSHA", NULL, NULL},
	{HIN_TRA_1, "TRA", NULL, &glyphs_hindi[58]},
	{HIN_TRA_2, "TRA_2", &glyphs_hindi[57], &glyphs_hindi[59]},
	{HIN_TRA_3, "TRA_3", &glyphs_hindi[58], NULL},
	{HIN_JNA, "JNA", NULL, NULL},
	{HIN_SHRA_1, "SHRA", NULL, &glyphs_hindi[62]},
	{HIN_SHRA_2, "SHRA_2", &glyphs_hindi[61], &glyphs_hindi[63]},
	{HIN_SHRA_3, "SHRA_3", &glyphs_hindi[62], NULL}
};

script_t script_hindi = {
	SCRIPT_HINDI_LENGTH,
	0,
	SCRIPT_HINDI_FILESET,
	glyphs_hindi,
}; 

#endif /* _SCRIPT_HINDI_H_ */
