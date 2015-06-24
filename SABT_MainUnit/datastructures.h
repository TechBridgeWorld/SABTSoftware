/**
 * @file datastructures.h
 * @brief Structures for single glyphs and scripts
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _DATASTRUCTURES_H_
#define _DATASTRUCTURES_H_

#include <stdbool.h>

typedef struct glyph glyph_t;
typedef struct letter letter_t;
typedef struct word word_t;
typedef struct alphabet alphabet_t;
typedef struct script script_t;
typedef struct script_old script_old_t;
typedef struct word_node word_node_t;

// Stores information about single glyph; used to build scripts
struct glyph{
	char pattern;			/* 0bxxxxxx 6-bit pattern Braille representation */
	char sound[5];			/* BBBB in AAA_BBBB.mp3 soundfile */
	glyph_t* prev;			/* Pointer to previous glyph in linked list */
	glyph_t* next;			/* Pointer to next glyph in linked list */
};

struct letter{
	int num_glyphs;
	glyph_t* first_glyph;
	char sound[5];
};

struct word{
	int num_letters;
	letter_t* first_letter;
	char sound[5];
};

struct alphabet{
	int num_letters;
	letter_t* first_letter;
	char sound[5];
};

// Structure representing a script (alphabet) - deprecated
struct script_old {
	int length;				/* Length of first cell glyph array */
	int index;				/* Current index */
	char fileset[5];		/* Fileset on SD card; 4 characters long */
	glyph_t* glyphs; 		/* Pointer to array of first cell glyphs */
};

// Structure representing a script/alphabet.
// Now represents both the glyphs and the letters, which may be more than
// one glyph each.
struct script {
	int length;				/* Length of glyph array */
	int num_letters;			/* Number of actual letters (<= length) */
	int index;				/* Current index */
	char fileset[5];		/* Fileset on SD card; 4 characters long */
	glyph_t* glyphs; 		/* Pointer to array of glyphs */
	int* letters;		/* Pointer to array of valid indices into glyphs */
	};


struct word_node {
	glyph_t* data;
	struct word_node* next;	
};

// Common glyph functions
bool glyph_equals(glyph_t* g1, glyph_t* g2);

#endif /* _DATASTRUCTURES_H_ */
