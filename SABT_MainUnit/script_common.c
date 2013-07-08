/**
 * @file script_common.c
 * @brief Code for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "glyph.h"
#include "script_common.h"
#include "audio.h"
#include "globals.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// *NOTE* - Common script MP3s must be part of every language
#define SCRIPT_COMMON_DEFAULT_FILESET "ENG_"

// Numbers script bit patterns
#define BLANK				0b000000
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
#define NUMSIGN			0b111100

// Numbers script array
static glyph_t glyphs_common[SCRIPT_COMMON_LENGTH] = {
	{BLANK, true, "BLNK", NULL},
	{NUM1, true, "#1", NULL},
	{NUM2, true, "#2", NULL},
	{NUM3, true, "#3", NULL},
	{NUM4, true, "#4", NULL},
	{NUM5, true, "#5", NULL},
	{NUM6, true, "#6", NULL},
	{NUM7, true, "#7", NULL},
	{NUM8, true, "#8", NULL},
	{NUM9, true, "#9", NULL},
	{NUM0, true, "#0", NULL},
	{NUMSIGN, true, "#NUM", NULL}
};

script_t script_common = {
	SCRIPT_COMMON_LENGTH,
	SCRIPT_COMMON_DEFAULT_FILESET,
	glyphs_common
};

script_t* lang_script = NULL;

/**
 * @brief Finds an glyph from based on a cell pattern, checks language script
 * and then the common script if no match found in language
 * @param char pattern - Cell pattern to look for
 * @return glyph_t* - Corresponding to glyph it found, NULL if not found
 */
glyph_t* get_glyph_by_pattern(char pattern) {
	glyph_t* this_glyph = NULL;

	// First search lang script
	glyph_t* script_glyphs = lang_script->glyphs;
	int script_length = lang_script->length;
	for (int i = 0; i < script_length; i++) {
		this_glyph = &script_glyphs[i];
		if (this_glyph->pattern == pattern) {
			return this_glyph;
		}
	}

	// Search common script
	script_glyphs = script_common.glyphs;
	script_length = script_common.length;
	for (int i = 0; i < script_common.length; i++) {
		this_glyph = &script_glyphs[i];
		if (this_glyph->pattern == pattern) {
			return this_glyph;
		}
	}

	// If nothing matches, return NULL
	sprintf(dbgstr, "[Script] Glyph match not found for pattern: %x\n\r",
		pattern);
	PRINTF(dbgstr);
	return NULL;
}

/*
* @brief Checks if a given glyph is a number
* @param glyph_t* Pointer to glyph to check
* @return bool - true if a number, false otherwise
*/
bool is_number(glyph_t* curr_glyph) {
	if (curr_glyph == NULL)
		return false;
	switch (curr_glyph->pattern) {
		case NUM1: case NUM2: case NUM3: case NUM4: case NUM5: case NUM6:
		case NUM7: case NUM8: case NUM9: case NUM0:
			return true;
			break;
		default:
			return false;
			break;
	}
}

/*
* @brief Converts a digit glyph to an integer
* @param glyph_t* - Pointer to glyph to convet
* @return int - 0-9 for valid digit, -1 otherwise
*/
int get_digit(glyph_t* curr_glyph) {
	if (curr_glyph == NULL)
		return -1;
	if (!is_number(curr_glyph)) {
		return -1;
	}

	switch(curr_glyph->pattern) {
		case NUM1: return 1; break;
		case NUM2: return 2; break;
		case NUM3: return 3; break;
		case NUM4: return 4; break;
		case NUM5: return 5; break;
		case NUM6: return 6; break;
		case NUM7: return 7; break;
		case NUM8: return 8; break;
		case NUM9: return 9; break;
		case NUM0: return 0; break;
		default: return -1; break;
	}
}

/*
* @brief Checks to see if a glyph is BLANK
* @param glyph_t* - Pointer to check
* @return bool - true if BLANk, false otherwise
*/
bool is_blank(glyph_t* curr_glyph) {
	if (curr_glyph == NULL)
		return false;
	else
		return (curr_glyph->pattern == BLANK);
}
