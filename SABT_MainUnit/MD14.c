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
#define MD14_STATE_SUMMARY 0x08

//bounds
#define MAX_INCORRECT_GUESS 8
#define MAX_WORD_LEN 20

 // Used to set global fileset variables
#define LANGUAGE "ENG_"
#define MODE_FILESET "MD14"

int  md14_num_mistakes;
char cell;
char cell_pattern;
char cell_control;
bool scrolled;
char* lang_fileset;
wordlist_t dict;


word_t chosen_word;
char input_word[MAX_WORD_LEN];
int  length_entered_word;
//static char md_last_dot = NO_DOTS;
char next_state = MD14_STATE_NULL;
static glyph_t* user_glyph;
static glyph_t* curr_glyph;
static cell_t user_cell;
static cell_t curr_cell;
static word_node_t* user_word;
static script_t* this_script;

void md14_reset() {
	this_script = &script_english;
	lang_fileset = "ENG_";
	md14_num_mistakes = 0;
	cell = 0;
	cell_pattern = 0;
	cell_control = 0;
	length_entered_word = 0;
	next_state = MD14_STATE_INTRO;
	scrolled = false;
	word_t cat, dog, cow;
	turn_string_into_eng_word("cat", &cat);
	turn_string_into_eng_word("dog", &dog);
	turn_string_into_eng_word("cow", &cow);
	word_t wl[3] = {cat, dog, cow};
	initialize_wordlist(3, wl, &dict);

	user_glyph = NULL;
	curr_glyph = NULL;
	user_word = NULL;

}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md14_main() {
  switch(next_state)
  {
  	case MD14_STATE_NULL:
  		md14_reset();
  		break;
    case MD14_STATE_INTRO:
    	PRINTF("In intro state\n\r");
    	play_mp3(MODE_FILESET, "INT");
		next_state = MD14_STATE_LVLSEL;
		break;

	case MD14_STATE_LVLSEL:
	    PRINTF("In level set state\n\r");
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
/*		if (dict.index >= dict.length) {
			shuffle(dict.length, dict.order);
			dict.index = 0;
		} */
		chosen_word = dict.words[dict.index];// [dict.order[dict.index]];
		dict.index++;
		sprintf(dbgstr, "[MD14] Next word: %s\n\r", chosen_word.name);
		PRINTF(dbgstr);
        play_mp3(MODE_FILESET, "SPEL");
        speak_word(&chosen_word);
		next_state = MD14_STATE_INPUT;
	  	break;
	  

	case MD14_STATE_INPUT:
	if (io_user_abort == true) {
		PRINTF("[MD14] User aborted input\n\r");
		next_state = MD14_STATE_REPROMPT;
		io_init();
	}
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
		next_state = MD14_STATE_PROMPT;
		break;
		case WITH_RIGHT:
		next_state = MD14_STATE_REPROMPT;
		break;
		case WITH_CANCEL:
		break;
	}
	break;

	case MD14_STATE_CHECK:
		PRINTF("In check state\n\r");

		sprintf(dbgstr, "[Current cell: %x, user cell: %x.\n\r", curr_cell.pattern, user_cell.pattern);
		PRINTF(dbgstr);

		get_next_cell_in_word(&chosen_word, &curr_cell);
		if (cell_equals(&curr_cell, &user_cell)) {
			// @todo: play cell!
			play_mp3(LANGUAGE, "GOOD");
			md14_num_mistakes = 0;
			if (chosen_word.curr_letter == chosen_word.num_letters) { // done
				play_mp3(LANGUAGE, "NCWK");
				speak_word(&chosen_word);
			  	md14_num_mistakes = 0;
			  	next_state = MD14_STATE_GENQUES; // @todo: reset everything else
			}
			else // correct but not done
				next_state = MD14_STATE_INPUT;
		}
		else { // incorrect letter
			// @todo: play glyph entered
			md14_num_mistakes++;
			PRINTF("[MD14] User answered incorrectly\n\r");
			play_mp3(LANGUAGE, "NO");
			play_mp3(LANGUAGE, MP3_TRY_AGAIN);
			speak_correct_letters(&chosen_word);
			if (md14_num_mistakes >= MAX_INCORRECT_GUESS) {
				//@todo: play current glyph play_glyph(curr_glyph);
				play_mp3(MODE_FILESET, "PRSS");
				//@todo: play current dot sequence play_dot_sequence(curr_glyph);
			}
			next_state = MD14_STATE_INPUT;
		}
		break;
	
	case MD14_STATE_REPROMPT:
		switch(create_dialog("SKIP", ENTER_CANCEL | LEFT_RIGHT)) {
			case NO_DOTS:
				break;

			case CANCEL: case LEFT:
				PRINTF("[MD14] Reissuing prompt");
				next_state = MD14_STATE_PROMPT;
				scrolled = false;
				break;

			case ENTER:
				PRINTF("[MD14] Skipping chosen_word");
				if (scrolled)
					next_state = MD14_STATE_PROMPT;
				else
					next_state = MD14_STATE_GENQUES;
				scrolled = false;
				break;

			case RIGHT:
				next_state = MD14_STATE_GENQUES;
				break;

			default:
				break;
	}
	break;

	default:
	break;
  }
}
