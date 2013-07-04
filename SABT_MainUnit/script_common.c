/**
 * @file script_common.c
 * @brief Code for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "glyph.h"
#include "script_common.h"
#include "audio.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// *NOTE* - Common script MP3s must be part of every language
#define SCRIPT_COMMON_DEFAULT_FILESET "ENG_"

// Numbers script bit patterns
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

// Numbers script array
static glyph_t glyphs_common[SCRIPT_COMMON_LENGTH] = {
	{NUM1, "#1", NULL},
	{NUM2, "#2", NULL},
	{NUM3, "#3", NULL},
	{NUM4, "#4", NULL},
	{NUM5, "#5", NULL},
	{NUM6, "#6", NULL},
	{NUM7, "#7", NULL},
	{NUM8, "#8", NULL},
	{NUM9, "#9", NULL},
	{NUM0, "#0", NULL},
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
		if (this_glyph->cell_pattern == pattern) {
			return this_glyph;
		}
	}

	// Search common script
	script_glyphs = script_common.glyphs;
	script_length = script_common.length;
	for (int i = 0; i < script_common.length; i++) {
		this_glyph = &script_glyphs[i];
		if (this_glyph->cell_pattern == pattern) {
			return this_glyph;
		}
	}

	// If nothing matches, return NULL
	return NULL;
}
