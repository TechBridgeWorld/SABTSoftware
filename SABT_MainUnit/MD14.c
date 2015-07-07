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
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD14"

char next_state = MD14_STATE_NULL;

int  md14_num_mistakes = 0;
char cell = 0;
char cell_pattern = 0;
char cell_control = 0;
bool scrolled = false;

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
	scrolled = false;
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
		word_t adapt, water, steam, marsh, flood, larva;
		letter_t adapt_letters[5] = {eng_a, eng_d, eng_a, eng_p, eng_t};
		letter_t water_letters[5] = {eng_w, eng_a, eng_t, eng_e, eng_r};
		letter_t steam_letters[5] = {eng_s, eng_t, eng_e, eng_a, eng_m};
		letter_t marsh_letters[5] = {eng_m, eng_a, eng_r, eng_s, eng_h};
		letter_t flood_letters[5] = {eng_f, eng_l, eng_o, eng_o, eng_d};
		letter_t larva_letters[5] = {eng_l, eng_a, eng_r, eng_v, eng_a};
		initialize_english_word("ADAPT", adapt_letters, 5, &adapt);
		initialize_english_word("WATER", water_letters, 5, &water);
		initialize_english_word("STEAM", steam_letters, 5, &steam);
		initialize_english_word("MARSH", marsh_letters, 5, &marsh);
		initialize_english_word("FLOOD", flood_letters, 5, &flood);
		initialize_english_word("LARVA", larva_letters, 5, &larva);
		word_t wl[6] = {adapt, water, steam, marsh, flood, larva};
		initialize_wordlist(wl, 6, &dict);

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
		get_next_word_in_wordlist(&dict, &chosen_word);
		sprintf(dbgstr, "[MD14] Next word: %s\n\r", chosen_word->name);
		PRINTF(dbgstr);
        play_mp3(MODE_FILESET, "SPEL");
        speak_word(chosen_word);
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
		get_next_cell_in_word(chosen_word, &curr_cell);
		sprintf(dbgstr, "In check state. Current cell: %x, user cell: %x.\n\r", curr_cell.pattern, user_cell.pattern);
		PRINTF(dbgstr);

		if (cell_equals(&curr_cell, &user_cell)) {
			play_mp3(LANGUAGE, "GOOD");
			md14_num_mistakes = 0;
			if (chosen_word->curr_letter == chosen_word->num_letters) { // done
				play_mp3(LANGUAGE, "NCWK");
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
			speak_correct_letters(chosen_word);
			if (md14_num_mistakes >= MAX_INCORRECT_GUESS) {
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
