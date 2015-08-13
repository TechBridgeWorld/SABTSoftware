/**
 * @file mode_5.c
 * @brief Mode 5 code - Hangman (two player)
 * @author Kory Stiger (kstiger) & Marjorie Carlson (marjorie)
 */

#include "globals.h"
#include "Modes.h"
#include "common.h"
#include "letter_globals.h"
#include "audio.h"
#include "mp3s.h"

#define MAX_MISTAKE             8

// State variables
static char mistake_pool[MAX_MISTAKE + 1] = "";
char entered_letter;

// Globals
char mode_5_chosen_word[MAX_WORD_LENGTH + 1];    // Holds the current word being guessed
char input_word[MAX_WORD_LENGTH + 1];         // Holds the correctly input letters
int  input_word_index;                 // Used to read out the status of the current input word
bool is_game_over;
char expected_dot;

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not. The letter is saved in
 * entered_letter.
 */

bool mode_5_valid_letter(char button_bits) {
    char letter_from_bits = get_letter_from_bits(button_bits);
    if ((letter_from_bits >= 'a') && (letter_from_bits <= 'z')) {
        entered_letter = letter_from_bits;
        return true;
    }
    log_msg("Invalid pattern.");
    return false;
}

/**
 *  Check if the user made the same mistake before
 *
 *  @param entered_letter used enterd letter
 *
 *  @return whether it's past mistake
 */
bool mode_5_is_past_mistake(char entered_letter){
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
bool mode_5_place_letter() {
  bool found_match = false;

  for (int i = 0; i < strlen(mode_5_chosen_word); i++) {
    if (entered_letter == mode_5_chosen_word[i]) {
      input_word[i] = entered_letter;
      found_match = true;
    }
  }

  return found_match;
}


void mode_5_reset(void) {
    reset_globals();
    reset_stats();
    init_char_arr(mode_5_chosen_word, MAX_WORD_LENGTH + 1);
    init_char_arr(input_word, MAX_WORD_LENGTH + 1);
    init_char_arr(mistake_pool, max_mistakes + 1);
    input_word_index = 0;
    is_game_over = false;
    max_mistakes = MAX_MISTAKE;
}

/*
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void mode_5_main(void) {
    switch(current_state) {

        case INITIAL:
/*            if (!done_rd_dict) {
                log_msg("Reading dictionary file...");
                play_direction(MP3_PLEASE_WAIT)
                init_read_dict((unsigned char *)"wordsEn.txt");
                while (!done_rd_dict)
                    read_dict_file();
            } */
            play_welcome();
            current_state = REQUEST_QUESTION;
            break;



        case REQUEST_QUESTION:
        if (got_input) {
            got_input = false;
            current_state = GENERATE_QUESTION;
        }
        break;



        case GENERATE_QUESTION:
            if (cell == 0) { // if return was entered twice
                mode_5_chosen_word[input_word_index] = '\0';

//                if (bin_srch_dict((unsigned char *)mode_5_chosen_word)) { // valid word; switch to player 2
                    // @TODO "valid word, please hand device to player 2 and press enter when ready"
//                    play_mode_audio(MP3_VALID_WORD_PASS_DEVICE);
                    input_word_index = 0;

                    play_feedback(MP3_YOUR_WORD_IS);
                    play_string(mode_5_chosen_word, strlen(mode_5_chosen_word));
                    play_mode_audio(MP3_PASS_DEVICE_PRESS_ENTER);
                    current_state = SWITCH_USERS;

   /*             }
                else { // invalid word; clear variables and try again
                    play_feedback(MP3_WORD_NOT_FOUND);
                    mode_5_reset();
                    current_state = REQUEST_QUESTION;
                } */
            }
            
            else if (mode_5_valid_letter(cell)) { // letter valid; word not complete
                log_msg("Letter %c entered.", entered_letter);
                char letter[2] = {entered_letter, '\0'};
                play_mp3(lang_fileset, letter);

                // reset because too many letters were input
                if (input_word_index == MAX_WORD_LENGTH) {
                    log_msg("Too many letters inputted!");
                    play_feedback(MP3_TOO_LONG);
                    mode_5_reset();
                    current_state = REQUEST_QUESTION;
                    break;
                }

                mode_5_chosen_word[input_word_index] = entered_letter;  // add letter to chosen_word
                input_word_index++;
                current_state = REQUEST_QUESTION;
            }
            else { // invalid letter
                play_feedback(MP3_INVALID_PATTERN);
                current_state = REQUEST_QUESTION;
            }
            break;



        case SWITCH_USERS:
            if (got_input) {
                got_input = false;
                current_state = PROMPT;
            }
            break;


        case PROMPT:
            log_msg("Entering ask for guess state.");
            play_direction(MP3_PLAYER_2);
            play_direction(MP3_YOUR_WORD_IS_NOW);
            play_string(input_word, strlen(mode_5_chosen_word));
            play_direction(MP3_GUESS_A_LETTER);
            current_state = GET_INPUT;
            break;




        case GET_INPUT:
            if (got_input) {
                got_input = false;
                current_state = PROCESS_ANSWER;
            }
            break;




        case PROCESS_ANSWER:
            log_msg("Entering checkans state.");
            if (cell == 0) // nothing entered: repeat word
                current_state = GAME_OVER;

            else if (mode_5_valid_letter(cell)) { // valid letter: read aloud
                char buff[7];
                sprintf(buff, "%c", entered_letter);
                play_mp3(lang_fileset, buff);  //@todo fix this
                current_state = CHECK_ANSWER;
            }
            else {
                play_feedback(MP3_INVALID_PATTERN);
                mistakes++;
                current_state = GAME_OVER;
            }
            break;





        case CHECK_ANSWER:
            log_msg("Entering check match state.");
            if (mode_5_place_letter()) // letters is in word; fn places it
                play_feedback(MP3_YES);
            else {
                play_feedback(MP3_NO);
                if (!mode_5_is_past_mistake(entered_letter)) {
                    mistake_pool[mistakes] = entered_letter;
                    mistakes++;
                }
                else
                    play_feedback(MP3_YOU_HAVE_MADE_THE_SAME_MISTAKE);
            }
            current_state = GAME_OVER;
            break;





        case GAME_OVER:
            log_msg("Entering evaluate game state.");
            if (!strncmp(input_word, mode_5_chosen_word, strlen(mode_5_chosen_word))) {
                play_feedback(MP3_YOU_HAVE_GUESSED_THE_WORD);
                play_tada();
                current_state = INITIAL;
            }
            else if (mistakes == max_mistakes) {
                play_feedback(MP3_7_MISTAKES_YOU_MISSED);
                play_string(mode_5_chosen_word, strlen(mode_5_chosen_word));
                current_state = INITIAL;
            }
            else {
                if (mistakes > 0) {
                    play_feedback(MP3_YOU_HAVE);
                    play_number(max_mistakes - mistakes);
                    if (max_mistakes - mistakes == 1)
                        play_feedback(MP3_MISTAKE_REMAINING);
                    else
                        play_feedback(MP3_MISTAKES_REMAINING);
                }
                current_state = PROMPT;
            }
            break;

        default:
            log_msg("Invalid state_t %d", current_state);
            quit_mode();
            break;

    }
}


/**
 * @brief  Set the dot the from input
 * @param this_dot the dot input
 * @return Void
 */
void mode_5_input_dot(char this_dot) {
    last_dot = this_dot;
    log_msg("In input dot: %x", this_dot);
    play_dot(this_dot);
}

/**
 * @brief handle cell input
 * @param this_cell the cell input
 * @return Void
 */
void mode_5_input_cell(char this_cell) {
    if (last_dot != 0) {
        log_msg("In input cell: %x", this_cell);
        cell = this_cell;
        got_input = true;
    }
}