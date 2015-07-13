#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "datastructures.h"
#include "script_english.h"
#include "script_hindi.h"

int main() {
	time_t t;
	srand((unsigned) time(&t));
	cell_t a_cell, b_cell, c_cell, hin_a_cell, all_cell, blank_cell, null_cell;
	a_cell.pattern = DOTS1;
	b_cell.pattern = DOTS12;
	hin_a_cell.pattern = DOTS1;
	all_cell.pattern = DOTS123456;
	blank_cell.pattern = DOTS0;

	/* test of print_cell_pattern
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
	printf("Compare above to test print_cell_pattern.\n\n"); */

	// test of cell_equals
/*	bool same1 = cell_equals(&a_cell, &a_cell);
	bool diff1 = !cell_equals(&a_cell, &b_cell);
	bool cognate1 = cell_equals(&a_cell, &hin_a_cell);
	bool empty1 = !cell_equals(&a_cell, &blank_cell);
	bool bothempty1 = cell_equals(&blank_cell, &blank_cell);
	bool notinitialized1 = !cell_equals(&a_cell, &null_cell);
	printf("Cell_equals %s.\n\n", (same1 && diff1 && cognate1 &&
			empty1 && bothempty1 && notinitialized1) ? "works" : "IS BROKEN"); */


	// test of letter_equals
	letter_t blank_letter = {" ", UNIVERSAL, &blank_cell, 1};
	letter_t null_letter;

/*	bool same2 = letter_equals(&eng_a, &eng_a);
	bool samehin = letter_equals(&hindi_shra, &hindi_shra);
	bool diff2 = !letter_equals(&eng_a, &eng_b);
	bool cognate2 = !letter_equals(&eng_a, &hindi_a);
	bool diffnum = !letter_equals(&hindi_a, &hindi_shra);
	bool empty2 = !letter_equals(&eng_a, &blank_letter);
	bool notinitialized2 = !letter_equals(&eng_a, &null_letter);
	printf("Letter_equals %s.\n\n", (same2 && samehin && diff2 && cognate2 && diffnum && empty2 && notinitialized2) ? "works" : "IS BROKEN");

	// test of get_eng_letter_by_char and print_letter
	letter_t* foo = get_eng_letter_by_char('z');
	letter_t* bar = get_eng_letter_by_char('a');
	print_letter(foo);
	print_letter(bar);
	print_letter(foo);
	printf(" ");
	print_letter(&hindi_a);
	printf(" ");
	print_letter(&hindi_shra);

	printf("\nGet_eng_letter_by_char and print_letter work if the above reads 'zaz a shra.'\n\n"); */

	// test of parse_string_into_eng_word
/*	word_t goat, ox, chicken, myname, blank, notchars, funnycap, quitelong, verylong;
	parse_string_into_eng_word("goat", &goat);
	parse_string_into_eng_word("ox", &ox);
	parse_string_into_eng_word("chicken", &chicken);
	parse_string_into_eng_word("Marjorie", &myname);
	parse_string_into_eng_word("", &blank);
	parse_string_into_eng_word(":-)", &notchars);
	parse_string_into_eng_word("pH", &funnycap);
	parse_string_into_eng_word("abcdefghijklmnopqrst", &quitelong);
	parse_string_into_eng_word("abcdefghijklmnopqrstu", &verylong); */


	// test of print_word;
	letter_t sri_letters[2] = {hindi_shra, hindi_i};
	word_t sri = {"sri", 2,  sri_letters, 2, HINDI, 0, -1};
/*	print_word(&goat);
	print_word(&ox);
	print_word(&chicken);
	print_word(&goat);
	print_word(&sri);
	print_word(&myname);
	print_word(&blank);
	print_word(&notchars);
	print_word(&funnycap);
	print_word(&quitelong);
	print_word(&verylong); */

/*
	//test of word_to_cell_array
	cell_t goat_array[4];
	cell_t sri_array[4];
	cell_t me_array[9];

	word_to_cell_array(&goat, goat_array);
	word_to_cell_array(&sri, sri_array);
	word_to_cell_array(&myname, me_array);

	for (int i = 0; i < 4; i++){
		print_cell_pattern(&goat_array[i]);
	}
	for (int i = 0; i < 4; i++){
		print_cell_pattern(&sri_array[i]);
	}
	for (int i = 0; i < 9; i++){
		print_cell_pattern(&me_array[i]);
	}


	// test of get_next_cell_in_word
	cell_t next_cell;
	for (int i = 0; i < 4; i++) {
		get_next_cell_in_word(&goat, &next_cell);
		print_cell_pattern(&next_cell);
	}
	printf("\n");
	for (int i = 0; i < 4; i++) {
		get_next_cell_in_word(&sri, &next_cell);
		print_cell_pattern(&next_cell);
	}
	printf("\n");
	for (int i = 0; i < 9; i++) {
		get_next_cell_in_word(&myname, &next_cell);
		print_cell_pattern(&next_cell);
	}

	// test of get_lang
	printf("%s\n", get_lang(&goat));
	printf("Get_lang works if the above reads 'ENG_.'\n\n");

	// test of initialize_wordlist
	word_t animals[3] = {goat, ox, chicken};
	word_t* no_animals;
	wordlist_t wl;
	initialize_wordlist(animals, ARRAYLEN(animals), &wl);
	print_words_in_list(&wl);
	printf("Initialize_wordlist & print_words_in_list %s.\n\n", (wl.num_words == 3) ? "work if the above says goat, ox & chicken in any order" : "ARE BROKEN");

	// test of get_next_word_in_wordlist
	word_t* curr_word;
	for (int i = 0; i < 6; i++) {
		if (i % 3 == 0)
			printf("\n");
		get_next_word_in_wordlist(&wl, &curr_word);
		printf("%s\n", curr_word->name);
	}
	printf("get_next_word_in_wordlist works if the three animals now appear in ~3 orders.\n\n"); */

	// test of strings_to_wordlist
	char* small[1] = {"hi"};
	char* animal_strings[3] = {"dog", "cat", "mouse"};
	char* too_long_empty[22] = {"1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", "1", 
								"1", "1", "1", "1", "1", "1", "1", "1", "1"};
	char* too_long[21] = {"gibbon", "gorilla", "orangutang", "chimpanzee", "bonobo",
						 "spider", "howler", "tamarin", "siamang", "macaque", "lemur",
						 "sifaka", "ayeaye", "loris", "potto", "angwantibo", "bushbaby",
						 "tarsier", "marmoset", "capuchin", "human"};
 	char* dont_all_parse[2] = {"yes", "no?"};
	wordlist_t tiny, pets, primates, empty, bools;
	strings_to_wordlist(small, ARRAYLEN(small), &tiny);
	strings_to_wordlist(animal_strings, ARRAYLEN(animal_strings), &pets);
	strings_to_wordlist(too_long, ARRAYLEN(too_long), &primates);
	strings_to_wordlist(too_long_empty, ARRAYLEN(too_long_empty), &empty);
	strings_to_wordlist(dont_all_parse, ARRAYLEN(dont_all_parse), &bools);

	print_words_in_list(&tiny);
	print_words_in_list(&pets);
	print_words_in_list(&empty);
	print_words_in_list(&primates);
	print_words_in_list(&pets);
	print_words_in_list(&bools);

	// test of random_between
//   	printf("%d, %d, %d\n", random_between(0,1), random_between(5,10), random_between(0,1000));
 //  	printf("Should contain random numbers between 0 & 1, 5 & 10, 0 & 1000.\n");

	/* test of shuffle
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

	// test of unshuffle
	unshuffle(10, bar);
	bool unshuffle_worked = true;
	for (int i = 0; i < 10; i++)
		if (bar[i] != i)
			unshuffle_worked = false;
	printf("Unshuffle %s.\n\n", unshuffle_worked ? "works" : "IS BROKEN"); */


}