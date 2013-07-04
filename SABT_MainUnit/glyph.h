/**
 * @file glyph.h
 * @brief Structures for single glyphs and scripts
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _GLYPH_H_
#define _GLYPH_H_

#include <stdbool.h>

// Stores information about single glyph; used to build scripts
typedef struct glyph {
	char cell_pattern;			// 0b00xxxxxx 6-bit pattern Braille representation
	char sound[10];					// BBBB in AAA_BBBB.mp3, fileset is set by the file
	struct glyph* next_glyph;		// Pointer to next glyph if not the last in a
												// multi-cell glyph pattern, NULL otherwise
} glyph_t;

// Structure representing a script
typedef struct script {
	int length;							// Number of glyphs
	char fileset[5];				// Fileset on SD card; 4 characters long
	glyph_t* glyphs; 				// Pointer to array of glyphs
} script_t;

// Common glyph functions
bool glyph_equals(glyph_t* g1, glyph_t* g2);
bool glyph_sequence_equals(glyph_t* g1, glyph_t* g2);

#endif /* _GLYPH_H_ */
