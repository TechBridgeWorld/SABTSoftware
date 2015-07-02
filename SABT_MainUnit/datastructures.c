/**
* @file datastructures.c
* @brief Common functions involving glyphs, letters, etc.
* @author Vivek Nair & Marjorie Carlson (viveknair|marjorie@cmu.edu)
*/

#include <stdbool.h>
#include <stddef.h>
#include "datastructures.h"
#include "globals.h"

#ifdef DEBUGMODE
#else
	#include "audio.h"
#endif

// CELL FUNCTIONS
bool cell_equals(cell_t* cell1, cell_t* cell2) {
	#ifdef DEBUGMODE
		printf("[Script] Cell 1: 0x%x, Cell 2: 0x%x\n", cell1->pattern, cell2->pattern);
	#else
		sprintf(dbgstr, "[Script] Cell 1: 0x%x\n\r", cell1->pattern);
		PRINTF(dbgstr);
		sprintf(dbgstr, "[Script] Cell 2: 0x%x\n\r", cell1->pattern);
		PRINTF(dbgstr);
	#endif

	if (cell1 == NULL || cell2 == NULL)
		return false;
	else
		return (cell1->pattern == cell2->pattern);
}

/**
* @brief Compares 2 glyphs
* @param glyph_t* g1, g2 - The 2 glyphs to compare
* @bool true if cell patterns match, false otherwise

*/
bool glyph_equals(glyph_t* g1, glyph_t* g2) {
	#ifdef DEBUGMODE
		printf("[Script] Glyph 1: %s, Glyph 2: %s\n", g1->sound, g2->sound);
	#else
		sprintf(dbgst√r, "[Script] Glyph 1: %s\n\r", g1->sound);
		PRINTF(dbgstr);
		sprintf(dbgstr, "[Script] Glyph 2: %s\n\r", g2->sound);
		PRINTF(dbgstr);
	#endif

	if (g1 == NULL || g2 == NULL)
		return false;
	else
		return (g1->pattern == g2->pattern);
}

void print_cell_pattern(cell_t* cell){
	if (cell->pattern) {
		char pattern = cell->pattern;
		char binary[7];	
		int counter = 0;
		for (int i = 32; i > 0; i >>= 1) {
			binary[counter] =((pattern & i) == i) ? '1' : '0';
			counter++;
		}
		printf("0x%s\n", binary);
	}
	else
		printf("Blank cell.\n");
}

// LETTER FUNCTIONS

bool letter_equals(letter_t* letter1, letter_t* letter2) {
	#ifdef DEBUGMODE
		printf("[Script] Letter 1: %s (%d), Letter 2: %s (%d)\n", letter1->name, letter1->lang_enum,
				letter2->name, letter2->lang_enum);
	#else
		sprintf(dbgstr, "[Script] Letter 1: %s\n\r", letter1->name);
		PRINTF(dbgstr);
		sprintf(dbgstr, "[Script] Letter 2: %s\n\r", letter2->name);
		PRINTF(dbgstr);
	#endif

	// not equal if one doesn't exist; if of different langs; if of different lengths
	if ((letter1 == NULL || letter2 == NULL)
		| (letter1->lang_enum != letter2->lang_enum)
		| (letter1->num_cells != letter2->num_cells)) {
		return false;
	}
	else { // iterate through cells; if any are different, set return value to false
		bool are_the_same = true;
		for (int i = 0; i < letter1->num_cells; i++) {
			if (!(cell_equals(&(letter1->cells[i]), &(letter2->cells[i]))))
				are_the_same = false;
		}
		return are_the_same;
	}
}

// WORD FUNCTIONS

void parse_string_into_eng_word(char* string, word_t* word) {
	// what to do if string too long
	for (int i = 0; i < strlen(string); i++) {
		word->name[i] = string[i]; // how do I get the ltters in there?
	}
	word->length_name = word->num_letters = strlen(string);
	word->lang_enum = ENGLISH;
	word->curr_letter = 0;
	word->curr_glyph = 0;
}

void word_to_cell_array(word_t* word, cell_t* arr){
	int array_index = 0;
	for (int i = 0; i < word->num_letters; i++){
		letter_t this_letter = word->letters[i];
		for (int j = 0; j < this_letter.num_cells; j++) {
			arr[array_index] = this_letter.cells[j];
			array_index++;
		}
	}
}

void get_next_cell_in_word(word_t* word, cell_t* next_cell) {
	letter_t this_letter = word->letters[word->curr_letter];
	// if this is a one-glyph letter, return its glyph & move to next letter
	if (this_letter.num_cells == 1) {
		next_cell = &(this_letter.cells[0]);
		word->curr_letter++;
	}
	else { // if >1 glyph, get current glyph and increment
		next_cell = &(this_letter.cells[word->curr_glyph]);
		word->curr_glyph++;
		if (word->curr_glyph == this_letter.num_cells) { // last glyph in letter
			word->curr_glyph = 0; // reset to first glyph in next letter
			word->curr_letter++;
		}
	}
}

char* get_lang(word_t* word){
	switch (word->lang_enum) {
		case ENGLISH:
			return "ENG_";
			break;
		case HINDI:
			return "HIN_";
			break;
		case KANNADA:
			return "KAN_";
			break;
		default:
			return "SYS_";
			break;
	}
}

#ifdef DEBUGMODE
#else

void speak_word(word_t* word) {
	play_mp3(get_lang(word), word->sound);
}

void speak_letters_in_word(word_t* word){
	char* lang = get_lang(word);
	for (int i = 0; i < word->num_letters; i++)
		play_mp3(lang, word->letters[i].name);
}

void speak_correct_letters(word_t* word){
	char* lang = get_lang(word);
	for (int i = 0; i < word->curr_letter; i++)
		play_mp3(lang, word->letters[i].name);
}
#endif


// WORDLIST FUNCTIONS

void initialize_wordlist(int length, word_t* words, wordlist_t* list) {
	list->length = length;
	list->index = 0;
	list->words = words;
//	shuffle(length, (list->order));
}

void strings_to_wordlist(char** strings, int num_strings, wordlist_t list) {
	word_t words[num_strings];
	for (int i = 0; i < num_strings; i++)
		parse_string_into_eng_word(strings[i], &(words[i]));
	initialize_wordlist(num_strings, words, &list);
}

/**
 * @brief Returns an integer between i and j-1
 * @param: i and j
 * return: a random number between them
 * (including i, not including j)
 */
int random_between(int i, int j) {
	int range = j - i;
	return i + (rand() % range);
}

/**
* @brief Performs a Fisher-Yates shuffle on the
* an int array of length len.
* @param a length, an int array
* @return void
*/
void shuffle(int len, int* int_array) {
	int random_i, temp;
	for (int i = 0; i < len; i++) {
		random_i = random_between(i, len);
		temp = int_array[i];
		int_array[i] = int_array[random_i];
		int_array[random_i] = temp;
	}
}

/**
* @brief Uses selection sort to unshuffle
* an int array. Undoes shuffle.
* @param a length, an int array
* @return void
*/
void unshuffle(int len, int* int_array) {
	for (int i = 0; i < len; i++) {
		int min = i;
		for (int j = i+1; j < len; j++) {
			if (int_array[j] < int_array[min])
				min = j;
		}
		if (i != min) {
			int temp = int_array[i];
			int_array[i] = int_array[min];
			int_array[min] = temp;
		}
	}
}