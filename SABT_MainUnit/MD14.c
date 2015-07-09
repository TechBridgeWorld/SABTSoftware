/*
 * @file MD14.c
 *
 * @brief Mode 14: spelling bee
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

int  md14_num_mistakes = 0;
char cell = 0;
char cell_pattern = 0;
char cell_control = 0;

wordlist_t dict;
word_t* chosen_word;

static cell_t user_cell;
static cell_t curr_cell;

void md14_reset() {
	md14_num_mistakes = 0;
	cell = 0;
	cell_pattern = 0;
	cell_control = 0;
	next_state = MD14_STATE_INTRO;
}

void set_level_med(){
		char* science_words[6] = {"adapt", "water", "steam", "marsh", "flood", "larva"};
		strings_to_wordlist(science_words, 6, &dict);
		print_words_in_list(&dict);
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
    	play_mp3(MODE_FILESET, "INST");
    	lang_fileset = "ENG_";
		mode_fileset = "MD14";
		next_state = MD14_STATE_LVLSEL;
		break;

	case MD14_STATE_LVLSEL:
	    PRINTF("In level set state\n\r");
	    set_level_med();

/*		play_mp3(MODE_FILESET, "LVLS");
        md_last_dot = create_dialog(MP3_LEVEL, DOT_1 | DOT_2 | DOT_3 );
        switch (md_last_dot) {
			case NO_DOTS:
        	case '1':
        	case '2':
        	case '3':
				break;
		} */
		next_state = MD14_STATE_GENQUES;
	  
	case MD14_STATE_GENQUES:
		PRINTF("In genques state\n\r");
		md14_num_mistakes = 0;
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
			md14_num_mistakes = 0;
			if (chosen_word->curr_letter == chosen_word->num_letters - 1) { // done
				play_mp3(LANGUAGE, "CORR");
				play_mp3("SYS_", "TADA");
				md14_reset();
			  	next_state = MD14_STATE_GENQUES;
			}
			else {// correct but not done
				play_mp3(LANGUAGE, "GOOD");
				next_state = MD14_STATE_INPUT;
			}
		}
		else {// incorrect letter
			play_mp3(LANGUAGE, "NO");
			play_mp3(LANGUAGE, MP3_TRY_AGAIN);
			decrement_word_index(chosen_word);
			md14_num_mistakes++;
			next_state = MD14_STATE_REPROMPT;
		}
		break;

	case MD14_STATE_REPROMPT:
		PRINTF("In reprompt state\n\r");

		if (md14_num_mistakes >= MAX_INCORRECT_GUESS) {
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
