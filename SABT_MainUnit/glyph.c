/*
* @file glyph.c
* @brief Common functions involving glyphs
* @author Vivek Nair (viveknair@cmu.edu)
*/

#include <stdbool.h>
#include <stddef.h>
#include "glyph.h"

/*
* @brief Compares 2 glyphs
* @param glyph_t* g1, g2 - The 2 glyphs to compare
* @bool true if cell patterns match, false otherwise
*/
bool glyph_equals(glyph_t* g1, glyph_t* g2) {
	if (g1 == NULL || g2 == NULL) {
		return false;
	} else {
		return (g1->cell_pattern == g2->cell_pattern);
	}
}

