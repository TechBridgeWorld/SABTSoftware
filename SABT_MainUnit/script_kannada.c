#include "script_kannada.h"

// Kannada script array
glyph_t glyphs_kannada[SCRIPT_KANNADA_LENGTH] = {
	{KAN_A, "A", NULL, NULL},
	{KAN_AA, "AA", NULL, NULL},
	{KAN_I, "I", NULL, NULL},
	{KAN_II, "II", NULL, NULL},
	{KAN_U, "U", NULL, NULL},
	{KAN_UU, "UU", NULL, NULL},
	{KAN_RU_1, "RU", NULL, &glyphs_kannada[7]},
	{KAN_RU_2, "RU_2", &glyphs_kannada[6] ,NULL},

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
	{KAN_NNA, "NNA", NULL, NULL},

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
};

script_t script_kannada = {
	SCRIPT_KANNADA_LENGTH,
	0,
	SCRIPT_KANNADA_FILESET,
	glyphs_kannada,
};