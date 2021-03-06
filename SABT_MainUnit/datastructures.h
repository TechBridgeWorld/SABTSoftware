/**
 * @file datastructures.h
 * @brief Structures and declarations for all the data structures
 * @author Marjorie Carlson (marjorie@cmu.edu)
 */

#ifndef _DATASTRUCTURES_H_
#define _DATASTRUCTURES_H_

#include <stdbool.h>
#include "macros.h"

#define MAX_MP3_NAME_LENGTH  7      // 6 + /0
#define MAX_FILESET_LENGTH   3      // 2 + /0
#define MAX_WORD_LENGTH    15
#define MAX_WORDLIST_LENGTH 10

// Stores information about single glyph; used to build scripts
struct glyph {
    char pattern;           /* 0bxxxxxx 6-bit pattern Braille representation */
    char sound[MAX_MP3_NAME_LENGTH];    /* BBBB in AAA_BBBB.mp3 soundfile */
    glyph_t* prev;          /* Pointer to previous glyph in linked list */
    glyph_t* next;          /* Pointer to next glyph in linked list */
};

// @todo: pack structs
// @todo: work out which structs should be globals (e.g. current_word?)

typedef unsigned char cell_t;    // holds a 6-bit bitfield representing one braille cell

typedef struct letter {
    cell_t*    cells; // an array of cells in that letter. In most cases, this will only be one cell.
    uint8_t    num_cells;

    char       name[MAX_MP3_NAME_LENGTH];
    language_t language_of_origin;
} letter_t;

typedef struct word {  // @todo Pack this
    letter_t*  letters;
    uint8_t    num_letters;
    // curr_letter and curr_glyph are indices used when checking input against
    // the currently selected word. The next cell we're expecting from the user
    // is word->letters[curr_letter][curr_glyph].
    int        curr_letter;
    int        curr_glyph; 
    char       name[MAX_WORD_LENGTH];
    language_t language_of_origin;
} word_t;

typedef struct alphabet {
    letter_t*  letters;
    uint8_t    num_letters;
    language_t language_of_origin;
    // other info can eventually go in here, like whether capitalization occurs
    // or what numbers to use. (Currently, numbers must be considered a separate
    // alphabet.)
} alphabet_t;

// a wordlist is shufflable. The word array doesn't change, but a second array
// of 8-bit ints is kept, called "order", which consists of indices into words.
// to shuffle the wordlist, "order" is shuffled. It can then be iterated through,
// using index, to 
typedef struct wordlist {
    word_t*  words;
    int* order_of_words;
    int  num_words;
    int  index_into_order;
} wordlist_t;

// Structure representing a script (alphabet) - deprecated
typedef struct script_old {
    int length;             /* Length of first cell glyph array */
    int index;              /* Current index */
    char fileset[MAX_FILESET_LENGTH];       /* Fileset on SD card; 4 characters long */
    glyph_t* glyphs;        /* Pointer to array of first cell glyphs */
} script_old_t;

// Structure representing a script/alphabet.
// Now represents both the glyphs and the letters, which may be more than
// one glyph each.

typedef struct script {
    int length;                       /* Length of glyph array */
    int num_letters;                  /* Number of actual letters (<length) */
    int index;                        /* Current index */
    char fileset[MAX_FILESET_LENGTH]; /* Fileset on SD card; 4 characters long */
    glyph_t* glyphs;                  /* Pointer to array of glyphs */
    int* letters;                     /* Pointer to array of valid indices into glyphs */
} script_t;


typedef struct word_node {
    glyph_t* data;
    struct word_node* next; 
} word_node_t;

// @todo Remove the ones that should only be helper functions

// Cell functions
void  print_cell_pattern(cell_t cell);
bool  glyph_equals(glyph_t* g1, glyph_t* g2); // deprecated

// Letter functions
bool  letter_equals(letter_t* letter1, letter_t* letter2);
letter_t* get_eng_letter_by_char(char c);
char* get_eng_letter_name_by_cell(cell_t cell);
void  print_letter(letter_t* letter);

// Word functions
int   parse_string_into_eng_word(char* string, word_t* word);
void  word_to_cell_array(word_t* word, cell_t* arr);
void  decrement_word_index(word_t* word);
void  increment_word_index(word_t* word);
cell_t get_next_cell_in_word(word_t* word);
char* get_next_letter_name(word_t* word);
char* get_lang(word_t* word);
void  print_word(word_t* word);
void  free_word(word_t* word);
void  speak_word(word_t* word);
void  speak_letters_in_word(word_t* word);
void  speak_letters_so_far(word_t* word);

// Wordlist functions
void  initialize_wordlist(word_t* words, int num_words, wordlist_t* list);
void  strings_to_wordlist(char** strings, int num_strings, wordlist_t* list); // BUGGY
void  print_words_in_list(wordlist_t* wl);
void  get_next_word_in_wordlist(wordlist_t* wl, word_t** next_word);
void  free_wordlist(wordlist_t* wl);

int   random_between(int i, int j);
void  shuffle(int* int_array, int len);
void  sort(int* int_array, int len);

#endif /* _DATASTRUCTURES_H_ */
