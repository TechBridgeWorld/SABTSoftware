#include "script_english.h"

glyph_t english_glyphs[SCRIPT_ENGLISH_LENGTH] = {
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
	{ENG_Z, "Z", NULL, NULL}
};

int english_letters[SCRIPT_ENGLISH_NUM_LETTERS] = {
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25 };


script_t script_english = {
	SCRIPT_ENGLISH_LENGTH,
	SCRIPT_ENGLISH_LENGTH,
	0,
	SCRIPT_ENGLISH_FILESET,
	english_glyphs,
	english_letters
};