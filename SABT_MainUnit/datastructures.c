/**
* @file datastructures.c
* @author Marjorie Carlson (marjorie@cmu.edu)
* @date Summer 2015
* @brief Basic functions for creating and accessing cells, letters & words
* @warning THE BELOW FUNCTIONS HAVE ONLY BEEN TESTED ON ENGLISH LETTERS/WORDS.
* They still need to be tested on multi-cell letters (Hindi, Kannada).
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "datastructures.h"
#include "globals.h"
#include "script_english.h"
#include "audio.h"

/*******************
*  CELL FUNCTIONS  *
*******************/

/**
* Print the pattern of a cell in 0bnnnnnn format.
* @param Pointer to the cell
* @return Void; prints output.
* @remark Tested. (Language-agnostic.)
*/
void print_cell_pattern(cell_t* cell){
	if (cell->pattern) {
		char pattern = cell->pattern;
		char binary[7];	
		int counter = 0;
		for (int i = 32; i > 0; i >>= 1) {
			binary[counter] =((pattern & i) == i) ? '1' : '0';
			counter++;
		}
		sprintf(dbgstr, "0b%s\n", binary);
		PRINTF(dbgstr);
	}
	else {
			PRINTF("Blank cell\n");
	}
}

/**
* Determine whether two cells are equal.
* @param Pointers to the two cells.
* @return Boolean; true if equal, false if unequal OR >=1 cell is null.
* @remark Tested. (Language-agnostic.)
*/
bool cell_equals(cell_t* cell1, cell_t* cell2) {
	sprintf(dbgstr, "[Cell_equals script] Cell 1: 0x%x\n\r[Script] Cell 2: 0x%x\n\r",
		cell1->pattern, cell2->pattern);
	PRINTF(dbgstr);

	if (cell1 == NULL || cell2 == NULL)
		return false;
	else
		return (cell1->pattern == cell2->pattern);
}

/**
* Determine whether two glyphs are equal.
* @param Pointers to the two glyphs.
* @return Boolean; true if equal, false if unequal OR >=1 glyph is null.
* @warning Will be deprecated?
*/
bool glyph_equals(glyph_t* g1, glyph_t* g2) {
	sprintf(dbgstr, "[Glyph_equals] Glyph 1: %s, Glyph 2: %s\n\r",
		g1->sound, g2->sound);
	PRINTF(dbgstr);

	if (g1 == NULL || g2 == NULL)
		return false;
	else
		return (g1->pattern == g2->pattern);
}


/*******************
* LETTER FUNCTIONS *
*******************/
/**
* Determine whether two letters are equal (including in same language).
* @param Pointers to the two letters.
* @return Boolean; true if equal, false if unequal (which includes
* one or more being null or the letters being the same sound in different
* languages.)
* @remark Tested in English & lightly in Hindi.
*/
bool letter_equals(letter_t* letter1, letter_t* letter2) {
	sprintf(dbgstr, "[Letter_equals] Letter 1: %s, Letter 2: %s\n\r",
		letter1->name, letter2->name);
	PRINTF(dbgstr);

	// not equal if one doesn't exist; if of different langs; if of different lengths
	if ((letter1 == NULL || letter2 == NULL)
		| (letter1->lang_enum != letter2->lang_enum)
		| (letter1->num_cells != letter2->num_cells)) {
		return false;
	}
	else { // iterate through cells; if any are different, return false
		for (int i = 0; i < letter1->num_cells; i++) {
			if (!(cell_equals(&(letter1->cells[i]), &(letter2->cells[i]))))
				return false;
		}
		return true;
	}
}

/**
* Given an English char (e.g. 'a'), retrieve the English letter
* whose name begins with that char. Assumes names are one char each
* and thus that only one letter corresponds to each char.
* @param A char representing an English letter. NULL if none exist
* @return A pointer to a letter; null if no match is found.
* @remark Tested in English. WILL NOT WORK IN OTHER LANGUAGES.
* @remark Turns all letters into lower-case.
*/
letter_t* get_eng_letter_by_char(char c){
	char this_char;
	for (int i = 0; i < english_alphabet.num_letters; i++){
		this_char = (c >= 'A' && c <= 'Z') ? (c - 'A' + 'a') : c;
		if (this_char < 'a' || this_char > 'z') {
			sprintf(dbgstr, "GET_ENG_LETTER_BY_CHAR FAILED: '%c' IS NOT A LETTER.\n", c);
			PRINTF(dbgstr);
			return NULL;
		}
		else if (this_char == english_alphabet.letters[i].name[0])
			return &english_alphabet.letters[i];
	}
	sprintf(dbgstr, "GET_ENG_LETTER_BY_CHAR FAILED: '%c' DID NOT MATCH ANY LETTERS.\n", c);
	PRINTF(dbgstr);
	return NULL;
}

/**
* Print the name of a letter.
* @param Pointer to the letter.
* @return void
* remark Tested in English and Hindi.
*/
void print_letter(letter_t* letter){
	sprintf(dbgstr, "%s", letter->name);
	PRINTF(dbgstr);
}

/*******************
*  WORD FUNCTIONS  *
*******************/

/**
* Create a new word in English. (To be deprecated once create-from-string works.)
* @param The word as a string and letter array, number of letters, and the word
* struct to initialize.
* @return void
* @todo What to do if string is too long? Need to rename mp3.
* remark Tested in English
*/
void initialize_english_word(char* string, letter_t* letter_array, int num_letters, word_t* word) {
	strcpy(word->name, string);
	word->letters = letter_array;
	word->length_name = word->num_letters = num_letters;
	word->lang_enum = ENGLISH;
	word->curr_letter = 0;
	word->curr_glyph = -1;
}

/**
* Create a word (in English) from a character string. Assumes one cell per
* letter, but does check for capitalization of first word.
* @param A string containing the word; the word struct to initialize.
* @return 1 if function initialized word; 0 if it failed.
* @remark Tested thoroughly.
*/
int parse_string_into_eng_word(char* string, word_t* word) {
	int num_cells = strlen(string);
	bool is_capitalized = false;

	// if the word is capitalized, leave space for cap character
	if (string[0] >= 'A' && string[0] <= 'Z') {
		is_capitalized = true;
		num_cells++;
	}

	// if the word is too long, return failure
	if (num_cells > MAX_WORD_LENGTH) {
		sprintf(dbgstr, "PARSE_STRING FAILED: '%s' IS TOO LONG\n\r", string);
		PRINTF(dbgstr);
//		PRINTF("PARSE_STRING FAILED: WORDS MUST BE <=20 CHARS\n");
		return 0;
	}

	// create the word
	strcpy(word->name, string);
	letter_t* letters_in_word;
	letters_in_word = (letter_t*) malloc(sizeof(letter_t) * num_cells);
	if (letters_in_word == 0) // if malloc fails, return failure
		return 0;

	// iterate through both string and letter_t array. In most cases
	// the ith element of the string is copied into the ith element of
	// the letter array. However, if the word is capitalized, the 0th
	// element of the letter array is the capital letter, and then
	// the ith letter of the string goes into the i+1th index of the array.
	int letter_index = 0;
	if (is_capitalized) {
		letters_in_word[0] = eng_capital;
		letter_index = 1;
	}


	for (int string_index = 0; string_index < strlen(string); string_index++) {
			letter_t* this_letter = get_eng_letter_by_char(string[string_index]);
			if (this_letter == NULL) { // if failed to get a letter, return failure
				sprintf(dbgstr, "PARSE_STRING_INTO_ENG_WORD FAILED; FAILED TO PARSE '%s'\n\r", string);
				PRINTF(dbgstr);
				return 0;
			}
			letters_in_word[letter_index] = *this_letter;
			letter_index++;
		}

	if (num_cells == 0) {
		PRINTF("PARSE_STRING_INTO_ENG_WORD FAILED: NO LETTERS WERE FOUND.\n\r");
		return 0;
	}

	word->letters = letters_in_word;
	word->length_name = strlen(string); // length of English representation of word
	word->num_letters = num_cells;      // length of Braille representation of word
	word->curr_letter = 0;
	word->curr_glyph = -1;
	word->lang_enum = ENGLISH;
	return 1;
}

/**
* Fill an array of cells with the glyphs representing a word.
* @param The word struct and an (uninitialized) array of cells.
* @return Void; function returns pointer in arr.
* @remark Tested lightly in English and Hindi.
*/
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

/**
* Each word contains curr_letter and curr_glyph indices to
* represent the cell we're about to access. Increment_word_index
* increments curr_glyph, if we're not already at the last cell
* in the curr_letter, or curr_letter if we are.
* @param The word struct.
* @return Void; function modifies word.
* @remark Tested lightly in English and Hindi.
*/
void increment_word_index(word_t* word) {
	letter_t this_letter = word->letters[word->curr_letter];
	if (word->curr_glyph < this_letter.num_cells - 1)
		word->curr_glyph++;
	else {
		word->curr_letter++;
		word->curr_glyph = 0;
	}
}

void decrement_word_index(word_t* word) {
	// if you're at the first cell, reset to letter 0, glyph -1
	if (word->curr_letter == 0 && word->curr_glyph == 0)
		word->curr_glyph = -1;
	// if you're partway through a letter, decrement curr_glyph
	else if (word->curr_glyph > 0)
		word->curr_glyph--;
	// otherwise, decrement curr_letter
	else {
		word->curr_letter--;
		letter_t prev_letter = word->letters[word->curr_letter];
		word->curr_glyph = prev_letter.num_cells - 1;
	}
}

/**
* Iterate through word, get the next cell in it.
* @param The word struct and an (uninitialized) cell pointer.
* @return Void; function returns pointer in next_cell.
* @remark Tested lightly in English.
*/
void get_next_cell_in_word(word_t* word, cell_t* next_cell) {
	increment_word_index(word);
	letter_t this_letter = word->letters[word->curr_letter];
	*next_cell = this_letter.cells[word->curr_glyph];
	}

/**
* Print a word, not by printing its name but by printing the name
* of each letter in the word. (Used for testing & by print_words_in_list.)
* @param The word struct.
* @return Void.
* @remark Tested in English and Hindi.
*/
void print_word(word_t* word) {
	if (word->letters == NULL) {
		sprintf(dbgstr, "PRINT_WORD FAILED: '%s' CONTAINS NO LETTERS\n", word->name);
		PRINTF(dbgstr);
		return;
	}
	sprintf(dbgstr, "%s (spelled: ", word->name);
	PRINTF(dbgstr);
	for (int i = 0; i < word->num_letters; i++) {
		print_letter(&word->letters[i]);
		if (i < (word->num_letters - 1))
			PRINTF("-");
	}
	sprintf(dbgstr, (") [%d]\n\r"), word->num_letters);
	PRINTF(dbgstr);
}

/**
* Returns a word's appropriate language prefix.
* @param A pointer to a word struct.
* @return A string with the language prefix.
* @remark Tested in English.
*/
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

/**
* Play the mp3 associated with a word.
* @param Pointer to the word.
* @return Void.
* @warning Untested
*/
void speak_word(word_t* word) {
	play_mp3("V_", word->name);
}

/**
* Play the mp3s of each letter in a word.
* @param Pointer to the word.
* @return Void.
* @warning Untested
*/
void speak_letters_in_word(word_t* word){
	char* lang = get_lang(word);
	for (int i = 0; i < word->num_letters; i++)
		play_mp3(lang, word->letters[i].name);
}

/**
* Play the mp3s of each of the letters in a word
* before the current letter. Used to remind
* a user of the letters they've inputted so far.
* @param Pointer to the word.
* @return Void.
* @warning Untested
*/
void speak_letters_so_far(word_t* word){
	char* lang = get_lang(word);
	for (int i = 0; i <= word->curr_letter; i++)
		play_mp3(lang, word->letters[i].name);
}
#endif

// @todo: void free_word(word_t* word);



/***********************
*  WORDLIST FUNCTIONS  *
***********************/

/**
* Initialize a wordlist from an array of words. Can be used
* as helper function to strings_to_wordlist. If the array is
* longer than MAX_WORDLIST_LENGTH, only the first
* MAX_WORDLIST_LENGTH elements will be indexed in "order".
* @param Number of words; pointer to word array;
* pointer to the wordlist to initialize.
* @return Void; function initializes list.
* @remark Tested lightly in English.
* @BUG Sometimes buggy when used several times in a row.
*/
void initialize_wordlist(word_t* words, int num_words, wordlist_t* list) {
	if (num_words == 0) {
		PRINTF("No words. Wordlist uninitialized.\n\r");
		return;
	}
	list->num_words = (num_words < MAX_WORDLIST_LENGTH) ? num_words : MAX_WORDLIST_LENGTH;
	int* order = (int*) malloc(num_words*sizeof(int));
	list->index = 0;
	list->words = words;
	list->order = order;
	for (int i = 0; i < MAX_WORDLIST_LENGTH; i++) {
		if (i < list->num_words) {
			list->order[i] = i;
		}
		else {
			list->order[i] = -1;
		}
	}
	shuffle(num_words, list->order);
	sprintf(dbgstr, "Wordlist initialized. Num_words = %d.\n\r", num_words);
	PRINTF(dbgstr);
}

/**
* Initialize a wordlist from an array of strings, or a
* random subset thereof.
* @param Pointer to an array of strings; number of strings
* in the array; pointer to the wordlist to initialize.
* @return Void; function initializes list.
* @BUG: Data gets corrupted if num_strings is incorrect.
*/
void strings_to_wordlist(char** strings, int num_strings, wordlist_t* list) {
	static word_t* words;
	int str_index, word_index; // will iterate separtely through string array and word array
	// iterate through strings and parse them into words. If there are too many strings,
	// randomly pick MAX_WORDLIST_LENGTH of them. If any string fails to parse into
	// a word, shrink the wordlist, reset the word_index iterator, and keep going.

	if (num_strings <= MAX_WORDLIST_LENGTH) {
		str_index = word_index = 0;
		words = (word_t*) malloc(sizeof(word_t) * num_strings);
		if (words == 0) {
			PRINTF("MALLOC FAILED!!!\n\r");
			exit(0);
		}
		// iterate through strings; parse into words; increment index into word array only when parsing succeeds
		for (str_index = 0; str_index < num_strings; str_index++) {
			sprintf(dbgstr, "word_index: %d, str_index: %d, string %s\n\r", word_index, str_index, strings[str_index]);
			PRINTF(dbgstr);
			if (parse_string_into_eng_word(strings[str_index], &(words[word_index])))
				word_index++;
		}
	}

	else { // too many strings. Shuffle the words & pick the first MAX_WORDLIST_LENGTH.
		str_index = word_index = 0;
		words = (word_t*) malloc(sizeof(word_t) * MAX_WORDLIST_LENGTH);
		if (words == 0) {
			PRINTF("MALLOC FAILED!!!\n\r");
			exit(0);
		}
		int indices[num_strings];
		for (int i = 0; i < num_strings; i++) {
			indices[i] = i;
		}
		shuffle(num_strings, indices);

		// get MAX_WORDLIST_LENGTH of the strings, in random order
		// iterate through words; parse strings into it; increment string index each time,
		// but decrement word_index if parsing fails so it'll increment back to same place
		// in the next loop
		for (word_index = 0; word_index < MAX_WORDLIST_LENGTH; word_index++) {
			sprintf(dbgstr, "word_index: %d, str_index: %d, indices: %d, string %s\n\r", word_index, str_index, indices[str_index], strings[indices[str_index]]);
			PRINTF(dbgstr);
			if (str_index == MAX_WORDLIST_LENGTH) {// stop if we're out of strings -- could happen if
				PRINTF("Out of strings!\n\r");
				break;                            // lots of strings are unparseable
			}

			if (!parse_string_into_eng_word(strings[indices[str_index]], &(words[word_index]))) {
				word_index--;
			}
			str_index++;
		}
	}
	initialize_wordlist(words, word_index, list); //word_index = length of list
}

/**
* Iterates through wordlist, prints each word in it, in
* the order specified by the "order" array.
* @param Pointer to the wordlist.
* @return Void.
* @remark Tested in English.
*/
void print_words_in_list(wordlist_t* wl) {
	for (int i = 0; i < wl->num_words; i++) {
		print_word(&wl->words[wl->order[i]]);
	}
}

/**
* Iterate through wordlist, get the next word in it.
* @param The wordlist struct and an (uninitialized) word pointer.
* @return Void; function returns pointer in next_word.
* @remark Tested lightly in English.
*/
void get_next_word_in_wordlist(wordlist_t* wl, word_t** next_word) {
	if (wl->index == 0) // if at beginning of list, reshuffle
		shuffle(wl->num_words, wl->order);

	int randomized_index = wl->order[wl->index];
	*next_word = &(wl->words[randomized_index]); // added indirection to iterate in randomized order

	wl->index = (wl->index + 1) % wl->num_words; // increment index, reset if necessary
}

// @todo: void free_wordlist(wordlist_t* wl);

/**
 * Find a random number between i and j, inclusive of
 * i but not j. Helper function for shuffle.
 * @param Ints i and j, delineating the range
 * @return A random int between i and j-1 inclusive.
 */
int random_between(int i, int j) {
	int range = j - i;
	int retval = i + (rand() % range);
	return retval;
}

/**
* Perform Fisher-Yates shuffle on an int array of length len.
* @param A length len, an int array int_array.
* @return Void; function modifies int_array.
* @remark Tested lightly. Needs srand to be called first.
* @remark Consider implementing a "shuffle wordlist"
* function.
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
* Sort an int array using selection sort.
* @param A length len, an int array int_array.
* @return Void; function modifies int_array.
* @remark Tested.
* @remark Consider implementing an "unshuffle
* wordlist" function.
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
/*
// return: 0 if done with word, 1 if mistake
// needs access to globals: current_word; how does
// it get user's cell?
// num_mistakes; lang_fileset
char check_input(cell_t* cell) {
	cell_t goal_cell;
	get_next_cell_in_word(chosen_word, &goal);
	if (cell_equals(&goal_cell, cell)) {
		play_mp3(lang_fileset, "GOOD");
		num_mistakes = 0;

		// done with letter
		if 
	}

	// cases:
	// cell correct, not done with letter
	// letter correct
	// letter incorrect, out of mistakes
	// letter incorrect, not out of mistakes
}

			if (glyph_equals(curr_glyph, user_glyph)) {
				if (curr_glyph -> next == NULL) {
					incorrect_tries = 0;
					sprintf(dbgstr, "[%s] User answered correctly\n\r", mode_name);
					PRINTF(dbgstr);
					play_mp3(LANG_FILESET, MP3_CORRECT);
					play_mp3(SYS_FILESET, MP3_TADA);
					next_state = STATE_GENQUES;
				}
				else {
					curr_glyph = curr_glyph->next;
					play_mp3(LANG_FILESET, MP3_NEXT_CELL);
					
					if (submode == SUBMODE_LEARN)
						play_dot_sequence(curr_glyph);
					else
						play_glyph(curr_glyph);
					next_state = STATE_INPUT;
				}
			}
			else {
				incorrect_tries++;
				sprintf(dbgstr, "[%s] User answered incorrectly\n\r", mode_name);;
				PRINTF(dbgstr);			
				play_mp3(LANG_FILESET, MP3_INCORRECT);
				play_mp3(LANG_FILESET, MP3_TRY_AGAIN);
				curr_glyph = get_root(SCRIPT_ADDRESS, curr_glyph);	
				next_state = STATE_PROMPT;
				if (incorrect_tries >= MAX_INCORRECT_TRIES) {
					play_glyph(curr_glyph);
					play_mp3(MODE_FILESET, MP3_FOR_X_PRESS_DOTS);
					play_dot_sequence(curr_glyph);
					next_state = STATE_INPUT;
				}
			}
 */