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
#include "globals.h"
#include "audio.h"
#include "io.h"
#include "datastructures.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h" 
#include "dictionary.h"
#include "vocab.h"
#include "mp3s.h"

#define GAMELENGTH 12

bool player1_is_current = true; // keep track of whether player 1 or 2 is the current player
int num_turns = 0;
int p1_words_spelled = 0;
int p2_words_spelled = 0;
int p1_mistakes = 0;
int p2_mistakes = 0;

wordlist_t md15_dict;
word_t* chosen_word;

static cell_t user_cell;
static cell_t curr_cell;

void play_stats(){
    int words, mistakes;

    if (player1_is_current) {
        words = p1_words_spelled;
        if (words == 0)
            return;
        mistakes = p1_mistakes;
        play_direction(MP3_PLAYER_1);
    }
    else {
        words = p2_words_spelled;
        if (words == 0)
            return;
        mistakes = p2_mistakes;
        play_direction(MP3_PLAYER_2);
    }

    play_feedback(MP3_YOU_HAVE_SPELLED);
    play_number(words);
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
        play_number(mistakes);   
        if (mistakes == 1)
            play_feedback(MP3_MISTAKE);
        else
            play_feedback(MP3_MISTAKES);
    }
}

void correct_answer() {
    play_feedback(MP3_CORRECT);
    speak_letters_in_word(chosen_word);
    speak_word(chosen_word);
    play_tada();
    if (player1_is_current)
        p1_words_spelled++;
    else
        p2_words_spelled++;
}

void incorrect_answer() {
    curr_mistakes++;
    if (player1_is_current)
        p1_mistakes++;
    else
        p2_mistakes++;

    cell_t this_cell = {cell_pattern};
    if (get_eng_letter_name_by_cell(&this_cell) != NULL) { //INVP may already have said "try again"
        play_feedback(MP3_NO);
        play_feedback(MP3_TRY_AGAIN);
    }
    decrement_word_index(chosen_word);
    current_state = REPROMPT;
}

void speak_inputted_cell() {
    cell_t this_cell = {cell_pattern};
    char* letter_name = get_eng_letter_name_by_cell(&this_cell);
    if (letter_name == NULL)
        play_feedback(MP3_INVALID_PATTERN);
    else
        play_mp3(lang_fileset,letter_name);
}

void md15_reset() {
    reset_globals();
    reset_stats();
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md15_main() {
  switch(current_state) {

    case INITIAL:
        lang_fileset = get_lang_prefix();
        play_welcome();
        play_number(GAMELENGTH);
        play_feedback(MP3_WORDS);
        current_state = CHOOSE_LEVEL;
        break;

    case CHOOSE_LEVEL:
        last_dot = create_dialog(MP3_CHOOSE_LEVELS_3, (DOT_1 | DOT_2 | DOT_3));
        if (last_dot == NO_DOTS)
            break;

        switch (last_dot) {
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

            case CANCEL:
                log_msg("Quitting to main menu.");                    
                quit_mode();
                break;

            default:
                break;
        }

        print_words_in_list(&md15_dict);
        play_direction(MP3_INSTRUCTIONS_WORD);
        current_state = GENERATE_QUESTION;
        break;

    case GENERATE_QUESTION:
        reset_globals();
        reset_stats();
        get_next_word_in_wordlist(&md15_dict, &chosen_word);
        log_msg("[MD15] Next word: %s", chosen_word->name);
        current_state = PROMPT;
        break;

     case PROMPT:
        if (player1_is_current)
            play_direction(MP3_PLAYER_1);

        else
            play_direction(MP3_PLAYER_2);
        play_direction(MP3_SPELL_WORD);
        speak_word(chosen_word);
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
            user_cell.pattern = cell_pattern;
            current_state = CHECK_ANSWER;
            log_msg("[MD15] Checking answer");
            break;

            case WITH_LEFT:
            current_state = REPROMPT;
            break;

            case WITH_RIGHT:
            play_stats();
            current_state = GENERATE_QUESTION;
            break;

            case WITH_CANCEL:
            break;
        }
        break;

    case CHECK_ANSWER:
        speak_inputted_cell();
        get_next_cell_in_word(chosen_word, &curr_cell);
        log_msg("Target cell: %x, inputted cell: %x.", curr_cell.pattern, user_cell.pattern);
        
        if (cell_equals(&curr_cell, &user_cell)) {
            if (chosen_word->curr_letter == chosen_word->num_letters - 1) { // done
                correct_answer();
                current_state = SWITCH_USERS;
            }
            else {  // correct but not done
                play_feedback(MP3_GOOD);
                play_direction(MP3_NEXT_LETTER);
                curr_mistakes = 0;
                current_state = GET_INPUT;
            }
        }
        else
            incorrect_answer();
        break;

    case SWITCH_USERS:
        play_stats();
        player1_is_current = !player1_is_current; // switch players
        num_turns++;
        if (num_turns < GAMELENGTH * 2)
            current_state = GENERATE_QUESTION;
        else
            current_state = GAME_OVER;
        break;

    case REPROMPT:
        speak_word(chosen_word);
        if (curr_mistakes >= max_mistakes) {
            play_direction(MP3_PLEASE_PRESS);
            char* letter_name = get_eng_letter_name_by_cell(&curr_cell);
            play_mp3(lang_fileset, letter_name);
            if (curr_mistakes >= max_mistakes + 1)
                play_pattern(curr_cell.pattern);
        }

        else if (chosen_word->curr_glyph > -1) { // not at beginning of word
            play_feedback(MP3_SPELLING_SO_FAR);
            speak_letters_so_far(chosen_word);
        }
        current_state = GET_INPUT;
        break;

     case GAME_OVER:
        // a player wins if (s)he spelled more words OR spelled same number of words with fewer mistakes
        if (p1_words_spelled > p2_words_spelled
            || (p1_words_spelled == p2_words_spelled
                && p1_mistakes < p2_mistakes)) {
            log_msg("Winner is player 1!");
            play_feedback(MP3_GAME_OVER_WINNER_1);
         }
         else if (p1_words_spelled < p2_words_spelled
            || (p1_words_spelled == p2_words_spelled
                && p1_mistakes > p2_mistakes)) {
            log_msg("Winner is player 2!");
            play_feedback(MP3_GAME_OVER_WINNER_2);
         }
         else {// tie
            log_msg("The game is a draw.");
            play_feedback(MP3_GAME_OVER_DRAW);
         }

        // output stats for both players
        player1_is_current = true;
        play_stats();
        player1_is_current = false;
        play_stats();
        current_state = CHOOSE_LEVEL;

        default:
            log_msg("Invalid state_t %d", current_state);
            quit_mode();
            break;
  }
}
