/*
 * @file MD15.c
 *
 * @brief Mode 15: two-player spelling bee
 * @author: Marjorie Carlson (marjorie@cmu.edu)
 * @BUG: Memory leak if the user exits & reenters the
 * mode, because the memory allocated for the words &
 * wordlist can't be freed when the mode is exited.
 * In the worst case (if the user starts hard mode,
 * quits to the menu, restarts the mode, starts hard mode
 * again) only two games can be played before malloc
 * fails entirely.
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


#define MD15_STATE_NULL 0x00
#define MD15_STATE_INTRO 0x01
#define MD15_STATE_LVLSEL 0x02
#define MD15_STATE_GENQUES 0x03
#define MD15_STATE_PROMPT 0x04
#define MD15_STATE_INPUT 0x05
#define MD15_STATE_CHECK 0x06
#define MD15_STATE_SWITCH 0x07
#define MD15_STATE_REPROMPT 0x08
#define MD15_STATE_GAMEOVER 0x09

//bounds
#define MAX_INCORRECT_GUESS 3
#define MAX_WORD_LEN 20
#define GAMELENGTH 12

 // Used to set global fileset variables
#define LANGUAGE "ENG_"
#define LANG_FILESET "ENG_"
#define MODE_FILESET "M15_"

char next_state = MD15_STATE_NULL;

bool player1_is_current = true; // keep track of whether player 1 or 2 is the current player
int num_turns = 0;
int md15_p1_words_spelled = 0;
int md15_p2_words_spelled = 0;
int md15_p1_total_mistakes = 0;
int md15_p2_total_mistakes = 0;
int md15_curr_mistakes = 0;
char md15_cell = 0;
char md15_cell_pattern = 0;
int md15_cell_control = 0;
static char md15_last_dot = 0;

wordlist_t md15_dict;
word_t* md15_chosen_word;

static cell_t md15_user_cell;
static cell_t md15_curr_cell;

void md15_stats(){
	int words, mistakes;

	if (player1_is_current) {
		words = md15_p1_words_spelled;
		if (words == 0)
			return;
		mistakes = md15_p1_total_mistakes;
		play_direction(MP3_PLAYER_1);
	}
	else {
		words = md15_p2_words_spelled;
		if (words == 0)
			return;
		mistakes = md15_p2_total_mistakes;
		play_direction(MP3_PLAYER_2);
	}

	play_feedback(MP3_YOU_HAVE_SPELLED);
	play_number(words);                  // # of words
	if (mistakes == 0) {
		if (words == 1)
			play_feedback(MP3_WORD);
		else
			play_feedback(MP3_WORDS);
	}
	else {
		if (words == 1)
			play_feedback(MP3_WORD_AND_HAVE_MADE);
		else
			play_feedback(MP3_WORDS_AND_HAVE_MADE);
		play_number(mistakes);               // # of md15_curr_mistakes		
		if (mistakes == 1)
			play_feedback(MP3_MISTAKE);
		else
			play_feedback(MP3_MISTAKES);
	}
}

void md15_reset() {
	md15_curr_mistakes = 0;
	md15_cell = 0;
	md15_cell_pattern = 0;
	md15_cell_control = 0;
	next_state = MD15_STATE_INTRO;
}

void md15_correct_answer() {
	play_feedback(MP3_CORRECT);
	speak_letters_in_word(md15_chosen_word);
	speak_word(md15_chosen_word);
	play_tada();
	if (player1_is_current)
		md15_p1_words_spelled++;
	else
		md15_p2_words_spelled++;
}

void md15_incorrect_answer() {
	md15_curr_mistakes++;
	if (player1_is_current)
		md15_p1_total_mistakes++;
	else
		md15_p2_total_mistakes++;

	cell_t this_cell = {md15_cell_pattern};
	if (get_eng_letter_name_by_cell(&this_cell) != NULL) { //INVP may already have said "try again"
		play_feedback(MP3_NO);
		play_feedback(MP3_TRY_AGAIN);
	}
	decrement_word_index(md15_chosen_word);
	next_state = MD15_STATE_REPROMPT;
}

void md15_speak_inputted_cell() {
	cell_t this_cell = {md15_cell_pattern};
	char* letter_name = get_eng_letter_name_by_cell(&this_cell);
	if (letter_name == NULL)
		play_feedback(MP3_INVALID_PATTERN);
	else
		play_mp3(LANGUAGE,letter_name);
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md15_main() {
  switch(next_state) {
    case MD15_STATE_INTRO:
        lang_fileset = "e_";
		mode_fileset = "m15_";
		play_welcome();
    	play_number(GAMELENGTH);
    	play_feedback(MP3_WORDS);
		next_state = MD15_STATE_LVLSEL;
		srand(timer_rand());
		break;

	case MD15_STATE_LVLSEL:
        md15_last_dot = create_dialog("LVL3", (DOT_1 | DOT_2 | DOT_3));
        if (md15_last_dot == NO_DOTS)
        	break;
        switch (md15_last_dot) {
        	case '1':
        		play_direction(MP3_EASY_MODE);
   				strings_to_wordlist(easy, ARRAYLEN(easy), &md15_dict);
   				break;
        	case '2':
        		play_direction(MP3_MEDIUM_MODE);
  				strings_to_wordlist(medium, ARRAYLEN(medium), &md15_dict);
  				break;
        	case '3':
        		play_direction(MP3_HARD_MODE);
    			strings_to_wordlist(hard, ARRAYLEN(hard), &md15_dict);
    			break;
        }
		print_words_in_list(&md15_dict);
		play_instructions();
    	next_state = MD15_STATE_GENQUES;
    	break;

	case MD15_STATE_GENQUES:
		md15_reset();
		get_next_word_in_wordlist(&md15_dict, &md15_chosen_word);
		log_msg("[MD15] Next word: %s\n\r", md15_chosen_word->name);
		
		next_state = MD15_STATE_PROMPT;
	  	break;

	 case MD15_STATE_PROMPT:
	 		if (player1_is_current)
	 			play_direction(MP3_PLAYER_1);

	 		else
	 			play_direction(MP3_PLAYER_2);
	 	play_direction(MP3_SPELL_WORD);
	 	speak_word(md15_chosen_word);
	 	next_state = MD15_STATE_INPUT;
	 	break;

	case MD15_STATE_INPUT:
		md15_cell = get_cell();
		if (md15_cell == NO_DOTS) {
			break;
		}
		md15_cell_pattern = GET_CELL_PATTERN(md15_cell);
		md15_cell_control = GET_CELL_CONTROL(md15_cell);
		switch (md15_cell_control) {
			case WITH_ENTER:
			md15_user_cell.pattern = md15_cell_pattern;
			next_state = MD15_STATE_CHECK;
			log_msg("[MD15] Checking answer\n\r");
			break;
			case WITH_LEFT:
			next_state = MD15_STATE_REPROMPT;
			break;
			case WITH_RIGHT:
			md15_stats();
			next_state = MD15_STATE_GENQUES;
			break;
			case WITH_CANCEL:
			break;
		}
		break;

	case MD15_STATE_CHECK:
		md15_speak_inputted_cell();
		get_next_cell_in_word(md15_chosen_word, &md15_curr_cell);
		log_msg("Target cell: %x, inputted cell: %x.\n\r", md15_curr_cell.pattern, md15_user_cell.pattern);
		
		if (cell_equals(&md15_curr_cell, &md15_user_cell)) {
			if (md15_chosen_word->curr_letter == md15_chosen_word->num_letters - 1) { // done
				md15_correct_answer();
			  	next_state = MD15_STATE_SWITCH;
			}
			else {// correct but not done
				play_feedback(MP3_GOOD);
				play_mp3(LANGUAGE, MP3_NEXT_LETTER);
				md15_curr_mistakes = 0;
				next_state = MD15_STATE_INPUT;
			}
		}
		else
			md15_incorrect_answer(); // determines next state based on number of mistakes
		break;

	case MD15_STATE_SWITCH:
		md15_stats();
		player1_is_current = !player1_is_current; // switch players
		num_turns++;
		if (num_turns < GAMELENGTH * 2)
			next_state = MD15_STATE_GENQUES;
		else
			next_state = MD15_STATE_GAMEOVER;
		break;

	case MD15_STATE_REPROMPT:
	 	speak_word(md15_chosen_word);
	 	if (md15_curr_mistakes >= MAX_INCORRECT_GUESS) {
	 		play_direction(MP3_PLEASE_PRESS);
			char* letter_name = get_eng_letter_name_by_cell(&md15_curr_cell);
			play_mp3(LANGUAGE, letter_name);
			if (md15_curr_mistakes >= MAX_INCORRECT_GUESS + 1)
				play_pattern(md15_curr_cell.pattern);
		}

	 	else if (md15_chosen_word->curr_glyph > -1) { // not at beginning of word
	 		play_feedback(MP3_SPELLING_SO_FAR);
	 		speak_letters_so_far(md15_chosen_word);
	 	}
	 	next_state = MD15_STATE_INPUT;
	 	break;

	 case MD15_STATE_GAMEOVER:
	 	// a player wins if (s)he spelled more words OR spelled same number of words with fewer mistakes
	 	if (md15_p1_words_spelled > md15_p2_words_spelled
	 		|| (md15_p1_words_spelled == md15_p2_words_spelled
	 			&& md15_p1_total_mistakes < md15_p2_total_mistakes)) {
		 	log_msg("Winner is player 1!\n\r");
		 	play_feedback(MP3_GAME_OVER_WINNER_1);
		 }
		 else if (md15_p1_words_spelled < md15_p2_words_spelled
		 	|| (md15_p1_words_spelled == md15_p2_words_spelled
	 			&& md15_p1_total_mistakes > md15_p2_total_mistakes)) {
		 	log_msg("Winner is player 2!\n\r");
		 	play_feedback(MP3_GAME_OVER_WINNER_2);
		 }
		 else {// tie
		 	log_msg("The game is a draw.\n\r");
		 	play_feedback(MP3_GAME_OVER_DRAW);
		 }

		// output stats for both players
	 	player1_is_current = true;
	 	md15_stats();
	 	player1_is_current = false;
	 	md15_stats();
	 	next_state = MD15_STATE_LVLSEL;

	default:
		break;
  }
}
