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
wordlist_t md14_dict;
word_t* md14_chosen_word;

static cell_t md14_user_cell;
static cell_t md14_curr_cell;


void md14_play_stats(){
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

void md14_correct_answer() {
    play_feedback(MP3_CORRECT);
    speak_letters_in_word(md14_chosen_word);
    speak_word(md14_chosen_word);
    play_tada();
    score++;
    md14_play_stats();
    reset_globals();
    reset_stats();
}

void md14_incorrect_answer() {
    play_feedback(MP3_NO);
    play_feedback(MP3_TRY_AGAIN);

    decrement_word_index(md14_chosen_word);
    curr_mistakes++;
    mistakes++;
}

void md14_speak_inputted_cell() {
    cell_t this_cell = {cell};
    char* letter_name = get_eng_letter_name_by_cell(&this_cell);
    play_mp3(lang_fileset, letter_name);
}

void md14_reset() {
    reset_globals();
    reset_stats();
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md14_main() {
  switch(next_state) {

    case STATE_INITIAL:
        srand(timer_rand());
        play_welcome();
        next_state = STATE_CHOOSE_LEVEL;
        break;

    case STATE_CHOOSE_LEVEL:
        last_dot = create_dialog(MP3_CHOOSE_LEVELS_3, (DOT_1 | DOT_2 | DOT_3 | CANCEL));
        switch (last_dot) {
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

            case CANCEL:
                log_msg("Quitting to main menu.");                    
                quit_mode();
                break;

            default:
                break;
        }

        print_words_in_list(&md14_dict);
        play_direction(MP3_INSTRUCTIONS_WORD);
        next_state = STATE_GENERATE_QUESTION;
        break;

    case STATE_GENERATE_QUESTION:
        reset_globals();
        curr_mistakes = 0;
        get_next_word_in_wordlist(&md14_dict, &md14_chosen_word);
        log_msg("[MD14] Next word: %s", md14_chosen_word->name);
        next_state = STATE_PROMPT;
        break;

     case STATE_PROMPT:
        play_direction(MP3_SPELL_WORD);
        speak_word(md14_chosen_word);
        next_state = STATE_GET_INPUT;
        break;

    case STATE_GET_INPUT:
        cell = get_cell();
        if (cell == NO_DOTS)
            break;
        cell_pattern = GET_CELL_PATTERN(cell);
        cell_control = GET_CELL_CONTROL(cell);
        switch (cell_control) {
            case WITH_ENTER:
            md14_user_cell.pattern = cell_pattern;
            next_state = STATE_CHECK_ANSWER;
            break;

            case WITH_LEFT:
            next_state = STATE_REPROMPT;
            break;

            case WITH_RIGHT:
            next_state = STATE_GENERATE_QUESTION;
            break;

            case WITH_CANCEL:
            break;
        }
        break;

    case STATE_CHECK_ANSWER:
        md14_speak_inputted_cell();
        get_next_cell_in_word(md14_chosen_word, &md14_curr_cell);
        log_msg("Target cell: %x, inputted cell: %x.", md14_curr_cell.pattern, md14_user_cell.pattern);
        

        if (cell_equals(&md14_curr_cell, &md14_user_cell)) {
            if (md14_chosen_word->curr_letter == md14_chosen_word->num_letters - 1) { // done
                md14_correct_answer();
                next_state = STATE_GENERATE_QUESTION;
            }
            else {  // correct but not done
                play_feedback(MP3_GOOD);
                play_direction(MP3_NEXT_LETTER);
                curr_mistakes = 0;
                next_state = STATE_GET_INPUT;
            }
        }
        else {
            md14_incorrect_answer();
            next_state = STATE_REPROMPT;
        }
        break;

    case STATE_REPROMPT:
        if (curr_mistakes >= max_mistakes) {
            play_direction(MP3_PLEASE_PRESS);
            char* letter_name = get_eng_letter_name_by_cell(&md14_curr_cell);
            play_mp3(lang_fileset, letter_name);
            if (curr_mistakes >= max_mistakes + 1)
                play_pattern(md14_curr_cell.pattern);
        }
        else {
            play_direction(MP3_SPELL_WORD);
            speak_word(md14_chosen_word);
            if (md14_chosen_word->curr_glyph > -1) { // not at beginning of word
                play_feedback(MP3_SPELLING_SO_FAR);
                speak_letters_so_far(md14_chosen_word);
                play_direction(MP3_NEXT_LETTER);
            }
        }
        next_state = STATE_GET_INPUT;
        break;

    default:
        break;
  }
}
