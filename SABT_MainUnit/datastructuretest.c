#include <stdio.h>
#include "datastructures.h"
#include "script_english.h"

int main() {
	cell_t a, b, c, hin_a, all, blank, null;
	a.pattern = DOTS1;
	b.pattern = DOTS12;
	hin_a.pattern = DOTS1;
	all.pattern = DOTS123456;
	blank.pattern = DOTS0;

	// test of print_cell_pattern
	print_cell_pattern(&a);
	print_cell_pattern(&b);
	print_cell_pattern(&all);
	print_cell_pattern(&blank);
	print_cell_pattern(&null);
	printf("\n\n");

	// test of cell_equals
	bool same1 = cell_equals(&a, &a);
	bool diff1 = !cell_equals(&a, &b);
	bool cognate1 = cell_equals(&a, &hin_a);
	bool empty1 = !cell_equals(&a, &blank);
	bool notinitialized1 = !cell_equals(&a, &null);
	printf("Cell_equals working: %s\n\n\n", (same1 && diff1 && cognate1 && empty1 && notinitialized1) ? "true" : "false");

	// test of letter_equals

	letter_t a_letter = {"a", ENGLISH, &a, 1};
	letter_t b_letter = {"b", ENGLISH, &b, 1};
	letter_t hin_a_letter = {"a", HINDI, &hin_a, 1};
	letter_t blank_letter = {" ", UNIVERSAL, &blank, 1};
	letter_t null_letter;


	bool same2 = letter_equals(&a_letter, &a_letter);
	bool diff2 = !letter_equals(&a_letter, &b_letter);
	bool cognate2 = !letter_equals(&a_letter, &hin_a_letter);
	bool empty2 = !letter_equals(&a_letter, &blank_letter);
	bool notinitialized2 = !letter_equals(&a_letter, &null_letter);
	printf("Letter_equals working: %s\n\n\n", (same2 && diff2 && cognate2 && empty2 && notinitialized2) ? "true" : "false");

	// test of parse_string_into_eng_word and word_to_string

	word_t baa;

	letter_t* whatasheepsays = {&b_letter, &a_letter, &a_letter};
	parse_string_into_eng_word("baa", &baa);
	printf("Word: %s, length: %d, lang: %d, num letters: %d\n", baa.name, baa.length_name, baa.lang_enum, baa.num_letters);

}