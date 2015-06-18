#include "script_eng_contraction.h"

glyph_t contraction_glyphs[SCRIPT_CONTRACTION_LENGTH] = {
	{ENG_A, "A", NULL, NULL},
	{ENG_B, "B", NULL, NULL},
	{ENG_C, "C", NULL, NULL},
	{ENG_D, "D", NULL, NULL},
	{ENG_E, "E", NULL, NULL},
	{ENG_F, "F", NULL, NULL},
	{ENG_G, "G", NULL, NULL},
	{ENG_H, "H", NULL, NULL},
	{ENG_I, "I", NULL, NULL},
	{ENG_J, "J", NULL, NULL},
	{ENG_K, "K", NULL, NULL},
	{ENG_L, "L", NULL, NULL},
	{ENG_M, "M", NULL, NULL},
	{ENG_N, "N", NULL, NULL},
	{ENG_O, "O", NULL, NULL},
	{ENG_P, "P", NULL, NULL},
	{ENG_Q, "Q", NULL, NULL},
	{ENG_R, "R", NULL, NULL},
	{ENG_S, "S", NULL, NULL},
	{ENG_T, "T", NULL, NULL},
	{ENG_U, "U", NULL, NULL},
	{ENG_V, "V", NULL, NULL},
	{ENG_W, "W", NULL, NULL},
	{ENG_X, "X", NULL, NULL},
	{ENG_Y, "Y", NULL, NULL},
	{ENG_Z, "Z", NULL, NULL},
	{CON_S1, "S1", NULL, NULL},
	{CON_S2, "S2", NULL, NULL},
	{CON_S3, "S3", NULL, NULL},
	{CON_S4, "S4", NULL, NULL},
    {CON_S5, "S5", NULL, NULL},
	{CON_S6, "S6", NULL, NULL},
	{CON_S7, "S7", NULL, NULL},
	{CON_S8, "S8", NULL, NULL},
	{CON_S9, "S9", NULL, NULL},
	{CON_S10, "S10", NULL, NULL},
};

glyph_t contraction_pattern[SCRIPT_CONTRACTION_PATTERN_LENGTH] = {
	{0, "P0", NULL, NULL},						
	{CON_P1, "P1", NULL, NULL},
	{CON_P2, "P2", NULL, NULL},
	{CON_P3, "P3", NULL, NULL},
	{CON_P4, "P4", NULL, NULL},
	{CON_P5, "P5", NULL, NULL},
	{CON_P6, "P6", NULL, NULL}
};

// Patterns used to form braille cells  

script_old_t script_eng_contraction = {
	SCRIPT_CONTRACTION_LENGTH,
	0,
	SCRIPT_CONTRACTION_FILESET,
	contraction_glyphs
};