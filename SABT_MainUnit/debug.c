#include <stdio.h>
#include "mode_6.h"
#include "globals.h"
#include "common.h"
#include "script_english.h"

// NEEDED GLOBALS 
script_t* this_script;
language_t mode_language;       // an enum designating current language, so correct feedback mp3s are played
char* lang_name;                // the user-readable name of the current language, e.g. "English"

unsigned char last_dot;         // the last dot pressed
unsigned char cell;             // bitmap representing the entire cell + control button last pressed
unsigned char cell_pattern;     // bitmap representing the braille cell itself -- the last six bits of cell
unsigned char cell_control;     // bitmap representing control button pressed -- the first two bits of the cell

// these keep track of the state, submode and difficulty level selected in the current mode.
state_t      current_state;
submode_t    submode;
difficulty_t level;

unsigned char score;        // num of right answers so far
unsigned char max_mistakes; // num mistakes allowed before the correct answer given
unsigned char mistakes;     // total mistakes made so far since the mode began
unsigned char curr_mistakes; // mistakes made in current question

// @todo: diff between these and last_cell, etc.?
extern char io_dot;
extern char io_cell;
extern char io_line[MAX_BUF_SIZE];
extern glyph_t* io_parsed[MAX_BUF_SIZE];
extern bool io_user_abort;

// Used to keep track of all modes and to determine which mode is currently selected
#define MAX_NUMBER_OF_MODES 20
unsigned short number_of_modes;
unsigned short ui_modes[MAX_NUMBER_OF_MODES];

bool is_a_mode_executing;
char current_mode; // this is the current mode; e.g., if mode_4 is running, this will be 4
int  index_of_current_mode;

// USED FOR ACCESSING AUDIO FILES

#define MAX_FILENAME_SIZE   13      // 8 + 1 + 3 + 1

bool playing_sound; // true if we're currently playing a sound file
volatile unsigned char *g_file_to_play;
unsigned char g_file_name[FILE_NAME_LEN];
//used throughout the dict_process
unsigned char dict_file_name[FILE_NAME_LEN];




// DONE
char new_input;

char get_cell(void) {
    return new_input | WITH_ENTER;
}



int main(){
    printf("Debug running!\n");

    char input_array[3] = {f_cell, a_cell, b_cell};


    reset_globals();
    current_mode = 6;

    for (int i = 0; i < 3; i++) {
        mode_6_main();
        new_input = input_array[i];
    }
}


/*
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "datastructures.h"
#include "script_english.h"
#include "script_hindi.h"
#include "common.h"

int main() {
    time_t t;
    srand((unsigned) time(&t));
    cell_t a_cell = DOT_1;
    cell_t b_cell = DOT_1 | DOT_2;
    cell_t hin_a_cell = DOT_1;
    cell_t i_cell = DOT_2 | DOT_5;
    cell_t all_cell = DOT_1 | DOT_2 | DOT_3 | DOT_4 | DOT_5 | DOT_6;
    cell_t blank_cell = NO_DOTS;
    cell_t also_blank = NO_DOTS;
    cell_t null_cell;

    // test of print_cell_pattern
    printf("0b000001 =? ");
    print_cell_pattern(a_cell);
    printf("0b000011 =? ");
    print_cell_pattern(b_cell);
    printf("0b111111 =? ");
    print_cell_pattern(all_cell);
    printf("Blank cell =? ");
    print_cell_pattern(blank_cell);
    printf("[?] =? ");
    print_cell_pattern(null_cell);
    printf("Compare above to test print_cell_pattern.\n\n");

    printf("%s\n", get_eng_letter_name_by_cell(a_cell));

    // test of cell equality
    bool diff1 = a_cell == b_cell;
    bool cognate1 = a_cell == hin_a_cell;
    bool empty1 = a_cell == blank_cell;
    bool bothempty1 = blank_cell == also_blank;
    bool notinitialized1 = a_cell == null_cell;
    printf("Cell equality %s.\n\n", (diff1 && cognate1 &&
            empty1 && bothempty1 && notinitialized1) ? "works" : "IS BROKEN");


    // test of letter_equals
    #define ABBREV      DOT_5
    #define RA          DOT_1 | DOT_2 | DOT_3 | DOT_5
    #define SHA         DOT_1 | DOT_4 | DOT_6
    cell_t shra_cells[3]    = {ABBREV, SHA, RA};
    letter_t hindi_shra = {shra_cells, 3, "shra", HINDI};

    letter_t eng_a = {&a_cell, 1, "a", ENGLISH};
    letter_t eng_b = {&b_cell, 1, "b", ENGLISH};
    letter_t hindi_a = {&a_cell, 1, "a", HINDI};
    letter_t hindi_i = {&i_cell, 1, "i", HINDI};
    letter_t blank_letter = {&blank_cell, 1, " ", LANGUAGE_NULL,};
    letter_t null_letter;

    bool same2 = letter_equals(&eng_a, &eng_a);
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

    printf("\nGet_eng_letter_by_char and print_letter work if the above reads 'zaz a shra.'\n\n");

    // test of parse_string_into_eng_word
    word_t goat, ox, chicken, myname, blank, notchars, funnycap, quitelong, verylong;
    parse_string_into_eng_word("goat", &goat);
    parse_string_into_eng_word("ox", &ox);
    parse_string_into_eng_word("chicken", &chicken);
    parse_string_into_eng_word("Marjorie", &myname);
    parse_string_into_eng_word("", &blank);
    parse_string_into_eng_word(":-)", &notchars);
    parse_string_into_eng_word("pH", &funnycap);
    parse_string_into_eng_word("abcdefghijklmnopqrst", &quitelong);
    parse_string_into_eng_word("abcdefghijklmnopqrstu", &verylong);


    // test of print_word;
    letter_t sri_letters[2] = {hindi_shra, hindi_i};
    word_t sri = {sri_letters, 2, 0, -1, "sri", HINDI};
    print_word(&goat);
    print_word(&ox);
    print_word(&chicken);
    print_word(&goat);
    print_word(&sri);
    print_word(&myname);
    print_word(&blank);
    print_word(&notchars);
    print_word(&funnycap);
    print_word(&quitelong);
    print_word(&verylong);

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
    printf("get_next_word_in_wordlist works if the three animals now appear in ~3 orders.\n\n");

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

    char* easy_words[70] = {"air", "bog", "bud", "bug", "day", "den", "dew", "dig", "dry", "fly", "fog", "log", "low", "oil", "raw", "rot", "sap", "sun", "web", "wet", "fit", "top", "sea", "gas", "ray", "cave", "damp", "dirt", "drip", "drop", "dump", "east", "edge", "fall", "fern", "hive", "hole", "lake", "leaf", "mist", "mold", "nest", "pond", "rain", "rest", "ripe", "rock", "root", "salt", "soil", "stem", "tide", "tree", "weed", "west", "wind", "wood", "melt", "bite", "path", "wash", "mass", "hill", "lava", "life", "seed", "star", "moon", "fast", "slow"};
    char* med_words[77] = {"adapt", "blade", "bloom", "brook", "cloud", "cycle", "fresh", "grass", "humid", "marsh", "night", "ocean", "plant", "river", "stone", "trash", "waste", "water", "ridge", "earth", "north", "south", "cliff", "gorge", "sleep", "moist", "steam", "coast", "sting", "renew", "flood", "erupt", "under", "joint", "float", "force", "repel", "solid", "larva", "orbit", "light", "sound", "absorb", "Arctic", "branch", "spring", "canopy", "canyon", "desert", "flower", "forest", "fungus", "growth", "insect", "mammal", "planet", "pollen", "stream", "system", "tundra", "valley", "ground", "winter", "summer", "rotten", "fossil", "nature", "matter", "energy", "record", "liquid", "motion", "sprout", "season", "magnet", "zigzag", "living"};

    wordlist_t easy, med;
    strings_to_wordlist(med_words, ARRAYLEN(med_words), &med);
    print_words_in_list(&med);

    word_t hi;
    parse_string_into_eng_word("hi", &hi);
    print_word(&hi);
    free_word(&hi);
    free_wordlist(&med);
    parse_string_into_eng_word("hello", &hi);
    print_word(&hi);

    // test of random_between
//      printf("%d, %d, %d\n", random_between(0,1), random_between(5,10), random_between(0,1000));
 //     printf("Should contain random numbers between 0 & 1, 5 & 10, 0 & 1000.\n");

    test of shuffle
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
    printf("Unshuffle %s.\n\n", unshuffle_worked ? "works" : "IS BROKEN");


} */
