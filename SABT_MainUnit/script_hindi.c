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
	{HIN_RU_2, "NCEL", &glyphs_hindi[6] ,NULL}, // 7

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
	{HIN_TRA_2, "NCEL", &glyphs_hindi[48], &glyphs_hindi[50]},	// 49
	{HIN_TRA_3, "NCEL", &glyphs_hindi[49], NULL},				// 50
	{HIN_JNA, "JNA", NULL, NULL},								// 51
	{HIN_SHRA_1, "SHRA", NULL, &glyphs_hindi[53]},				// 52
	{HIN_SHRA_2, "NCEL", &glyphs_hindi[52], &glyphs_hindi[54]},// 53
	{HIN_SHRA_3, "NCEL", &glyphs_hindi[53], NULL}				// 54
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

// new stuff

// used in multicell letters
#define HALANT 		DOT_4
#define ABBREV 		DOT_5
#define RA 			DOT_1 | DOT_2 | DOT_3 | DOT_5
#define TA 			DOT_2 | DOT_3 | DOT_4 | DOT_5
#define SHA 		DOT_1 | DOT_4 | DOT_6

// one-cell letters
cell_t hin_a_cell      	= DOT_1;
cell_t aa_cell     	= DOT_3 | DOT_4 | DOT_5;
cell_t hin_i_cell	   	= DOT_2 | DOT_4;
cell_t ii_cell	   	= DOT_3 | DOT_5;
cell_t hin_u_cell      	= DOT_1 | DOT_3 | DOT_6;
cell_t uu_cell     	= DOT_1 | DOT_2 | DOT_5 | DOT_6;

cell_t ee_cell	   	= DOT_1 | DOT_5;
cell_t ai_cell		= DOT_3 | DOT_4;
cell_t oo_cell 		= DOT_1 | DOT_3 | DOT_5;
cell_t au_cell 		= DOT_2 | DOT_4 | DOT_6;
cell_t am_cell		= DOT_5 | DOT_6;
cell_t aha_cell		= DOT_6;

cell_t ka_cell 		= DOT_1 | DOT_3;
cell_t kha_cell 	= DOT_4 | DOT_6;
cell_t ga_cell 		= DOT_1 | DOT_2 | DOT_4 | DOT_5;
cell_t gha_cell 	= DOT_1 | DOT_2 | DOT_6;
cell_t nna_cell 	= DOT_3 | DOT_4 | DOT_6;

cell_t cha_cell 	= DOT_1 | DOT_4;
cell_t chha_cell 	= DOT_1 | DOT_6;
cell_t ja_cell 		= DOT_2 | DOT_4 | DOT_5;
cell_t jha_cell 	= DOT_3 | DOT_5 | DOT_6;
cell_t nyaa_cell 	= DOT_2 | DOT_5;

cell_t tta_cell 	= DOT_2 | DOT_3 | DOT_4 | DOT_5 | DOT_6;
cell_t ttha_cell	= DOT_2 | DOT_4 | DOT_5 | DOT_6;
cell_t dda_cell 	= DOT_1 | DOT_2 | DOT_4 | DOT_6;
cell_t ddha_cell	= DOT_1 | DOT_2 | DOT_3 | DOT_4 | DOT_5 | DOT_6;
cell_t gna_cell		= DOT_3 | DOT_4 | DOT_5 | DOT_6;

cell_t ta_cell 		= TA;
cell_t tha_cell 	= DOT_1 | DOT_4 | DOT_5 | DOT_6;
cell_t da_cell 		= DOT_1 | DOT_4 | DOT_5;
cell_t dha_cell 	= DOT_2 | DOT_3 | DOT_4 | DOT_6;
cell_t na_cell 		= DOT_1 | DOT_3 | DOT_4 | DOT_5;

cell_t pa_cell 		= DOT_1 | DOT_2 | DOT_3 | DOT_4;
cell_t pha_cell 	= DOT_2 | DOT_3 | DOT_5;
cell_t ba_cell 		= DOT_1 | DOT_2;
cell_t bha_cell		= DOT_4 | DOT_5;
cell_t ma_cell 		= DOT_1 | DOT_3 | DOT_4;

cell_t ya_cell		= DOT_1 | DOT_3 | DOT_4 | DOT_5 | DOT_6;
cell_t ra_cell		= RA;
cell_t la_cell		= DOT_1 | DOT_2 | DOT_3;
cell_t va_cell		= DOT_1 | DOT_2 | DOT_3 | DOT_6;

cell_t sha_cell		= SHA;
cell_t shha_cell	= DOT_1 | DOT_2 | DOT_3 | DOT_4 | DOT_6;
cell_t sa_cell		= DOT_2 | DOT_3 | DOT_4;
cell_t ha_cell		= DOT_1 | DOT_2 | DOT_5;

cell_t ksha_cell	= DOT_1 | DOT_2 | DOT_3 | DOT_4 | DOT_5;
cell_t jna_cell		= DOT_1 | DOT_5 | DOT_6;

// multicell letters

cell_t ru_cells[2] 		= {ABBREV, RA};
cell_t tra_cells[3]		= {HALANT, TA, RA};
cell_t shra_cells[3]	= {ABBREV, SHA, RA};

#define HIND_A 		{&hin_a_cell, 1, "a", HINDI}
#define HIND_AA 	{&aa_cell, 1, "aa", HINDI}
#define HIND_I 		{&hin_i_cell, 1, "i", HINDI}
#define HIND_II 	{&ii_cell, 1, "ii", HINDI}
#define HIND_U 		{&hin_u_cell, 1, "u", HINDI}
#define HIND_UU		{&uu_cell, 1, "uu", HINDI}
#define HIND_RU		{ru_cells, 2, "ru", HINDI}

#define HIND_EE 	{&ee_cell, 1, "ee", HINDI}
#define HIND_AI 	{&ai_cell, 1, "ai", HINDI}
#define HIND_OO 	{&oo_cell, 1, "oo", HINDI}
#define HIND_AU 	{&au_cell, 1, "au", HINDI}
#define HIND_AM 	{&am_cell, 1, "am", HINDI}
#define HIND_AHA 	{&aha_cell, 1, "aha", HINDI}

#define HIND_KA 	{&ka_cell, 1, "ka", HINDI}
#define HIND_KHA 	{&kha_cell, 1, "kha", HINDI}
#define HIND_GA 	{&ga_cell, 1, "ga", HINDI}
#define HIND_GHA 	{&gha_cell, 1, "gha", HINDI}
#define HIND_NNA 	{&nna_cell, 1, "nna", HINDI}

#define HIND_CHA 	{&cha_cell, 1, "cha", HINDI}
#define HIND_CHHA 	{&chha_cell, 1, "chha", HINDI}
#define HIND_JA		{&ja_cell, 1, "ja", HINDI}
#define HIND_JHA	{&jha_cell, 1, "jha", HINDI}
#define HIND_NYAA 	{&nyaa_cell, 1, "nyaa", HINDI}

#define HIND_TTA 	{&tta_cell, 1, "tta", HINDI}
#define HIND_TTHA 	{&ttha_cell, 1, "ttha", HINDI}
#define HIND_DDA 	{&dda_cell, 1, "dda", HINDI}
#define HIND_DDHA 	{&ddha_cell, 1, "ddha", HINDI}
#define HIND_GNA 	{&gna_cell, 1, "gna", HINDI}

#define HIND_TA 	{&ta_cell, 1, "ta", HINDI}
#define HIND_THA 	{&tha_cell, 1, "tha", HINDI}
#define HIND_DA 	{&da_cell, 1, "da", HINDI}
#define HIND_DHA 	{&dha_cell, 1, "dha", HINDI}
#define HIND_NA 	{&na_cell, 1, "na", HINDI}

#define HIND_PA 	{&pa_cell, 1, "pa", HINDI}
#define HIND_PHA 	{&pha_cell, 1, "pha", HINDI}
#define HIND_BA 	{&ba_cell, 1, "ba", HINDI}
#define HIND_BHA 	{&bha_cell, 1, "bha", HINDI}
#define HIND_MA 	{&ma_cell, 1, "ma", HINDI}

#define HIND_YA 	{&ya_cell, 1, "ya", HINDI}
#define HIND_RA 	{&ra_cell, 1, "ra", HINDI}
#define HIND_LA 	{&la_cell, 1, "la", HINDI}
#define HIND_VA 	{&va_cell, 1, "va", HINDI}

#define HIND_SHA 	{&sha_cell, 1, "sha", HINDI}
#define HIND_SHHA 	{&shha_cell, 1, "shha", HINDI}
#define HIND_SA 	{&sa_cell, 1, "sa", HINDI}
#define HIND_HA 	{&ha_cell, 1, "ha", HINDI}

#define HIND_KSHA 	{&ksha_cell, 1, "ksha", HINDI}
#define HIND_TRA 	{tra_cells, 3, "tra", HINDI}
#define HIND_JNA 	{&jna_cell, 1, "jna", HINDI}
#define HIND_SHRA 	{shra_cells, 3, "shra", HINDI}

letter_t hindi_letters[50] = { HIND_A, HIND_AA,
    HIND_I, HIND_II, HIND_U, HIND_UU, HIND_RU, HIND_EE,
    HIND_AI, HIND_OO, HIND_AU, HIND_AM, HIND_AHA, HIND_KA,
    HIND_KHA, HIND_GA, HIND_GHA, HIND_NNA, HIND_CHA, HIND_CHHA,
    HIND_JA, HIND_JHA, HIND_NYAA, HIND_TTA, HIND_TTHA, HIND_DDA,
    HIND_DDHA, HIND_GNA, HIND_TA, HIND_THA, HIND_DA, HIND_DHA,
    HIND_NA, HIND_PA, HIND_PHA, HIND_BA, HIND_BHA, HIND_MA,
    HIND_YA, HIND_RA, HIND_LA, HIND_VA, HIND_SHA, HIND_SHHA,
    HIND_SA, HIND_HA, HIND_KSHA, HIND_TRA, HIND_JNA, HIND_SHRA };

alphabet_t hindi_alphabet = {hindi_letters, 50, HINDI};