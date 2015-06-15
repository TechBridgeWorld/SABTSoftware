#include "script_hindi.h"

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
	{HIN_SHRA_2, "SHRA2", &glyphs_hindi[61], &glyphs_hindi[63]},
	{HIN_SHRA_3, "SHRA3", &glyphs_hindi[62], NULL}
};

script_t script_hindi = {
	SCRIPT_HINDI_LENGTH,
	0,
	SCRIPT_HINDI_FILESET,
	glyphs_hindi,
};