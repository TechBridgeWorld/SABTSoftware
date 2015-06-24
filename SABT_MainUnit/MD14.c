/*
 * @file md14_mode.c
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

static char *dictionary[11] = {"rock", "tree", "sky", "apple", "car", "dog",
    "cat", "water", "floor", "leaf", "road"};
static int index_arr[11] = {0,1,2,3,4,5,6,7,8,9,10};
static dictionary_t dict = {
    dictionary,
    11,
    0,
    index_arr
};

int  md14_num_mistakes;
char cell;
char cell_pattern;
char cell_control;
bool scrolled;
char* lang_fileset;

char *chosen_word;
char input_word[MAX_WORD_LEN];
int  length_entered_word;
//static char md_last_dot = NO_DOTS;
char next_state = MD14_STATE_NULL;
static glyph_t* user_glyph;
static glyph_t* curr_glyph;
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
    	// todo
    	play_mp3(MODE_FILESET, "INT");
    	dict.index = dict.num_words;
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
		if (dict.index >= dict.num_words) {
			shuffle(dict.num_words, dict.index_array);
			dict.index = 0;
		}
		chosen_word = dict.words[dict.index_array[dict.index]];
		dict.index++;

		char chosen_word_as_chars[10];
		sprintf(chosen_word_as_chars, "word:%s\r\n", chosen_word);
        PRINTF(chosen_word_as_chars);
        play_mp3(LANGUAGE, "PRSS");
        // @todo: say "Please spell:"
        // @todo: read whole word, not letters!
        play_string(chosen_word, strlen(chosen_word));
		next_state = MD14_STATE_INPUT;
	  break;
	  

	case MD14_STATE_INPUT:
	if (io_user_abort == true) {
		PRINTF("[MD3] User aborted input\n\r");
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
		user_glyph = search_script(this_script, cell_pattern);
		next_state = MD14_STATE_CHECK;
		PRINTF("[MD3] Checking answer\n\r");
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
		curr_glyph = search_script(this_script, get_bits_from_letter(chosen_word[length_entered_word]));
		if (glyph_equals(curr_glyph, user_glyph)) {
			play_glyph(curr_glyph);
			md14_num_mistakes = 0;
			length_entered_word++;
			user_word = add_glyph_to_word(user_word, user_glyph);
			if(length_entered_word != strlen(chosen_word)) { // not done
			  play_mp3(LANGUAGE, "GOOD");
			  next_state = MD14_STATE_INPUT;
			}
			else { // finished word
			  play_mp3(LANGUAGE,  "GOOD");
			  play_mp3(LANGUAGE, "NCWK");
			  play_word(user_word);
			  md14_num_mistakes = 0;
			  next_state = MD14_STATE_GENQUES;
			}
		}
		
		else { // incorrect letter
			play_glyph(user_glyph);
			md14_num_mistakes++;
			PRINTF("[MD3] User answered incorrectly\n\r");
			play_mp3(LANGUAGE, "NO");
			play_mp3(LANGUAGE, MP3_TRY_AGAIN);
			play_word(user_word);
			if (md14_num_mistakes >= MAX_INCORRECT_GUESS) {
				play_glyph(curr_glyph);
				play_mp3(MODE_FILESET, "PRSS");
				play_dot_sequence(curr_glyph);
			}
			next_state = MD14_STATE_INPUT;
		}
		break;
	
	case MD14_STATE_REPROMPT:
		switch(create_dialog("SKIP", ENTER_CANCEL | LEFT_RIGHT)) {
			case NO_DOTS:
				break;

			case CANCEL: case LEFT:
				PRINTF("[MD3] Reissuing prompt");
				next_state = MD14_STATE_PROMPT;
				scrolled = false;
				break;

			case ENTER:
				PRINTF("[MD3] Skipping chosen_word");
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
