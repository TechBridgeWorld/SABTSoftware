/**
 * @file MD5.c
 * @brief Mode 5 code - Hangman (two player)
 * @author Kory Stiger (kstiger) & Marjorie Carlson (marjorie)
 */

#include "globals.h"
#include "Modes.h"
#include "letter_globals.h"
#include "audio.h"
#include "mp3s.h"

  // State definitions

#define MD5_STATE_INTRO             1       // Just started, load dictionary if needed
#define MD5_STATE_INPUT_GOAL        2       // Player 1 enters word
#define MD5_STATE_PROCESS_GOAL      3       // Check for valid input word
#define MD5_STATE_SWITCH_USERS      4       // Wait for transition to player2 to press enter
#define MD5_STATE_ASK_FOR_GUESS     6       // Prompt user for input
#define MD5_STATE_INPUT_GUESS       7       // Waiting for user input
#define MD5_STATE_CHECKANS          8       // Process user input
#define MD5_STATE_CHECK_MATCH       9       // Valid input was entered
#define MD5_STATE_EVALUATE_GAME     10      // Check for win or lose

#define MAX_LEN              10
#define MAX_INCORRECT_GUESS   8     // if change this, rerecord the "you lose" mp3.

#define LANG_FILESET "e_"

// State variables
static char md5_next_state;
static char md5_last_dot = NO_DOTS;
static char mistake_pool[MAX_INCORRECT_GUESS + 1] = "";
static int md5_incorrect_tries = 0;
char entered_letter;

// Globals
char md5_chosen_word[MAX_LEN + 1];    // Holds the current word being guessed
char input_word[MAX_LEN + 1];         // Holds the correctly input letters
int input_word_index;                 // Used to read out the status of the current input word
bool is_game_over;

char last_cell, expected_dot;

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not. The letter is saved in
 * entered_letter.
 */

bool md5_valid_letter(char button_bits) {
    char letter_from_bits = get_letter_from_bits(button_bits);
    if ((letter_from_bits >= 'a') && (letter_from_bits <= 'z')) {
        entered_letter = letter_from_bits;
        return true;
    }
    log_msg("Invalid pattern.");
    return false;
}

void md5_init_char_arr(char* arr, int len){
    for (int i = 0; i < len; i++)
        arr[i] = '\0';
}

/**
 *  Check if the user made the same mistake before
 *
 *  @param entered_letter used enterd letter
 *
 *  @return whether it's past mistake
 */
bool md5_is_past_mistake(char entered_letter){
    int i = 0;
    while (mistake_pool[i] != '\0') {
        if (mistake_pool[i] == entered_letter) return true;
        i++;
    }
    return false;
}

/*
 * @brief Try to place global entered_letter into global input_word. 
 * @return bool - True on success (finds a match and places letter), 
 *                false failure (doesn't find a match)
 */
bool md5_place_letter() {
  bool found_match = false;

  for (int i = 0; i < strlen(md5_chosen_word); i++) {
    if (entered_letter == md5_chosen_word[i]) {
      input_word[i] = entered_letter;
      found_match = true;
    }
  }

  return found_match;
}


void md5_reset(void) {
    md5_init_char_arr(md5_chosen_word, MAX_LEN + 1);
    md5_init_char_arr(input_word,  MAX_LEN + 1);
    md5_init_char_arr(mistake_pool, MAX_INCORRECT_GUESS + 1);
    input_word_index = 0;
    md5_incorrect_tries = 0;
    is_game_over = false;
    md5_next_state = MD5_STATE_INTRO;
    md5_last_dot = 0;
    lang_fileset = LANG_FILESET;
}

/*
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md5_main(void) {
    switch(md5_next_state) {

        case MD5_STATE_INTRO:
            md5_reset();
/*            if (!done_rd_dict) {
                log_msg("Reading dictionary file...");
                play_direction(MP3_PLEASE_WAIT)
                init_read_dict((unsigned char *)"wordsEn.txt");
                while (!done_rd_dict)
                    read_dict_file();
            } */
            play_welcome();
            md5_next_state = MD5_STATE_INPUT_GOAL;
            break;



        case MD5_STATE_INPUT_GOAL:
        if (got_input) {
            got_input = false;
            md5_next_state = MD5_STATE_PROCESS_GOAL;
        }
        break;



        case MD5_STATE_PROCESS_GOAL:
            log_msg("Entering process solution state.");

            if (last_cell == 0) { // if return was entered twice
                md5_chosen_word[input_word_index] = '\0';

//                if (bin_srch_dict((unsigned char *)md5_chosen_word)) { // valid word; switch to player 2
                    // @TODO "valid word, please hand device to player 2 and press enter when ready"
//                    play_mode_audio(MP3_VALID_WORD_PASS_DEVICE);
                    input_word_index = 0;

                    play_feedback(MP3_YOUR_WORD_IS);
                    play_string(md5_chosen_word, strlen(md5_chosen_word));
                    play_mode_audio(MP3_PASS_DEVICE_PRESS_ENTER);
                    md5_next_state = MD5_STATE_SWITCH_USERS;

   /*             }
                else { // invalid word; clear variables and try again
                    play_feedback(MP3_WORD_NOT_FOUND);
                    md5_reset();
                    md5_next_state = MD5_STATE_INPUT_GOAL;
                } */
            }
            
            else if (md5_valid_letter(last_cell)) { // letter valid; word not complete
                log_msg("Letter %c entered.", entered_letter);
                char letter[2] = {entered_letter, '\0'};
                play_mp3(LANG_FILESET, letter);

                // reset because too many letters were input
                if (input_word_index == MAX_LEN) {
                    log_msg("Too many letters inputted!");
                    play_feedback(MP3_TOO_LONG);
                    md5_reset();
                    md5_next_state = MD5_STATE_INPUT_GOAL;
                    break;
                }

                md5_chosen_word[input_word_index] = entered_letter;  // add letter to chosen_word
                input_word_index++;
                md5_next_state = MD5_STATE_INPUT_GOAL;
            }
            else { // invalid letter
                play_feedback(MP3_INVALID_PATTERN);
                md5_next_state = MD5_STATE_INPUT_GOAL;
            }
            break;



        case MD5_STATE_SWITCH_USERS:
            if (got_input) {
                got_input = false;
                md5_next_state = MD5_STATE_ASK_FOR_GUESS;
            }
            break;


        case MD5_STATE_ASK_FOR_GUESS:
            log_msg("Entering ask for guess state.");
            play_direction(MP3_PLAYER_2);
            play_direction(MP3_YOUR_WORD_IS_NOW);
            play_string(input_word, strlen(md5_chosen_word));
            play_direction(MP3_GUESS_A_LETTER);
            md5_next_state = MD5_STATE_INPUT_GUESS;
            break;




        case MD5_STATE_INPUT_GUESS:
            if (got_input) {
                got_input = false;
                md5_next_state = MD5_STATE_CHECKANS;
            }
            break;




        case MD5_STATE_CHECKANS:
            log_msg("Entering checkans state.");
            if (last_cell == 0) // nothing entered: repeat word
                md5_next_state = MD5_STATE_EVALUATE_GAME;

            else if (md5_valid_letter(last_cell)) { // valid letter: read aloud
                char buff[7];
                sprintf(buff, "%c", entered_letter);
                play_mp3(LANG_FILESET, buff);  //@todo fix this
                md5_next_state = MD5_STATE_CHECK_MATCH;
            }
            else {
                play_feedback(MP3_INVALID_PATTERN);
                md5_incorrect_tries++;
                md5_next_state = MD5_STATE_EVALUATE_GAME;
            }
            break;





        case MD5_STATE_CHECK_MATCH:
            log_msg("Entering check match state.");
            if (md5_place_letter()) // letters is in word; fn places it
                play_feedback(MP3_YES);
            else {
                play_feedback(MP3_NO);
                if (!md5_is_past_mistake(entered_letter)) {
                    mistake_pool[md5_incorrect_tries] = entered_letter;
                    md5_incorrect_tries++;
                }
                else
                    play_feedback(MP3_YOU_HAVE_MADE_THE_SAME_MISTAKE);
            }
            md5_next_state = MD5_STATE_EVALUATE_GAME;
            break;





        case MD5_STATE_EVALUATE_GAME:
            log_msg("Entering evaluate game state.");
            if (!strncmp(input_word, md5_chosen_word, strlen(md5_chosen_word))) {
                play_feedback(MP3_YOU_HAVE_GUESSED_THE_WORD);
                play_tada();
                md5_next_state = MD5_STATE_INTRO;
            }
            else if (md5_incorrect_tries == MAX_INCORRECT_GUESS) {
                play_feedback(MP3_7_MISTAKES_YOU_MISSED);
                play_string(md5_chosen_word, strlen(md5_chosen_word));
                md5_next_state = MD5_STATE_INTRO;
            }
            else {
                if (md5_incorrect_tries > 0) {
                    play_feedback(MP3_YOU_HAVE);
                    play_number(MAX_INCORRECT_GUESS - md5_incorrect_tries);
                    if (MAX_INCORRECT_GUESS - md5_incorrect_tries == 1)
                        play_feedback(MP3_MISTAKE_REMAINING);
                    else
                        play_feedback(MP3_MISTAKES_REMAINING);
                }
                md5_next_state = MD5_STATE_ASK_FOR_GUESS;
            }
            break;

    }
}


/**
 * @brief  Set the dot the from input
 * @param this_dot the dot input
 * @return Void
 */
void md5_input_dot(char this_dot) {
    md5_last_dot = this_dot;
    log_msg("In input dot: %x", this_dot);
    play_dot(this_dot);
}

/**
 * @brief handle cell input
 * @param this_cell the cell input
 * @return Void
 */
void md5_input_cell(char this_cell) {
    if (md5_last_dot != 0) {
        log_msg("In input cell: %x", this_cell);
        last_cell = this_cell;
        got_input = true;
    }
}