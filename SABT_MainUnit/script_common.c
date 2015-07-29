/**
 * @file script_common.c
 * @brief Code for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "datastructures.h"
#include "script_common.h"
#include "script_digits.h"
#include "letter_globals.h"
#include "audio.h"
#include "globals.h"
#include "common.h"
#include "io.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


script_t* lang_script = NULL;

glyph_t blank_cell = {
    0x00,
    MP3_BLANK,
    NULL,
    NULL
};

/**
* @brief Resets the alphabet and reshuffles or unshuffles
* it as needed.
* @param the alphabet's script struct and a bool indicating
* whether it should be shuffled (i.e., we're in "practice" mode)
* or not (i.e., we're in "learn" mode).
* @return void
*/
void reset_script_queue(script_t* script, bool should_shuffle) {
    script->index = -1;
    if (should_shuffle)
        shuffle(script->num_letters, script->letters);
}

/**
* @brief Pattern matches from a string of patterns to a glyph by traversing
*   the script provided
* @param script_t* script - Pointer to script to search in
* @param char* pattern - Pointer to pattern array to search for
* @param int* index - Index to update as parsing continues
* @return glyph_t* - Pointer to glyph, NULL if not found
*/
glyph_t* get_glyph(script_t* script, char* patterns, int* index) {
    char curr_pattern = patterns[*index];
    glyph_t* curr_glyph;

    // Return if EOT
    if (curr_pattern == END_OF_TEXT) {
        log_msg("[IO] Current pattern is EOT; returning NULL");
        return NULL;
    }

    if (curr_pattern == 0x00) {
        log_msg("[IO] Blank cell");
        return &blank_cell;
    }

    // If no match found in script, return NULL
    curr_glyph = search_script(script, curr_pattern);
    if (curr_glyph == NULL) {
        curr_glyph = search_script(&script_digits, curr_pattern);
        if (curr_glyph == NULL) {
            log_msg("[IO] Matching glyph not found; returning NULL");
            return NULL;
        }
    }
    return curr_glyph;
}

/**
 * @brief Finds an glyph from based on a cell pattern, checks language script
 * and then the common script if no match found in language
 * @param   script_t* script - Script to look in
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
        if ((curr_glyph != NULL) && (curr_glyph->pattern == pattern)) {
            log_msg("Search%s: %s (0x%x)",curr_script->fileset, curr_glyph->sound, curr_glyph->pattern);
            return curr_glyph;
        }
    }

    // If nothing matches, return NULL
    log_msg("[Script] Glyph match not found: 0x%x", pattern);    
    return NULL;
}

/**
* @brief Adds added_glyph to the end of a word_node
* @param word_node_t* curr_word pointer to word being added to
* @param glyph_t* added_glyph pointer to glyph being added
* @return word_node_t* - curr_word with added_glyph added
*/
word_node_t* add_glyph_to_word(word_node_t* curr_word, glyph_t* added_glyph) {
    word_node_t* new_word_node = malloc(sizeof(word_node_t));
    new_word_node->data = added_glyph; 
    new_word_node->next = NULL;
    if (added_glyph == NULL) return NULL;
    if (curr_word == NULL)
        return new_word_node;
    else {
        word_node_t* next_word_node = curr_word;
        while (next_word_node->next != NULL)
            next_word_node = next_word_node->next;
        next_word_node->next = new_word_node;
        return curr_word;
    }
}

word_node_t* free_word_old(word_node_t* this_word) {
    word_node_t* curr_node;
    while (this_word!= NULL) {
        curr_node = this_word;
        this_word = this_word->next;
        free(curr_node);
    }
    return NULL;
}


/**
 * @brief Returns the glyph in the script the corresponds to 
 * the first node in the linked list that includes curr_glyph
 * @param glyph_t* curr_glyph - Pointer to glyph to find next of
 * @param script_t* script - Script to look in
 * @return glyph_t* - pointer to first glyph in the linked list
 */
glyph_t* get_root(script_t* curr_script, glyph_t* curr_glyph) {
    int index = curr_script->letters[curr_script->index];
    return &(curr_script->glyphs[index]);
}

/**
 * @brief Returns a word linked list that corresponds to the given word
 * @param char* word -> word to turn into a glyph linked list
 * @param script_t* script - Script to look in
 * @return word_node_t* - pointer to first word_node in the linked list representing word
 */
word_node_t* word_to_glyph_word(script_t* curr_script, char* word) {
    word_node_t* curr_word = NULL;
    glyph_t* curr_glyph = NULL;
    for (int i = 0; i < strlen(word); i++) {
        curr_glyph = search_script(curr_script,get_bits_from_letter(word[i]));
        curr_word = add_glyph_to_word(curr_word,curr_glyph);
    }
    return curr_word;
}

/**
* @brief Returns the first glyph of the next letter
* @param the current script and whether it should be shuffled
* if we're cycling back to the beginning of the alphabet.
* @glyph_t* - Pointer to the first glyph in the next letter
*/
glyph_t* get_next_letter(script_t* script, bool should_shuffle) {
    // increment the index
    script->index++;

    // if we're out of letters, reset index and
    // shuffle as needed
    if (script->index >= script->num_letters) {
        script->index = 0;
        if (should_shuffle)
            shuffle(script->num_letters, script->letters);
    }

    // return the first glyph of the script->index'th letter
    return &(script->glyphs[script->letters[script->index]]);
}

/**
* @brief Returns the first glyph of the previous letter
* @param the current script
* @glyph_t* - Pointer to the first glyph in the next letter
* NB: this is not currently used.
*/
glyph_t* get_prev_letter(script_t* script) {
    // decrement the index
    script->index--;

    // if we're already at the beginning, reset index
    // to end. Don't shuffle since they presumably
    // want to repeat what they did.
    if (script->index < 0)
        script->index = script->num_letters - 1;
    
    // return the first glyph of the script->index'th letter
    return &(script->glyphs[script->letters[script->index]]);
}


/**
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
        default:
            return false;
    }
}

/**
* @brief Converts a digit glyph to an integer
* @param glyph_t* - Pointer to glyph to convet
* @return int - 0-9 for valid digit, -1 otherwise
*/
int get_digit(glyph_t* curr_glyph) {
    if (curr_glyph == NULL)
        return -1;
    if (!is_number(curr_glyph))
        return -1;

    switch(curr_glyph->pattern) {
        case NUM1: return 1;
        case NUM2: return 2;
        case NUM3: return 3;
        case NUM4: return 4;
        case NUM5: return 5;
        case NUM6: return 6;
        case NUM7: return 7;
        case NUM8: return 8;
        case NUM9: return 9;
        case NUM0: return 0;
        default:   return -1;
    }
}

/**
* @brief Checks to see if a glyph is BLANK
* @param glyph_t* - Pointer to check
* @return bool - true if BLANK, false otherwise
*/
bool is_blank(glyph_t* curr_glyph) {
    if (curr_glyph == NULL)
        return false;
    else
        return (curr_glyph->pattern == 0x00);
}
