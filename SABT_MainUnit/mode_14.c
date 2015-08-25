/*
 * @file mode_14.c
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
#include "globals.h"
#include "audio.h"
#include "datastructures.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h" 
#include "dictionary.h"
#include "vocab.h"
#include "mp3s.h"
wordlist_t mode_14_dict;
word_t* mode_14_chosen_word;

static cell_t mode_14_curr_cell;


void mode_14_play_stats(){
    if (score == 0)
        return;
    play_feedback(MP3_YOU_HAVE_SPELLED);
    play_number(score);
    if (mistakes == 0) {
        if (score == 1)
            play_feedback(MP3_WORD);
        else
            play_feedback(MP3_WORDS);
    }
    else {
        if (score == 1)
            play_feedback(MP3_WORD_AND_HAVE_MADE);
        else
            play_feedback(MP3_WORDS_AND_HAVE_MADE);
        play_number(mistakes);  
        if (mistakes == 1)
            play_feedback(MP3_MISTAKE);
        else
            play_feedback(MP3_MISTAKES);
    }
}

void mode_14_correct_answer() {
    play_feedback(MP3_CORRECT);
    speak_letters_in_word(mode_14_chosen_word);
    speak_word(mode_14_chosen_word);
    play_tada();
    score++;
    mode_14_play_stats();
    reset_globals();
    reset_stats();
}

void mode_14_incorrect_answer() {
    play_feedback(MP3_NO);
    play_feedback(MP3_TRY_AGAIN);

    decrement_word_index(mode_14_chosen_word);
    curr_mistakes++;
    mistakes++;
}

void mode_14_reset() {
    max_mistakes = 3;
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void mode_14_main() {
  switch(current_state) {

    case INITIAL:
        play_welcome();
        current_state = CHOOSE_LEVEL;
        break;

    case CHOOSE_LEVEL:
        last_dot = create_dialog(MP3_CHOOSE_LEVELS_3, (DOT_1 | DOT_2 | DOT_3 | CANCEL));
        switch (last_dot) {
            case '1':
                play_direction(MP3_EASY_MODE);
                strings_to_wordlist(easy, ARRAYLEN(easy), &mode_14_dict);
                current_state = GENERATE_QUESTION;
                break;

            case '2':
                play_direction(MP3_MEDIUM_MODE);
                strings_to_wordlist(medium, ARRAYLEN(medium), &mode_14_dict);
                current_state = GENERATE_QUESTION;
                break;

            case '3':
                play_direction(MP3_HARD_MODE);
                strings_to_wordlist(hard, ARRAYLEN(hard), &mode_14_dict);
                current_state = GENERATE_QUESTION;
                break;

            case CANCEL:
                log_msg("Quitting to main menu.");                    
                quit_mode();
                break;

            default:
                break;
        }        
        break;

    case GENERATE_QUESTION:
        print_words_in_list(&mode_14_dict);
        play_direction(MP3_INSTRUCTIONS_WORD);
        curr_mistakes = 0;
        get_next_word_in_wordlist(&mode_14_dict, &mode_14_chosen_word);
        log_msg("[mode_14] Next word: %s", mode_14_chosen_word->name);
        current_state = PROMPT;
        break;

     case PROMPT:
        play_direction(MP3_SPELL_WORD);
        speak_word(mode_14_chosen_word);
        current_state = GET_INPUT;
        break;

    case GET_INPUT:
        cell = get_cell();
        if (cell == NO_DOTS)
            break;
        cell_pattern = GET_CELL_PATTERN(cell);
        cell_control = GET_CELL_CONTROL(cell);
        switch (cell_control) {
            case WITH_ENTER:
            current_state = CHECK_ANSWER;
            break;

            case WITH_LEFT:
            current_state = REPROMPT;
            break;

            case WITH_RIGHT:
            current_state = GENERATE_QUESTION;
            break;

            case WITH_CANCEL:
            break;
        }
        break;

    case CHECK_ANSWER:
        mode_14_curr_cell = get_next_cell_in_word(mode_14_chosen_word);
        log_msg("Target cell: %x, inputted cell: %x.", mode_14_curr_cell, cell_pattern);
        

        if (cell_pattern == mode_14_curr_cell) {
            if (mode_14_chosen_word->curr_letter == mode_14_chosen_word->num_letters - 1) { // done
                mode_14_correct_answer();
                current_state = GENERATE_QUESTION;
            }
            else {  // correct but not done
                play_feedback(MP3_GOOD);
                play_direction(MP3_NEXT_LETTER);
                curr_mistakes = 0;
                current_state = GET_INPUT;
            }
        }
        else {
            mode_14_incorrect_answer();
            current_state = REPROMPT;
        }
        break;

    case REPROMPT:
        if (curr_mistakes >= max_mistakes) {
            play_direction(MP3_PLEASE_PRESS);
            char* letter_name = get_eng_letter_name_by_cell(mode_14_curr_cell);
            play_lang_audio(letter_name);
            if (curr_mistakes >= max_mistakes + 1)
                play_cell(mode_14_curr_cell);
        }
        else {
            play_direction(MP3_SPELL_WORD);
            speak_word(mode_14_chosen_word);
            if (mode_14_chosen_word->curr_glyph > -1) { // not at beginning of word
                play_feedback(MP3_SPELLING_SO_FAR);
                speak_letters_so_far(mode_14_chosen_word);
                play_direction(MP3_NEXT_LETTER);
            }
        }
        current_state = GET_INPUT;
        break;

    default:
        log_msg("Invalid state_t %d", current_state);
        quit_mode();
        break;
  }
}
