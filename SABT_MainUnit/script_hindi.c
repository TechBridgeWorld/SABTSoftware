#include "script_hindi.h"

// Hindi script array
glyph_t glyphs_hindi[SCRIPT_HINDI_LENGTH] = {
	{HIN_A, "A", NULL, NULL},		// 0
	{HIN_AA, "AA", NULL, NULL},		// 1
	{HIN_I, "I", NULL, NULL},		// 2
	{HIN_II, "II", NULL, NULL},		// 3
	{HIN_U, "U", NULL, NULL},		// 4
	{HIN_UU, "UU", NULL, NULL},		// 5
	{HIN_RU_1, "RU", NULL, &glyphs_hindi[7]},  // 6
	{HIN_RU_2, "RU_2", &glyphs_hindi[6] ,NULL}, // 7

	{HIN_EE, "EE", NULL, NULL},		// 8
	{HIN_AI, "AI", NULL, NULL},		// 9
	{HIN_OO, "OO", NULL, NULL},		// 10
	{HIN_AU, "AU", NULL, NULL},		// 11
	{HIN_AM, "AM", NULL, NULL},		// 12
	{HIN_AHA, "AHA", NULL, NULL},	// 13

	{HIN_KA, "KA", NULL, NULL},		// 14
	{HIN_KHA, "KHA", NULL, NULL},	// 15
	{HIN_GA, "GA", NULL, NULL},		// 16
	{HIN_GHA, "GHA", NULL, NULL},	// 17
	{HIN_NNA, "NNA", NULL, NULL},	// 18

	{HIN_CHA, "CHA", NULL, NULL},	// 19
	{HIN_CHHA, "CHHA", NULL, NULL},	// 20
	{HIN_JA, "JA", NULL, NULL},		// 21
	{HIN_JHA, "JHA", NULL, NULL},	// 22
	{HIN_NYAA, "NYAA", NULL, NULL},	// 23

	{HIN_TTA, "TTA", NULL, NULL},	// 24
	{HIN_TTHA, "TTHA", NULL, NULL},	// 25
	{HIN_DDA, "DDA", NULL, NULL},	// 26
	{HIN_DDHA, "DDHA", NULL, NULL},	// 27
	{HIN_GNA, "GNA", NULL, NULL},	// 28

	{HIN_TA, "TA", NULL, NULL},		// 29
	{HIN_THA, "THA", NULL, NULL},	// 30
	{HIN_DA, "DA", NULL, NULL},		// 31
	{HIN_DHA, "DHA", NULL, NULL},	// 32
	{HIN_NA, "NA", NULL, NULL},		// 33

	{HIN_PA, "PA", NULL, NULL},		// 34
	{HIN_PHA, "PHA", NULL, NULL},	// 35
	{HIN_BA, "BA", NULL, NULL},		// 36
	{HIN_BHA, "BHA", NULL, NULL},	// 37
	{HIN_MA, "MA", NULL, NULL},		// 38

	{HIN_YA, "YA", NULL, NULL},		// 39
	{HIN_RA, "RA", NULL, NULL},		// 40
	{HIN_LA, "LA", NULL, NULL},		// 41
	{HIN_VA, "VA", NULL, NULL},		// 42

	{HIN_SHA, "SHA", NULL, NULL},	// 43
	{HIN_SHHA, "SHHA", NULL, NULL},	// 44
	{HIN_SA, "SA", NULL, NULL},		// 45
	{HIN_HA, "HA", NULL, NULL},		// 46

	{HIN_KSHA, "KSHA", NULL, NULL},								// 47
	{HIN_TRA_1, "TRA", NULL, &glyphs_hindi[49]},				// 48
	{HIN_TRA_2, "TRA_2", &glyphs_hindi[48], &glyphs_hindi[50]},	// 49
	{HIN_TRA_3, "TRA_3", &glyphs_hindi[49], NULL},				// 50
	{HIN_JNA, "JNA", NULL, NULL},								// 51
	{HIN_SHRA_1, "SHRA", NULL, &glyphs_hindi[53]},				// 52
	{HIN_SHRA_2, "SHRA2", &glyphs_hindi[52], &glyphs_hindi[54]},// 53
	{HIN_SHRA_3, "SHRA3", &glyphs_hindi[53], NULL}				// 54
};

int letters_hindi[SCRIPT_HINDI_NUM_LETTERS] = {
	0,  1,  2,  3,  4,  5,  6,       8,  9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48,
	    51, 52 };

script_t script_hindi = {
	SCRIPT_HINDI_LENGTH,
	SCRIPT_HINDI_NUM_LETTERS,
	0,
	SCRIPT_HINDI_FILESET,
	glyphs_hindi,
	letters_hindi
};