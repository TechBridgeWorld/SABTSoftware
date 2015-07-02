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
#include <time.h>
#include "datastructures.h"
#include "globals.h"
#include "script_english.h"
// #include "audio.h"



/*******************
*  CELL FUNCTIONS  *
*******************/

/**
* Print the pattern of a cell in 0xnnnnnn format.
* @param Pointer to the cell
* @return void
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
		#ifdef DEBUGMODE
			printf("0x%s\n", binary);
		#else
			sprintf(dbgstr, "0x%s\n", binary);
			PRINTF(dbgstr);
		#endif
	}
	else {
		#ifdef DEBUGMODE
			printf("Blank cell\n");
		#else
			PRINTF("Blank cell\n");
		#endif
	}
}

/**
* Determine whether two cells are equal.
* @param Pointers to the two cells.
* @return Boolean; true if equal, false if
* unequal OR >=1 cell is null.
*/
bool cell_equals(cell_t* cell1, cell_t* cell2) {
	#ifdef DEBUGMODE
		printf("[Script] Cell 1: 0x%x, Cell 2: 0x%x\n", cell1->pattern, cell2->pattern);
	#else
		sprintf(dbgstr, "[Script] Cell 1: 0x%x\n\r[Script] Cell 2: 0x%x\n\r",
			cell1->pattern, cell2->pattern);
		PRINTF(dbgstr);
	#endif

	if (cell1 == NULL || cell2 == NULL)
		return false;
	else
		return (cell1->pattern == cell2->pattern);
}

/**
* Determine whether two glyphs are equal.
* @param Pointers to the two glyphs.
* @return Boolean; true if equal, false if
* unequal OR >=1 glyph is null.
* @warning Will be deprecated?
*/
bool glyph_equals(glyph_t* g1, glyph_t* g2) {
	#ifdef DEBUGMODE
		printf("[Script] Glyph 1: %s, Glyph 2: %s\n", g1->sound, g2->sound);
	#else
		sprintf(dbgstr, "[Script] Glyph 1: %s, Glyph 2: %s\n\r",
			g1->sound, g2->sound);
		PRINTF(dbgstr);
	#endif

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
*/
bool letter_equals(letter_t* letter1, letter_t* letter2) {
	#ifdef DEBUGMODE
		printf("[Script] Letter 1: %s (%d), Letter 2: %s (%d)\n", letter1->name, letter1->lang_enum,
				letter2->name, letter2->lang_enum);
	#else
		sprintf(dbgstr, "[Script] Letter 1: %s, Letter 2: %s\n\r",
			letter1->name, letter2->name);
		PRINTF(dbgstr);
	#endif

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

letter_t* get_eng_letter_by_char(char c){
	for (int i = 0; i < english_alphabet.num_letters; i++){
		if (c == english_alphabet.letters[i].name[0])
			return &english_alphabet.letters[i];
	}
	return NULL;
}

/**
* Print the name of a letter.
* @param Pointer to the letter.
* @return void
*/
void print_letter(letter_t* letter){
	printf("%s", letter->name);
}

/*******************
*  WORD FUNCTIONS  *
*******************/

/**
* Create a word (in English) from a character string. (Cuts off string
* at 9 characters -- max word length.)
* @param A string containing the word; the word struct to initialize.
* @return Void; function initializes word.
* @bug DOES NOT INITIALIZE THE LETTER ARRAY
* @bug cuts off strings > 10 letters long
*/
void parse_string_into_eng_word(char* string, word_t* word) {
	int length = (strlen(string) < MAX_WORD_LENGTH) ? strlen(string) : MAX_WORD_LENGTH;
	static letter_t letters_in_word[MAX_WORD_LENGTH];
	for (int i = 0; i < length; i++) {
		word->name[i] = string[i];
		letters_in_word[i] = *get_eng_letter_by_char(string[i]);
	}
	word->length_name = word->num_letters = length;
	word->curr_letter = word->curr_glyph = 0;
	word->lang_enum = ENGLISH;
	word->letters = letters_in_word;
}

/**
* Fill an array of cells with the glyphs representing a word.
* @param The word struct and an (uninitialized) array of cells.
* @return Void; function returns pointer in arr.
* @bug CURRENTLY BUGGY
*/
void word_to_cell_array(word_t* word, cell_t* arr){
	int array_index = 0;
	for (int i = 0; i < word->num_letters; i++){
		letter_t this_letter = word->letters[i];
//		printf("i = %d, letter = %c, cells %d\n", i, this_letter.name[0], this_letter.num_cells);
		for (int j = 0; j < this_letter.num_cells; j++) {
			arr[array_index] = this_letter.cells[j];
			array_index++;
		}
	}
}

/**
* Iterate through word, get the next cell in it.
* @param The word struct and an (uninitialized) cell pointer.
* @return Void; function returns pointer in next_cell.
*/
void get_next_cell_in_word(word_t* word, cell_t* next_cell) {
	letter_t this_letter = word->letters[word->curr_letter];
	// if this is a one-glyph letter, return its glyph & move to next letter
	if (this_letter.num_cells == 1) {
		*next_cell = this_letter.cells[0];
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

void print_letters_in_word(word_t* word) {
	for (int i = 0; i < word->num_letters; i++) {
		print_letter(&word->letters[i]);
	}
	printf("\n");
}

/**
* Returns a word's appropriate language prefix.
* @param A pointer to a word struct.
* @return A string with the language prefix.
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
	play_mp3(get_lang(word), word->name);
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
void speak_correct_letters(word_t* word){
	char* lang = get_lang(word);
	for (int i = 0; i < word->curr_letter; i++)
		play_mp3(lang, word->letters[i].name);
}
#endif


/***********************
*  WORDLIST FUNCTIONS  *
***********************/

/**
* Initialize a wordlist from an array of words. Can be used
* as helper function to strings_to_wordlist.
* @param Number of words; pointer to word array;
* pointer to the wordlist to initialize.
* @return Void; function initializes list.
* @BUG DOES NOT INITIALIZE THE ORDER
*/
void initialize_wordlist(word_t* words, int num_words, wordlist_t* list) {
	list->num_words = num_words;
	list->index = 0;
	list->words = words;
	shuffle(num_words, (list->order));
}

/**
* Initialize a wordlist from an array of strings.
* @param Pointer to an array of strings; number of strings
* in the array; pointer to the wordlist to initialize.
* @return Void; function initializes list.
*/
void strings_to_wordlist(char** strings, int num_strings, wordlist_t* list) {
	word_t words[num_strings];
	for (int i = 0; i < num_strings; i++)
		parse_string_into_eng_word(strings[i], &(words[i]));
	initialize_wordlist(words, num_strings, list);
}

void print_words_in_list(wordlist_t* wl) {
	for (int i = 0; i < wl->num_words; i++) {
		print_letters_in_word(&wl->words[wl->order[i]]);
	}
}

/**
 * Find a random number between i and j, inclusive of
 * i but not j. Helper function for shuffle.
 * @param Ints i and j, delineating the range
 * @return A random int between i and j-1 inclusive.
 * @warning THIS IS NOT RETURNING RANDOM NUMBERS
 * @warning MAYBE WORKS IF YOU CALL SRAND ONCE FROM MAIN?
 */
int random_between(int i, int j) {
	int range = j - i;
	return i + (rand() % range);
}

/**
* Perform Fisher-Yates shuffle on an int array of length len.
* @param A length len, an int array int_array.
* @return Void; function modifies int_array.
* @warning NOT FULLY TESTED -- randomness looks questionable
*/
void shuffle(int len, int* int_array) {
	int random_i, temp;
	for (int i = 0; i < len; i++) {
		random_i = random_between(i, len);
//		printf("i = %d, random i = %d\n", i, random_i);
		temp = int_array[i];
		int_array[i] = int_array[random_i];
		int_array[random_i] = temp;
	}
}

/**
* Sort an int array using selection sort.
* @param A length len, an int array int_array.
* @return Void; function modifies int_array.
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