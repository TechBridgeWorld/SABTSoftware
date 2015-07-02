#include <stdio.h>
#include "datastructures.h"
#include "script_english.h"
#include "script_hindi.h"

int main() {
	cell_t a_cell, b_cell, c_cell, hin_a_cell, all_cell, blank_cell, null_cell;
	a_cell.pattern = DOTS1;
	b_cell.pattern = DOTS12;
	hin_a_cell.pattern = DOTS1;
	all_cell.pattern = DOTS123456;
	blank_cell.pattern = DOTS0;

	// test of print_cell_pattern
	printf("0x000001 =? ");
	print_cell_pattern(&a_cell);
	printf("0x000011 =? ");
	print_cell_pattern(&b_cell);
	printf("0x111111 =? ");
	print_cell_pattern(&all_cell);
	printf("Blank cell =? ");
	print_cell_pattern(&blank_cell);
	printf("[?] =? ");
	print_cell_pattern(&null_cell);
	printf("Compare above to test print_cell_pattern.\n\n");

	// test of cell_equals
	bool same1 = cell_equals(&a_cell, &a_cell);
	bool diff1 = !cell_equals(&a_cell, &b_cell);
	bool cognate1 = cell_equals(&a_cell, &hin_a_cell);
	bool empty1 = !cell_equals(&a_cell, &blank_cell);
	bool bothempty1 = cell_equals(&blank_cell, &blank_cell);
	bool notinitialized1 = !cell_equals(&a_cell, &null_cell);
	printf("Cell_equals %s.\n\n", (same1 && diff1 && cognate1 &&
			empty1 && bothempty1 && notinitialized1) ? "works" : "IS BROKEN");

	// test of letter_equals

	letter_t blank_letter = {" ", UNIVERSAL, &blank_cell, 1};
	letter_t null_letter;

	bool same2 = letter_equals(&eng_a, &eng_a);
	bool diff2 = !letter_equals(&eng_a, &eng_b);
	bool cognate2 = !letter_equals(&eng_a, &hindi_a);
	bool diffnum = !letter_equals(&hindi_a, &hindi_shra);
	bool empty2 = !letter_equals(&eng_a, &blank_letter);
	bool notinitialized2 = !letter_equals(&eng_a, &null_letter);
	printf("Letter_equals %s.\n\n", (same2 && diff2 && cognate2 && diffnum && empty2 && notinitialized2) ? "works" : "IS BROKEN");

	// test of get_eng_letter_by_char and print_letter
	letter_t* foo = get_eng_letter_by_char('z');
	print_letter(foo);
	printf("\nGet_eng_letter_by_char and print_letter work if the above reads 'z.'\n\n");

	// test of print_letters_in_word;
	letter_t goat_letters[4] = {eng_g, eng_o, eng_a, eng_t};
	word_t goat = {"goat", 4, ENGLISH, goat_letters, 4, 0, 0};
	print_letters_in_word(&goat);
	printf("Print_letters_in_word works if the above reads 'goat.'\n\n");

	// test of word_to_cell_array
	cell_t array[4];
	word_to_cell_array(&goat, array);
	bool cell_array_correct = true;
	for (int i = 0; i < 4; i++) {
		if (!cell_equals(&array[i], &goat_letters[i].cells[0]))
			cell_array_correct = false;
	}
	printf("Word_to_cell_array %s.\n\n", word_to_cell_array ? "works" : "IS BROKEN");

	// test of get_next_cell_in_word
	cell_t next_cell;
	bool next_cell_correct = true;
	for (int i = 0; i < 4; i++) {
		get_next_cell_in_word(&goat, &next_cell);
		if (!cell_equals(&next_cell, &goat_letters[i].cells[0]))
			next_cell_correct = false;
	}
	printf("Get_next_cell_in_word %s.\n\n", next_cell_correct ? "works" : "IS BROKEN");

	// test of get_lang
	printf("%s\n", get_lang(&goat));
	printf("Get_lang works if the above reads 'ENG_.'\n\n");

	// test of parse_string_into_eng_word
	word_t horse;
	parse_string_into_eng_word("horse", &horse);
	bool horse_stats_right = (horse.length_name == 5) && (horse.num_letters == 5) && (horse.lang_enum == ENGLISH);
	print_letters_in_word(&horse);
	printf("Parse_string_into_eng_word %s\n\n", horse_stats_right ? "works if the above says 'horse.'" : "IS BROKEN.");

	// test of initialize_wordlist
	word_t animals[2] = {goat, horse};
	wordlist_t wl;
	int order[2] = {0, 1};
	wl.order = order;
	initialize_wordlist(animals, 2, &wl);
	print_words_in_list(&wl);
	printf("Initialize_wordlist %s.\n\n", (wl.num_words == 2) ? "works if the above says 'goat horse' in any order" : "IS BROKEN");


/*	// test of strings_to_wordlist
	char* animal_strings[2] = {"goat", "horse"};
	wordlist_t wl2;
	strings_to_wordlist(animal_strings, 2, &wl2);
	print_words_in_list(&wl2); */

	// test of random_between
//	printf("%d, %d, %d\n", random_between(0,1), random_between(5,10), random_between(0,1000));

	// test of shuffle
	int bar[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	shuffle(10, bar);
	for (int i = 0; i < 10; i++)
		printf("%d, ", bar[i]);
	printf("\n");
	shuffle(10, bar);
	for (int i = 0; i < 10; i++)
		printf("%d, ", bar[i]);
	printf("\n");
	shuffle(10, bar);
	for (int i = 0; i < 10; i++)
		printf("%d, ", bar[i]);
	printf("\n");
	printf("Shuffle sorta-works if you see three randomly ordered arrays above.\n");
	printf("Note: it relies on random_between, which seems not very random.\n");

	unshuffle(10, bar);
	bool unshuffle_worked = true;
	for (int i = 0; i < 10; i++)
		if (bar[i] != i)
			unshuffle_worked = false;
	printf("Unshuffle %s.\n\n", unshuffle_worked ? "works" : "IS BROKEN");



	// test of unshuffle

}