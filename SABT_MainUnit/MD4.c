/**
 * @file MD4.c
 * @brief Mode 4 code - Hangman
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#include "letter_globals.h"

int md4_current_state;
char md4_last_dot, last_cell, expected_dot;

/*
 * @brief Choose random word from dictionary
 * @return char* - Random chosen word
 */
char* choose_word()
{
  return "dog";
}

/*
 * @brief Try to place global entered_letter into global input_word. 
 * @return bool - True on success (finds a match and places letter), 
 *                false failure (doesn't find a match)
 */
bool place_letter() {
    int i;
    bool found_match = false;
    
    for (i = 0; i < strlen(current_word); i++) {
        if (entered_letter == current_word[i]) {
            input_word[i] = entered_letter;
            found_match = true;
        }
    }
    
    if (found_match)
        return true;
    else
        return false;
}

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool md4_valid_letter(char button_bits){
  char letter_from_bits = get_letter_from_bits(button_bits);
  if((letter_from_bits >= 'a') && (letter_from_bits <= 'z')){
    entered_letter = letter_from_bits;
	return true;
  }
  return false;
}


/**
 * @brief  Given a char, in last_cell, play the corresponding number
 *         sound file
 * @return Void
 */
void md4_play_requested_dot(void)
{
    // This will hold formatted file to access
    char req_mp3[10];
    sprintf((char*)req_mp3, "dot_%c.MP3", md4_last_dot);
    request_to_play_mp3_file(req_mp3);
}

void md4_reset(void)
{
  md4_current_state = 0;
  md4_last_dot = 0;
}

/*
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md4_main(void)
{
  switch(md4_current_state)
  {
    case MD4_STATE_INITIAL:
      PRINTF("INITIAL\r\n");
      request_to_play_mp3_file("MD4INT.mp3");

      md4_current_state = MD4_STATE_CHOOSE_WORD;

      break;
    
    case MD4_STATE_CHOOSE_WORD:
      PRINTF("CHOOSING\r\n");
      current_word = choose_word();
      input_word = calloc(sizeof(current_word), sizeof(char));
      input_word_index = 0;
      num_mistakes = 0;
          
      md4_current_state = MD4_STATE_SAY_STATUS;
      break;
    
    case MD4_STATE_SAY_STATUS:
      PRINTF("STATUS\r\n");
      
      if (input_word[input_word_index] != 0) {
        char buf[10];
        sprintf(buf, "%c.mp3", input_word[input_word_index]);
        request_to_play_mp3_file(buf);
      } else {
        request_to_play_mp3_file("blank.mp3");
      }
    
      input_word_index++;
          
      if (input_word_index == strlen(input_word)) {
        request_to_play_mp3_file("and_mstk.mp3");
        input_word_index = 0;
        md4_current_state = MD4_STATE_SAY_MISTAKES;
      }
      
      break;

    case MD4_STATE_SAY_MISTAKES:
      PRINTF("MISTAKES\r\n");
      
      char buf[10];
      sprintf(buf, "%d_mstks.mp3", num_mistakes);
      request_to_play_mp3_file(buf);
          
      md4_current_state = MD4_STATE_WAIT_INPUT;
          
      break;
    
    case MD4_STATE_WAIT_INPUT:
      if(got_input){
          got_input = false;
          md4_current_state = MD4_STATE_PROC_INPUT;
      }
    
      break;
    
    case MD4_STATE_PROC_INPUT:
      PRINTF("PROCESS\r\n");
      // set entered_letter in valid_letter(), but return true or false
      if (md4_valid_letter(last_cell)) {
        char buf[7];
        sprintf(buf, "%c.mp3", entered_letter);
        request_to_play_mp3_file(buf);
        md4_current_state = MD4_STATE_CHECK_MATCH;
      } else {
        request_to_play_mp3_file("INVPAT.mp3");
        num_mistakes++;
        md4_current_state = MD4_STATE_SAY_STATUS;
      }
      break;
    
    case MD4_STATE_CHECK_MATCH:
      PRINTF("CHECKING\r\n");

      // place_letter() returns true if the letter guessed is found in
      // the word, false otherwise. If true, it will place the entered_letter
      // into input_word.
      if (place_letter()) {
        request_to_play_mp3_file("yes.mp3");
      } else {
        request_to_play_mp3_file("no.mp3");
        num_mistakes++;
      }
          
      md4_current_state = MD4_STATE_EVALUATE_GAME;

      break;

    case MD4_STATE_EVALUATE_GAME:
      md4_current_state = MD4_STATE_SAY_STATUS;
          
      if (!strcmp(current_word, input_word)) {
        request_to_play_mp3_file("nc_wrk.mp3");
        free(input_word);
        md4_current_state = MD4_STATE_CHOOSE_WORD;
      } else if (num_mistakes == 7) {
        request_to_play_mp3_file("you_lose.mp3");
        free(input_word);
        md4_current_state = MD4_STATE_CHOOSE_WORD;
      }
    
      break;
        
  }
}

void md4_call_mode_yes_answer(void)
{
}

void md4_call_mode_no_answer(void)
{
}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void md4_input_dot(char this_dot)
{
  md4_last_dot = this_dot;
  md4_play_requested_dot();
}

/**
 * @brief
 * @return Void
 */
void md4_input_cell(char this_cell)
{
  if(md4_last_dot != 0)
  {
    last_cell = this_cell;
    got_input = true;
  }
}
