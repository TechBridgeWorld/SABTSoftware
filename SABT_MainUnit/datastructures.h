/**
 * @file datastructures.h
 * @brief Structures and declarations for all the data structures
 * @author Marjorie Carlson (marjorie@cmu.edu)
 */

#ifndef _DATASTRUCTURES_H_
#define _DATASTRUCTURES_H_

#include <stdbool.h>

   /*********************
  *  SWITCH DEBUG MODE *
 *   ON & OFF HERE!!! *
*********************/

//this allows you to run datastructures & datastructurestest on a PC
//#define DEBUGMODE

   /*^^^^^^^^^^^^^^^^^^^*
  *  SWITCH DEBUG MODE *
 *   ON & OFF HERE!!! *
*********************/

//@todo move some of these to global?
#define MAX_WORD_LENGTH     15
#define MAX_WORDLIST_LENGTH 10
#define MAX_FILESET_LENGTH	  5
#define MAX_MP3_NAME_LENGTH  5


#define DOT0     0b000000
#define DOT1     0b000001
#define DOT2     0b000010
#define DOT3     0b000100
#define DOT4     0b001000
#define DOT5     0b010000
#define DOT6     0b100000

typedef enum {UNIVERSAL, ENGLISH, HINDI, KANNADA} lang_type;

typedef struct glyph glyph_t;

// Stores information about single glyph; used to build scripts
struct glyph {
	char pattern;			/* 0bxxxxxx 6-bit pattern Braille representation */
	char sound[MAX_MP3_NAME_LENGTH];	/* BBBB in AAA_BBBB.mp3 soundfile */
	glyph_t* prev;			/* Pointer to previous glyph in linked list */
	glyph_t* next;			/* Pointer to next glyph in linked list */
};

// @todo: pack structs
// @todo: work out which structs should be globals (e.g. current_word?)

typedef struct cell {
	char pattern;			/* 0bxxxxxx 6-bit pattern Braille representation */
} cell_t;

typedef struct letter {
	char name[MAX_MP3_NAME_LENGTH];
	char lang_enum;
	cell_t* cells;
	int num_cells;
} letter_t;

typedef struct word {
	char name[MAX_WORD_LENGTH];
	int length_name;
	letter_t* letters;
	int num_letters;
	char lang_enum;
	int curr_letter;
	int curr_glyph;
} word_t; 

typedef struct alphabet {
	letter_t* letters;
	int num_letters;
	char lang_enum;
	// other info can eventually go in here
} alphabet_t;

typedef struct wordlist {
	word_t* words;
	int* order;
	int num_words;
	int index;
} wordlist_t;

// Structure representing a script (alphabet) - deprecated
typedef struct script_old {
	int length;				/* Length of first cell glyph array */
	int index;				/* Current index */
	char fileset[MAX_FILESET_LENGTH];		/* Fileset on SD card; 4 characters long */
	glyph_t* glyphs; 		/* Pointer to array of first cell glyphs */
} script_old_t;

// Structure representing a script/alphabet.
// Now represents both the glyphs and the letters, which may be more than
// one glyph each.

typedef struct script {
	int length;				          /* Length of glyph array */
	int num_letters;			      /* Number of actual letters (<length) */
	int index;			         	  /* Current index */
	char fileset[MAX_FILESET_LENGTH]; /* Fileset on SD card; 4 characters long */
	glyph_t* glyphs; 		          /* Pointer to array of glyphs */
	int* letters;		              /* Pointer to array of valid indices into glyphs */
} script_t;


typedef struct word_node {
	glyph_t* data;
	struct word_node* next;	
} word_node_t;

// @todo Remove the ones that should only be helper functions

// Cell functions
void print_cell_pattern(cell_t* cell);
bool cell_equals(cell_t* cell1, cell_t* cell2);
bool glyph_equals(glyph_t* g1, glyph_t* g2); // deprecated

// Letter functions
bool letter_equals(letter_t* letter1, letter_t* letter2);
letter_t* get_eng_letter_by_char(char c);
void print_letter(letter_t* letter);

// Word functions
int parse_string_into_eng_word(char* string, word_t* word);
void word_to_cell_array(word_t* word, cell_t* arr);
void decrement_word_index(word_t* word);
void increment_word_index(word_t* word);
void get_next_cell_in_word(word_t* word, cell_t* next_cell);
char* get_lang(word_t* word);
void print_word(word_t* word);
void free_word(word_t* word);
void speak_word(word_t* word);
void speak_letters_in_word(word_t* word);
void speak_letters_so_far(word_t* word);

// Wordlist functions
void initialize_wordlist(word_t* words, int num_words, wordlist_t* list);
void strings_to_wordlist(char** strings, int num_strings, wordlist_t* list); // BUGGY
void print_words_in_list(wordlist_t* wl);
void get_next_word_in_wordlist(wordlist_t* wl, word_t** next_word);
void free_wordlist(wordlist_t* wl);

int random_between(int i, int j);
void shuffle(int len, int* int_array);
void unshuffle(int len, int* int_array);

#endif /* _DATASTRUCTURES_H_ */
