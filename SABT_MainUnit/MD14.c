/*
 * @file MD14.c
 *
 * @brief Mode 14: spelling practice
 * @author: Marjorie Carlson (marjorie@cmu.edu)
 */ 

// Standard libraries
#include <stdbool.h>
#include <stdio.h>

// Custom libraries
#include "Globals.h"
#include "audio.h"
#include "datastructures.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h" 
#include "dictionary.h"
#include "vocab_nature.h"


#define MD14_STATE_NULL 0x00
#define MD14_STATE_INTRO 0x01
#define MD14_STATE_LVLSEL 0x02
#define MD14_STATE_GENQUES 0x03
#define MD14_STATE_PROMPT 0x04
#define MD14_STATE_INPUT 0x05
#define MD14_STATE_CHECK 0x06
#define MD14_STATE_REPROMPT 0x07

//bounds
#define MAX_INCORRECT_GUESS 3
#define MAX_WORD_LEN 20

 // Used to set global fileset variables
#define LANGUAGE "ENG_"
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD14"

char next_state = MD14_STATE_NULL;

int md14_words_spelled = 0;
int  md14_curr_mistakes = 0;
int md14_total_mistakes = 0;
char cell = 0;
char cell_pattern = 0;
char cell_control = 0;
static char md_last_dot = 0;

wordlist_t dict;
word_t* chosen_word;

static cell_t user_cell;
static cell_t curr_cell;

void md14_reset() {
	md14_curr_mistakes = 0;
	cell = 0;
	cell_pattern = 0;
	cell_control = 0;
	next_state = MD14_STATE_INTRO;
}

void stats(){
	play_mp3(MODE_FILESET, "STS1");      // "You have spelled"
	play_number(md14_words_spelled);     // #
	if (md14_words_spelled == 1)
		play_mp3(MODE_FILESET, "STS2"); // "word, and have made"
	else
		play_mp3(MODE_FILESET, "STS3");  // "words, and have made"
	play_number(md14_total_mistakes);    // #
	if (md14_total_mistakes == 1)
		play_mp3(MODE_FILESET, "STS4");  // "mistake"
	else
		play_mp3(MODE_FILESET, "STS5");  // "mistakes"
}

void correct_answer(){
	play_mp3(LANGUAGE, "CORR");
	play_mp3("SYS_", "TADA");
	md14_words_spelled++;
	stats();
	md14_reset();
}

void incorrect_answer() {
	play_mp3(LANGUAGE, "NO");
	play_mp3(LANGUAGE, MP3_TRY_AGAIN);
	decrement_word_index(chosen_word);
	md14_curr_mistakes++;
	md14_total_mistakes++;
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md14_main() {
  switch(next_state)
  {
    case MD14_STATE_INTRO:
    	PRINTF("In intro state\n\r");
    	play_mp3(MODE_FILESET, "INT");
    	lang_fileset = "ENG_";
		mode_fileset = "MD14";
		next_state = MD14_STATE_LVLSEL;
		srand(timer_rand());
		break;

	case MD14_STATE_LVLSEL:
        md_last_dot = create_dialog("LVLS", (DOT_1 | DOT_2 | DOT_3 | DOT_4 | DOT_5 | DOT_6));
        switch (md_last_dot) {

        	case NO_DOTS:
        		break;

        	case '1':
        		PRINTF("1\n");
        		strings_to_wordlist(three, 25, &dict); //todo: calculate length of array?
				print_words_in_list(&dict);
        		play_mp3(MODE_FILESET, "INST");
        		next_state = MD14_STATE_GENQUES;
        		break;

        	case '2':
        		PRINTF("1\n");
        		strings_to_wordlist(four, 45, &dict);
				print_words_in_list(&dict);
        		play_mp3(MODE_FILESET, "INST");
        		next_state = MD14_STATE_GENQUES;
        		break;

        	// todo: add other cases; figure out too-long words

        	default:
        		PRINTF("-");
				break;
		}
		break;

	case MD14_STATE_GENQUES:
		PRINTF("In genques state\n\r");
		md14_curr_mistakes = 0;
		get_next_word_in_wordlist(&dict, &chosen_word);
		sprintf(dbgstr, "[MD14] Next word: %s\n\r", chosen_word->name);
		PRINTF(dbgstr);
		next_state = MD14_STATE_PROMPT;
	  	break;

	 case MD14_STATE_PROMPT:
	 	PRINTF("In prompt state\n\r");
	 	play_mp3(LANGUAGE, "SPEL");
	 	speak_word(chosen_word);
	 	next_state = MD14_STATE_INPUT;
	 	break;

	case MD14_STATE_INPUT:
		cell = get_cell();
		if (cell == NO_DOTS) {
			break;
		}
		cell_pattern = GET_CELL_PATTERN(cell);
		cell_control = GET_CELL_CONTROL(cell);
		switch (cell_control) {
			case WITH_ENTER:
			user_cell.pattern = cell_pattern;
			next_state = MD14_STATE_CHECK;
			PRINTF("[MD14] Checking answer\n\r");
			break;
			case WITH_LEFT:
			next_state = MD14_STATE_REPROMPT;
			break;
			case WITH_RIGHT:
			stats();
			next_state = MD14_STATE_GENQUES;
			break;
			case WITH_CANCEL:
			break;
		}
		break;

	case MD14_STATE_CHECK:
		get_next_cell_in_word(chosen_word, &curr_cell);
		sprintf(dbgstr, "In check state. Current cell: %x, user cell: %x.\n\r", curr_cell.pattern, user_cell.pattern);
		PRINTF(dbgstr);

		if (cell_equals(&curr_cell, &user_cell)) {
			md14_curr_mistakes = 0;
			if (chosen_word->curr_letter == chosen_word->num_letters - 1) { // done
				correct_answer();
			  	next_state = MD14_STATE_GENQUES;
			}
			else {// correct but not done
				play_mp3(LANGUAGE, "GOOD");
				play_mp3(LANGUAGE, "NLET");
				next_state = MD14_STATE_INPUT;
			}
		}
		else {// incorrect letter
			incorrect_answer();
			next_state = MD14_STATE_REPROMPT;
		}
		break;

	case MD14_STATE_REPROMPT:
		PRINTF("In reprompt state\n\r");

		if (md14_curr_mistakes >= MAX_INCORRECT_GUESS) {
			play_mp3(LANGUAGE, "PRSS");
			play_pattern(curr_cell.pattern);
		}
		else {
			play_mp3(LANGUAGE, "SPEL");
	 		speak_word(chosen_word);
	 		if (chosen_word->curr_glyph > -1) {// not at beginning of word
	 			play_mp3(MODE_FILESET, "SOFA");
	 			speak_letters_so_far(chosen_word);
	 		}
	 	}
	 	next_state = MD14_STATE_INPUT;
	 	break;

	default:
		break;
  }
}
