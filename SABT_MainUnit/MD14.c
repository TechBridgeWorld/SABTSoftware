/*
 * @file MD14.c
 *
 * @brief Mode 14: spelling practice
 * @author: Marjorie Carlson (marjorie@cmu.edu)
 * @BUG: Memory leak if the user exits & reenters the
 * mode, because the memory allocated for the words &
 * wordlist can't be freed when the mode is exited.
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
#include "mp3s.h"


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

char md14_next_state = MD14_STATE_NULL;

int md14_words_spelled = 0;
int md14_curr_mistakes = 0;
int md14_total_mistakes = 0;
char md14_cell = 0;
char md14_cell_pattern = 0;
int md14_cell_control = 0;
static char md14_last_dot = 0;
char* lang_fileset;

wordlist_t md14_dict;
word_t* md14_chosen_word;

static cell_t md14_user_cell;
static cell_t md14_curr_cell;


void md14_stats(){
	if (md14_words_spelled == 0)
		return;
	play_feedback(MP3_YOU_HAVE_SPELLED);
	play_number(md14_words_spelled);        // # of words
	if (md14_total_mistakes == 0) {
		if (md14_words_spelled == 1)
			play_feedback(MP3_WORD);
		else
			play_feedback(MP3_WORDS);
	}
	else {
		if (md14_words_spelled == 1)
			play_feedback(MP3_WORD_AND_HAVE_MADE);
		else
			play_feedback(MP3_WORDS_AND_HAVE_MADE);
		play_number(md14_total_mistakes);    // # of md14_curr_mistakes		
		if (md14_total_mistakes == 1)
			play_feedback(MP3_MISTAKE);
		else
			play_feedback(MP3_MISTAKES);
	}
}

void md14_reset() {
	md14_curr_mistakes = 0;
	md14_cell = 0;
	md14_cell_pattern = 0;
	md14_cell_control = 0;
	md14_next_state = MD14_STATE_INTRO;
}

void md14_correct_answer() {
	play_feedback(MP3_CORRECT);
	speak_letters_in_word(md14_chosen_word);
	speak_word(md14_chosen_word);
	play_tada();
	md14_words_spelled++;
	md14_stats();
	md14_reset();
}

void md14_incorrect_answer() {
	play_feedback(MP3_NO);
	play_feedback(MP3_TRY_AGAIN);

	decrement_word_index(md14_chosen_word);
	md14_curr_mistakes++;
	md14_total_mistakes++;
}

void md14_speak_inputted_cell() {
	cell_t this_cell = {md14_cell};
	char* letter_name = get_eng_letter_name_by_cell(&this_cell);
	play_mp3(lang_fileset, letter_name);
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md14_main() {
  switch(md14_next_state) {
    case MD14_STATE_INTRO:
    	lang_fileset = get_lang_prefix();
		play_welcome();
		md14_next_state = MD14_STATE_LVLSEL;
		srand(timer_rand());
		break;

	case MD14_STATE_LVLSEL:
        md14_last_dot = create_dialog(MP3_CHOOSE_LEVELS_3, (DOT_1 | DOT_2 | DOT_3));
        if (md14_last_dot == NO_DOTS)
        	break;
        switch (md14_last_dot) {
        	case '1':
        		play_direction(MP3_EASY_MODE);
   				strings_to_wordlist(easy, ARRAYLEN(easy), &md14_dict);
   				break;
        	case '2':
        	    play_direction(MP3_MEDIUM_MODE);
  				strings_to_wordlist(medium, ARRAYLEN(medium), &md14_dict);
  				break;
        	case '3':
				play_direction(MP3_HARD_MODE);
    			strings_to_wordlist(hard, ARRAYLEN(hard), &md14_dict);
    			break;
        }
		print_words_in_list(&md14_dict);
		play_direction(MP3_INSTRUCTIONS_WORD);
    	md14_next_state = MD14_STATE_GENQUES;
    	break;

	case MD14_STATE_GENQUES:
		md14_reset();
		get_next_word_in_wordlist(&md14_dict, &md14_chosen_word);
		log_msg("[MD14] Next word: %s\n\r", md14_chosen_word->name);
		
		md14_next_state = MD14_STATE_PROMPT;
	  	break;

	 case MD14_STATE_PROMPT:
	 	play_direction(MP3_SPELL_WORD);
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
			log_msg("[MD14] Checking answer\n\r");
			break;
			case WITH_LEFT:
			md14_next_state = MD14_STATE_REPROMPT;
			break;
			case WITH_RIGHT:
			md14_next_state = MD14_STATE_GENQUES;
			break;
			case WITH_CANCEL:
			break;
		}
		break;

	case MD14_STATE_CHECK:
		md14_speak_inputted_cell();
		get_next_cell_in_word(md14_chosen_word, &md14_curr_cell);
		log_msg("Target cell: %x, inputted cell: %x.\n\r", md14_curr_cell.pattern, md14_user_cell.pattern);
		

		if (cell_equals(&md14_curr_cell, &md14_user_cell)) {
			if (md14_chosen_word->curr_letter == md14_chosen_word->num_letters - 1) { // done
				md14_correct_answer();
			  	md14_next_state = MD14_STATE_GENQUES;
			}
			else {// correct but not done
				play_feedback(MP3_GOOD);
				play_direction(MP3_NEXT_LETTER);
				md14_curr_mistakes = 0;
				md14_next_state = MD14_STATE_INPUT;
			}
		}
		else {
			md14_incorrect_answer();
			md14_next_state = MD14_STATE_REPROMPT;
		}
		break;

	case MD14_STATE_REPROMPT:
		if (md14_curr_mistakes >= MAX_INCORRECT_GUESS) {
			play_direction(MP3_PLEASE_PRESS);
			char* letter_name = get_eng_letter_name_by_cell(&md14_curr_cell);
			play_mp3(lang_fileset, letter_name);
			if (md14_curr_mistakes >= MAX_INCORRECT_GUESS + 1)
				play_pattern(md14_curr_cell.pattern);
		}
		else {
			play_direction(MP3_SPELL_WORD);
	 		speak_word(md14_chosen_word);
	 		if (md14_chosen_word->curr_glyph > -1) {// not at beginning of word
	 			play_feedback(MP3_SPELLING_SO_FAR);
	 			speak_letters_so_far(md14_chosen_word);
				play_direction(MP3_NEXT_LETTER);
	 		}
	 	}
	 	md14_next_state = MD14_STATE_INPUT;
	 	break;

	default:
		break;
  }
}
