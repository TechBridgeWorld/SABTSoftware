/**
 * @file script_common.h
 * @brief Definitions for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_COMMON_H_
#define _SCRIPT_COMMON_H_ 

#include "glyph.h"
#include <stdbool.h>
#include <stddef.h>

#define SYS_FILESET "SYS_"

// Common SYS sounds
#define MP3_TADA "TADA"

// Common lang-inspecific MP3 prompts

#define MP3_INVALID_PATTERN "INVP"
#define MP3_CORRECT "CORR"
#define MP3_INCORRECT "WRNG"
#define MP3_TRY_AGAIN "TAGA"
 
#define MP3_FIRST_CELL "FCEL"
#define MP3_LAST_CELL "LCEL"
#define MP3_NEXT_CELL "NCEL"
#define MP3_PREV_CELL "PCEL"
#define MP3_NEXT_LETTER "NLET"
#define MP3_BLANK "BLNK"
#define MP3_NEXT_PATTERN "NPAT"
#define MP3_NUMSIGN_MISSED "NUMS"
#define MP3_NO_NUMBER "NNUM"

extern script_t* lang_script;
extern script_t script_common;

void reset_script_indices(script_t* script);
void reset_script_queue(script_t* script, bool should_shuffle);
void shuffle_alphabet(script_t* script);
void unshuffle_alphabet(script_t* script);
glyph_t* get_glyph(script_t* script, char* patterns, int* index);
glyph_t* search_script(script_t* script, char pattern);
glyph_t* get_next_glyph(script_t* script, bool should_shuffle);
glyph_t* get_prev_glyph(script_t* script, bool should_shuffle);
bool is_number(glyph_t* curr_glyph);
int get_digit(glyph_t* curr_glyph);
bool is_blank(glyph_t* curr_glyph);
glyph_t* get_next(script_t* curr_script, glyph_t* curr_glyph);
glyph_t* get_root(script_t* curr_script, glyph_t* curr_glyph);
word_node_t* word_to_glyph_word(script_t* curr_script, char* word);
word_node_t* free_word(word_node_t* this_word);
word_node_t* add_glyph_to_word(word_node_t* curr_word, glyph_t* added_glyph);

#endif /* _SCRIPT_COMMON_H_ */
