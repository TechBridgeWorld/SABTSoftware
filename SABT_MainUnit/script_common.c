/**
 * @file script_common.c
 * @brief Code for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "glyph.h"
#include "script_common.h"
#include "audio.h"
#include "globals.h"
#include "common.h"
#include "io.h"

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
#define NUMSIGN				0b111100

// Numbers script array
static glyph_t glyphs_common[SCRIPT_COMMON_LENGTH] = {
	{BLANK, MP3_BLANK, NULL, NULL},
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

script_t script_common = {
	SCRIPT_COMMON_LENGTH,
	0,
	SCRIPT_COMMON_DEFAULT_FILESET,
	glyphs_common
};

script_t* lang_script = NULL;

/*
* @brief Resets a script's indices
* @param script_t* - Pointer to script to reset
* @return void
*/
void reset_script_indices(script_t* script) {
	script->index = -1;
	for (int i = 0; i < script->length; i++) {
		if (script->glyphs[i].subscript != NULL) {
			reset_script_indices(script->glyphs[i].subscript);
		}
	}
}

/*
* @brief Pattern matches from a string of patterns to a glyph by traversing
*	the script provided
* @param script_t* script - Pointer to script to search in
* @param char* pattern - Pointer to pattern array to search for
* @param int* index - Index to update as parsing continues
* @return glyph_t* - Pointer to glyph, NULL if not found
*/
glyph_t* get_glyph(script_t* script, char* patterns, int* index) {
	char curr_pattern = patterns[*index];
	script_t* subscript = NULL;
	glyph_t* curr_glyph;

	sprintf(dbgstr, "[IO] Current pattern: 0x%x\n\r", curr_pattern);
	PRINTF(dbgstr);
	
	// Return if EOT
	if (curr_pattern == END_OF_TEXT) {
		PRINTF("[IO] Current pattern is EOT; returning NULL\n\r");
		return NULL;
	}

	// If no match found in script, return NULL
	curr_glyph = search_script(script, curr_pattern);
	if (curr_glyph == NULL) {
		curr_glyph = search_script(&script_common, curr_pattern);
		if (curr_glyph == NULL) {
			PRINTF("[IO] Matching glyph not found; returning NULL\n\r");
			return NULL;
		}
	}
	subscript = curr_glyph->subscript;
	
	// If glyph does not have subscript, check subscript
	if (curr_glyph->subscript != NULL) {
		PRINTF("[IO] Recursively checking subscript for next pattern\n\r");
		*index = *index + 1;
		return get_glyph(subscript, patterns, index);
	
	// Otherwise return subscript
	} else {
		PRINTF("[IO] No subscript; returning glyph\n\r");
		return curr_glyph;
	}
}

/**
 * @brief Finds an glyph from based on a cell pattern, checks language script
 * and then the common script if no match found in language
 * @param	script_t* script - Script to look in
 * @param char pattern - Cell pattern to look for
 * @return glyph_t* - Corresponding to glyph it found, NULL if not found
 */
glyph_t* search_script(script_t* curr_script, char pattern) {
	glyph_t* curr_glyph = NULL;
	int index_bound = 0;
	index_bound = curr_script->length;

	// Search through array of glyphs
	for (int glyph_index = 0; glyph_index < index_bound; glyph_index++) {
		curr_glyph = &(curr_script->glyphs[glyph_index]);
		if (curr_glyph && curr_glyph->pattern == pattern) {
			return curr_glyph;
		}
	}

	// If nothing matches, return NULL
	sprintf(dbgstr, "[Script] Glyph match not found: 0x%x\n\r", pattern);
	PRINTF(dbgstr);
	return NULL;
}

/*
* @brief Returns a random last-order glyph from the current script
* @param script_t* - Script to get random glyph from
* @return glyph_t* - Pointer to random glyph
*/
glyph_t* get_random_glyph(script_t* script) {
	glyph_t* curr_glyph = &(script->glyphs[timer_rand() % script->length]);
	if (curr_glyph->subscript == NULL) {
		return curr_glyph;
	} else {
		return get_random_glyph(curr_glyph->subscript);
	}
}

/*
* @brief Returns the next glyph from the current script
* @param void
* @glyph_t* - Pointer to next glyph
*/
glyph_t* get_next_glyph(script_t* script) {

	glyph_t* curr_glyph;
	glyph_t* next_glyph;

	script->index++;

	// Return NULL if reached end of script
	if (script->index >= script->length) {
		script->index = script->length;
		return NULL;
	}

	curr_glyph = &(script->glyphs[script->index]);

	// If current glyph is valid, update index and return
	if (curr_glyph->subscript == NULL) {
		return curr_glyph;
	// Otherwise recursively call on subscript
	} else {
		next_glyph = get_next_glyph(curr_glyph->subscript);
		// If glyph found in subscript, return
		if (next_glyph != NULL) {
			script->index--;
			return next_glyph;
		// Otherwise update superscript and call again
		} else {
			return get_next_glyph(script);
		}
	}
}

/*
* @brief Returns the previous glyph from the current script
* @param void
* @glyph_t* - Pointer to previous glyph
*/
glyph_t* get_prev_glyph(script_t* script) {

	glyph_t* curr_glyph;
	glyph_t* prev_glyph;

	script->index--;

	// Return NULL if reached end of script
	if (script->index <= -1) {
		script->index = -1;
		return NULL;
	}

	curr_glyph = &(script->glyphs[script->index]);

	// If current glyph is valid, update index and return
	if (curr_glyph->subscript == NULL) {
		return curr_glyph;
	// Otherwise recursively call on subscript
	} else {
		prev_glyph = get_prev_glyph(curr_glyph->subscript);
		// If glyph found in subscript, return
		if (prev_glyph != NULL) {
			script->index++;
			return prev_glyph;
		// Otherwise update superscript and call again
		} else {
			return get_prev_glyph(script);
		}
	}
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
