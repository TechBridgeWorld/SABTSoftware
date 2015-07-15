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
#include "vocab.h"


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

char md14_next_state = MD14_STATE_NULL;

int md14_words_spelled = 0;
int  md14_curr_mistakes = 0;
int md14_total_mistakes = 0;
char md14_cell = 0;
char md14_cell_pattern = 0;
char md14_cell_control = 0;
static char md14_last_dot = 0;

wordlist_t md14_dict;
word_t* md14_chosen_word;

static cell_t md14_user_cell;
static cell_t md14_curr_cell;

void md14_reset() {
	md14_curr_mistakes = 0;
	md14_cell = 0;
	md14_cell_pattern = 0;
	md14_cell_control = 0;
	md14_next_state = MD14_STATE_INTRO;
}

void stats(){
	play_mp3(MODE_FILESET, "STS1");         // "You have spelled"
	play_number(md14_words_spelled);        // # of words
	if (md14_total_mistakes == 0) {
		if (md14_words_spelled == 1)
			play_mp3(MODE_FILESET,"WORD");  // word.   [if 1 word, 0 mistakes]
		else
			play_mp3(MODE_FILESET,"WRDS");  // words.  [if !1 word, 0 mistakes] 
	}
	else {
		if (md14_words_spelled == 1)
			play_mp3(MODE_FILESET, "STS2"); // "word, and have made" [if 1 word, >0 mistakes]
		else
			play_mp3(MODE_FILESET, "STS3");  // "words, and have made" [if >1 word, >0 mistakes]
		play_number(md14_total_mistakes);    // # of md15_curr_mistakes		
		if (md14_total_mistakes == 1)
			play_mp3(MODE_FILESET, "STS4");  // "mistake" [if 1 mistake]
		else
			play_mp3(MODE_FILESET, "STS5");  // "mistakes" [if >1 mistakes]
	}
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
	decrement_word_index(md14_chosen_word);
	md14_curr_mistakes++;
	md14_total_mistakes++;
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md14_main() {
  switch(md14_next_state)
  {
    case MD14_STATE_INTRO:
    	PRINTF("In intro state\n\r");
    	play_mp3(MODE_FILESET, "WELC");
    	lang_fileset = "ENG_";
		mode_fileset = "MD14";
		md14_next_state = MD14_STATE_LVLSEL;
		srand(timer_rand());
		break;

	case MD14_STATE_LVLSEL:
        md14_last_dot = create_dialog("LVLS", (DOT_1 | DOT_2));
        switch (md14_last_dot) {

        	case NO_DOTS:
        		break;

        	case '1':
        		play_mp3(LANGUAGE, "EASY");
        		strings_to_wordlist(easy, ARRAYLEN(easy), &md14_dict);
				print_words_in_list(&md14_dict);
        		play_mp3(MODE_FILESET, "INST");
        		md14_next_state = MD14_STATE_GENQUES;
        		break;

        	case '2':
        		play_mp3(LANGUAGE, "HARD");
        		strings_to_wordlist(medium, ARRAYLEN(medium), &md14_dict);
				print_words_in_list(&md14_dict);
        		play_mp3(MODE_FILESET, "INST");
        		md14_next_state = MD14_STATE_GENQUES;
        		break;

        	// todo: add case 3; figure out too-long words

        	default:
        		play_mp3(LANGUAGE, "INVP");
        		PRINTF("Invalid entry.");
				break;
		}
		break;

	case MD14_STATE_GENQUES:
		PRINTF("In genques state\n\r");
		md14_curr_mistakes = 0;
		get_next_word_in_wordlist(&md14_dict, &md14_chosen_word);
		sprintf(dbgstr, "[MD14] Next word: %s\n\r", md14_chosen_word->name);
		PRINTF(dbgstr);
		md14_next_state = MD14_STATE_PROMPT;
	  	break;

	 case MD14_STATE_PROMPT:
	 	PRINTF("In prompt state\n\r");
	 	play_mp3(LANGUAGE, "SPEL");
	 	speak_word(md14_chosen_word);
	 	md14_next_state = MD14_STATE_INPUT;
	 	break;

	case MD14_STATE_INPUT:
		md14_cell = get_cell();
		if (md14_cell == NO_DOTS) {
			break;
		}
		md14_cell_pattern = GET_CELL_PATTERN(md14_cell);
		md14_cell_control = GET_CELL_CONTROL(md14_cell);
		switch (md14_cell_control) {
			case WITH_ENTER:
			md14_user_cell.pattern = md14_cell_pattern;
			md14_next_state = MD14_STATE_CHECK;
			PRINTF("[MD14] Checking answer\n\r");
			break;
			case WITH_LEFT:
			md14_next_state = MD14_STATE_REPROMPT;
			break;
			case WITH_RIGHT:
			stats();
			md14_next_state = MD14_STATE_GENQUES;
			break;
			case WITH_CANCEL:
			break;
		}
		break;

	case MD14_STATE_CHECK:
		get_next_cell_in_word(md14_chosen_word, &md14_curr_cell);
		sprintf(dbgstr, "In check state. Current cell: %x, user cell: %x.\n\r", md14_curr_cell.pattern, md14_user_cell.pattern);
		PRINTF(dbgstr);

		if (cell_equals(&md14_curr_cell, &md14_user_cell)) {
			md14_curr_mistakes = 0;
			if (md14_chosen_word->curr_letter == md14_chosen_word->num_letters - 1) { // done
				correct_answer();
			  	md14_next_state = MD14_STATE_GENQUES;
			}
			else {// correct but not done
				play_mp3(LANGUAGE, "GOOD");
				play_mp3(LANGUAGE, "NLET");
				md14_next_state = MD14_STATE_INPUT;
			}
		}
		else {// incorrect letter
			incorrect_answer(); // @todo mark invalid letters invalid
			md14_next_state = MD14_STATE_REPROMPT;
		}
		break;

	case MD14_STATE_REPROMPT:
		PRINTF("In reprompt state\n\r");

		if (md14_curr_mistakes >= MAX_INCORRECT_GUESS) {
			play_mp3(LANGUAGE, "PRSS");
			play_pattern(md14_curr_cell.pattern);
		}
		else {
			play_mp3(LANGUAGE, "SPEL");
	 		speak_word(md14_chosen_word);
	 		if (md14_chosen_word->curr_glyph > -1) {// not at beginning of word
	 			play_mp3(MODE_FILESET, "SPLS");
	 			speak_letters_so_far(md14_chosen_word);
	 		}
	 	}
	 	md14_next_state = MD14_STATE_INPUT;
	 	break;

	default:
		break;
  }
}
