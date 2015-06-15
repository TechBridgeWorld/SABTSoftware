#include "script_digits.h"

glyph_t glyphs_digits[SCRIPT_DIGITS_LENGTH] = {
	{NUM1, "#1", NULL, NULL},
	{NUM2, "#2", NULL, NULL},
	{NUM3, "#3", NULL, NULL},
	{NUM4, "#4", NULL, NULL},
	{NUM5, "#5", NULL, NULL},
	{NUM6, "#6", NULL, NULL},
	{NUM7, "#7", NULL, NULL},
	{NUM8, "#8", NULL, NULL},
	{NUM9, "#9", NULL, NULL},
	{NUM0, "#0", NULL, NULL},
	{NUMSIGN, "#NUM", NULL, NULL}
};

script_t script_digits = {
	SCRIPT_DIGITS_LENGTH,
	0,
	SCRIPT_DIGITS_FILESET,
	glyphs_digits
};