/**
 *  @file script_digits.h
 *  @brief Structures for digits
 *  @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_DIGITS_H_
#define _SCRIPT_DIGITS_H_

#include "glyph.h"
#include "script_digits.h"
#include <stddef.h>

// Language fileset can be overriden
#define SCRIPT_DIGITS_FILESET "ENG_"
#define SCRIPT_DIGITS_LENGTH 11

// Digit bit patterns
#define NUM1 				0b000001
#define NUM2				0b000011
#define NUM3				0b001001
#define NUM4				0b011001
#define NUM5				0b010001
#define NUM6				0b001011
#define NUM7				0b011011
#define NUM8				0b010011
#define NUM9				0b001010
#define NUM0				0b011010
#define NUMSIGN				0b111100

static glyph_t glyphs_digits[SCRIPT_DIGITS_LENGTH] = {
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

static script_t script_digits = {
	SCRIPT_DIGITS_LENGTH,
	0,
	SCRIPT_DIGITS_FILESET,
	glyphs_digits
};

#endif /* _SCRIPT_DIGiTS_H_ */

