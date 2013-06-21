/**
 *  @file script_hindi.h
 *  @brief Bit patterns representing alphabets of the Hindi script
 *  @author Vivek Nair (viveknai)
 */

#ifndef _SCRIPT_HINDI_H_
#define _SCRIPT_HINDI_H_

#include "alphabet.h"
#include "script_common.h"

#define SCRIPT_HINDI_FILESET "HIN_"
#define SCRIPT_HINDI_LENGTH 46

// Devanagari script bit patterns
// NOTE: Only a subset of all Hindi# alphabets is represented here
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
alphabet_t alphabets_hindi[] = {
	{DEV_A, "A"},
	{DEV_A_, "A_"},
	{DEV_I, "I"},
	{DEV_I_, "I_"},
	{DEV_U, "U"},
	{DEV_U_, "U_"},
	{DEV_E, "E"},
	{DEV_AI, "AI"},
	{DEV_O, "O"},
	{DEV_AU, "AU"},
	{DEV_A_N, "A_N"},
	{DEV_AH_, "AH_"},
	{DEV__AM_, "_AM_"},
	{DEV_KA, "KA"},
	{DEV_KHA, "KHA"},
	{DEV_GA, "GA"},
	{DEV_GHA, "GHA"},
	{DEV__NA, "_NA"},
	{DEV_CA, "CA"},
	{DEV_CHA, "CHA"},
	{DEV_JA, "JA"},
	{DEV_JHA, "JHA"},
	{DEV___NA, "__NA"},
	{DEV_T_A, "T_A"},
	{DEV_T_HA, "T_HA"},
	{DEV_D_A, "D_A"},
	{DEV_D_HA, "D_HA"},
	{DEV_N_A, "N_A"},
	{DEV_TA, "TA"},
	{DEV_THA, "THA"},
	{DEV_DA, "DA"},
	{DEV_DHA, "DHA"},
	{DEV_NA, "NA"},
	{DEV_PA, "PA"},
	{DEV_PHA, "PHA"},
	{DEV_BA, "BA"},
	{DEV_BHA, "BHA"},
	{DEV_MA, "MA"},
	{DEV_YA, "YA"},
	{DEV_RA, "RA"},
	{DEV_LA, "LA"},
	{DEV_VA, "VA"},
	{DEV__SA, "_SA"},
	{DEV_S_A, "S_A"},
	{DEV_SA, "SA"},
	{DEV_HA, "HA"}
};

script_t script_hindi = {
	SCRIPT_HINDI_LENGTH,
	SCRIPT_HINDI_FILESET,
	alphabets_hindi
}; 

#endif /* _SCRIPT_HINDI_H_ */
