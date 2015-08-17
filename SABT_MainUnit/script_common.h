/**
 * @file script_common.h
 * @brief Definitions for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_COMMON_H_
#define _SCRIPT_COMMON_H_ 

#include "datastructures.h"
#include <stdbool.h>
#include <stddef.h>

extern script_t* lang_script;
extern script_t script_common;

void reset_script_queue(script_t* script, bool should_shuffle);
glyph_t* get_glyph(script_t* script, char* patterns, int* index);
glyph_t* search_script(script_t* script, char pattern);
glyph_t* get_next_letter(script_t* script, bool should_shuffle);
glyph_t* get_prev_letter(script_t* script);
bool is_number(glyph_t* curr_glyph);
int get_digit(glyph_t* curr_glyph);
bool is_blank(glyph_t* curr_glyph);
glyph_t* get_root(script_t* curr_script, glyph_t* curr_glyph);
word_node_t* word_to_glyph_word(script_t* curr_script, char* word);
word_node_t* free_word_old(word_node_t* this_word);
word_node_t* add_glyph_to_word(word_node_t* curr_word, glyph_t* added_glyph);

#endif /* _SCRIPT_COMMON_H_ */
