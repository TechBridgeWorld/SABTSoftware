/**
 * @file glyph.h
 * @brief Structures for single glyphs and scripts
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _GLYPH_H_
#define _GLYPH_H_

#include <stdbool.h>

typedef struct glyph glyph_t;
typedef struct script script_t;

// Stores information about single glyph; used to build scripts
struct glyph {
	char pattern;					/* 0bxxxxxx 6-bit pattern Braille representation */
	bool is_valid;				/* If "path" to this glyph is a valid Braille char
														Is false for first glyph in a multi-cell
														character that is not a valid character in
														itself; true for other glyphs */
	char sound[5];				/* BBBB in AAA_BBBB.mp3 soundfile */
	script_t* subscript;	/* Pointer to array of possible next glyphs in
														cell sequence if not the last in a multi-cell
														character, NULL otherwise */
};

// Structure representing a script
struct script {
	int length;					/* Length of first cell glyph array */
	char fileset[5];		/* Fileset on SD card; 4 characters long */
	glyph_t* glyphs; 		/* Pointer to array of first cell glyphs */
};

// Common glyph functions
bool glyph_equals(glyph_t* g1, glyph_t* g2);

#endif /* _GLYPH_H_ */
