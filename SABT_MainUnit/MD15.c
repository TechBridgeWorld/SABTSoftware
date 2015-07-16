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
#define GAMELENGTH 5

 // Used to set global fileset variables
#define LANGUAGE "ENG_"
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD15"

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
char md15_cell_control = 0;
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
		play_mp3(MODE_FILESET, "PLR1");
	}
	else {
		words = md15_p2_words_spelled;
		if (words == 0)
			return;
		mistakes = md15_p2_total_mistakes;
		play_mp3(MODE_FILESET, "PLR2");
	}

	play_mp3(MODE_FILESET, "STS1");      // "You have spelled"
	play_number(words);                  // # of words
	if (mistakes == 0) {
		if (words == 1)
			play_mp3(MODE_FILESET,"WORD");  // word.
		else
			play_mp3(MODE_FILESET,"WRDS");  // words.
	}
	else {
		if (words == 1)
			play_mp3(MODE_FILESET, "STS2"); // "word, and have made"
		else
			play_mp3(MODE_FILESET, "STS3");  // "words, and have made"
		play_number(mistakes);               // # of md15_curr_mistakes		
		if (mistakes == 1)
			play_mp3(MODE_FILESET, "STS4");  // "mistake"
		else
			play_mp3(MODE_FILESET, "STS5");  // "mistakes"
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
	play_mp3(LANGUAGE, "CORR");
	speak_letters_in_word(md15_chosen_word);
	speak_word(md15_chosen_word);
	play_mp3("SYS_", "TADA");
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

	if (md15_curr_mistakes <= MAX_INCORRECT_GUESS) {
		play_mp3(LANGUAGE, "NO");
		play_mp3(LANGUAGE, MP3_TRY_AGAIN);
		decrement_word_index(md15_chosen_word);
		next_state = MD15_STATE_REPROMPT;
	}
	else {
		play_mp3(LANGUAGE, "WRNG");
		speak_word(md15_chosen_word);
		play_mp3(MODE_FILESET, "ISSP");
		speak_letters_in_word(md15_chosen_word);
		next_state = MD15_STATE_SWITCH;
	}
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md15_main() {
  switch(next_state) {
    case MD15_STATE_INTRO:
    	play_mp3(MODE_FILESET, "WELC");
    	lang_fileset = "ENG_";
		mode_fileset = "MD15";
		next_state = MD15_STATE_LVLSEL;
		srand(timer_rand());
		break;

	case MD15_STATE_LVLSEL:
        md15_last_dot = create_dialog("LVL3", (DOT_1 | DOT_2 | DOT_3));
        if (md15_last_dot == NO_DOTS)
        	break;
        switch (md15_last_dot) {
        	case '1':
				play_mp3(LANGUAGE, "EASY");
   				strings_to_wordlist(easy, ARRAYLEN(easy), &md15_dict);
   				break;
        	case '2':
				play_mp3(LANGUAGE, "MED");
  				strings_to_wordlist(medium, ARRAYLEN(medium), &md15_dict);
  				break;
        	case '3':
				play_mp3(LANGUAGE, "HARD");
    			strings_to_wordlist(hard, ARRAYLEN(hard), &md15_dict);
    			break;
        }
		print_words_in_list(&md15_dict);
    	play_mp3(MODE_FILESET, "INST");
    	next_state = MD15_STATE_GENQUES;
    	break;

	case MD15_STATE_GENQUES:
		md15_reset();
		get_next_word_in_wordlist(&md15_dict, &md15_chosen_word);
		sprintf(dbgstr, "[MD15] Next word: %s\n\r", md15_chosen_word->name);
		PRINTF(dbgstr);
		next_state = MD15_STATE_PROMPT;
	  	break;

	 case MD15_STATE_PROMPT:
	 		if (player1_is_current)
	 			play_mp3(MODE_FILESET, "PLR1");
	 		else
	 			play_mp3(MODE_FILESET, "PLR2");
	 	play_mp3(LANGUAGE, "SPEL");
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
			PRINTF("[MD15] Checking answer\n\r");
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
		get_next_cell_in_word(md15_chosen_word, &md15_curr_cell);
		sprintf(dbgstr, "Target cell: %x, inputted cell: %x.\n\r", md15_curr_cell.pattern, md15_user_cell.pattern);
		PRINTF(dbgstr);

		if (cell_equals(&md15_curr_cell, &md15_user_cell)) {
			if (md15_chosen_word->curr_letter == md15_chosen_word->num_letters - 1) { // done
				md15_correct_answer();
			  	next_state = MD15_STATE_SWITCH;
			}
			else {// correct but not done
				play_mp3(LANGUAGE, "GOOD");
				play_mp3(LANGUAGE, "NLET");
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
	 	if (md15_chosen_word->curr_glyph > -1) { // not at beginning of word
	 		play_mp3(MODE_FILESET, "SPLS");
	 		speak_letters_so_far(md15_chosen_word);
	 	}
	 	next_state = MD15_STATE_INPUT;
	 	break;

	 case MD15_STATE_GAMEOVER:
	 	// a player wins if (s)he spelled more words OR spelled same number of words with fewer mistakes
	 	if (md15_p1_words_spelled > md15_p2_words_spelled
	 		|| (md15_p1_words_spelled == md15_p2_words_spelled
	 			&& md15_p1_total_mistakes < md15_p2_total_mistakes)) {
		 	PRINTF("Winner is player 1!\n\r");
		 	play_mp3(MODE_FILESET, "WIN1");
		 }
		 else if (md15_p1_words_spelled < md15_p2_words_spelled
		 	|| (md15_p1_words_spelled == md15_p2_words_spelled
	 			&& md15_p1_total_mistakes > md15_p2_total_mistakes)) {
		 	PRINTF("Winner is player 2!\n\r");
		 	play_mp3(MODE_FILESET, "WIN2");
		 }
		 else {// tie
		 	PRINTF("Tie!\n\r");
		 	play_mp3(MODE_FILESET, "WIN0");
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
