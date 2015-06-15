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
#define KAN_NNA				0b101100

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
#define KAN_RA				0b010111 //changed from 0b001111 as that is PA (6/1/15)
#define KAN_LA				0b000111
#define KAN_VA				0b100111

#define KAN_SHA				0b101001
#define KAN_SHHA			0b101111
#define KAN_SA 				0b001110
#define KAN_HA				0b010011

#define KAN_DLA				0b111000

extern script_t script_kannada;

#endif /* _SCRIPT_KANNADA_H_ */
