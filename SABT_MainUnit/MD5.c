/**
 * @file MD5.c
 * @brief Mode 5 code - Hangman (two player)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#include "letter_globals.h"

int md5_current_state;
char md5_last_dot, last_cell, expected_dot;


/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool md5_valid_letter(char button_bits){
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
void md5_play_requested_dot(void)
{
    // This will hold formatted file to access
    char req_mp3[10];
    sprintf((char*)req_mp3, "dot_%c.MP3", md5_last_dot);
    request_to_play_mp3_file(req_mp3);
}

void md5_reset(void)
{
  md5_current_state = 0;
  md5_last_dot = 0;
}

/*
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md5_main(void)
{
  switch(md5_current_state)
  {
    case MD5_STATE_INITIAL:
      PRINTF("INITIAL\r\n");
      request_to_play_mp3_file("MD4INT.mp3");    // @TODO "you have chosen 2 player hangman, player 1 please enter a word"

      md5_current_state = MD5_STATE_PROC_INPUT_1;
      //md5_current_state = MD5_STATE_CHOOSE_WORD;

      break;
    
    case MD5_STATE_WAIT_INPUT_1:
      //PRINTF("PLAYER1\r\n");
      
	  if(got_input){
          got_input = false;
          md5_current_state = MD5_STATE_PROC_INPUT_1;
      }
    
      break;

	case MD5_STATE_PROC_INPUT_1:
      PRINTF("CHOOSING\r\n");
      
      // Enter was pressed, but nothing was entered so we check to see if it was a valid word
      if (last_cell == 0) {
	    player1_word[input_word_index] = '\0';

        //@todo - find better way to inlude dictionary file name
	    if (bin_srch_dict(player1_word)) {
		  // valid word so move on to player 2's turn
          request_to_play_mp3_file("fnd_wrd.mp3"); // @TODO "valid word, please hand device to player 2 and press enter when ready"
		  md5_current_state = MD5_STATE_WAIT4P2;
		} else {
		  // word not found in dictionary, clear variables and try again
          request_to_play_mp3_file("not_fnd.mp3"); // @TODO "word not found in dictionary, please try again"

          int i;
	      for (i = 0; i < MAX_LEN + 1; i++) {
            player1_word[i] = '0';
	      }
		  input_word_index = 0;

          md5_current_state = MD5_STATE_WAIT_INPUT_1;
		}
      } else if (md5_valid_letter(last_cell)) {  // set entered_letter in valid_letter(), but return true or false
        char buff[7];
        sprintf(buff, "%c.mp3", entered_letter);
        request_to_play_mp3_file(buff);

        if (input_word_index == MAX_LEN) {
          int i;
	      for (i = 0; i < MAX_LEN + 1; i++) {
            player1_word[i] = '0';
	      }
		  input_word_index = 0;

          md5_current_state = MD5_STATE_WAIT_INPUT_1;
		  break;
		}

        player1_word[input_word_index] = entered_letter;
		input_word_index++;

        md5_current_state = MD5_STATE_WAIT_INPUT_1;
      } else {
        request_to_play_mp3_file("MD5_INV.mp3");  // @TODO "invalid pattern, please enter another letter"
		md5_current_state = MD5_STATE_WAIT_INPUT_1;
	  }

      break;

    case MD5_STATE_WAIT4P2:

      if(got_input){
          got_input = false;

          request_to_play_mp3_file("trans2.mp3"); // @ TODO "your word is"

          md5_current_state = MD5_STATE_SAY_STATUS;
      }
    
	  break;

    case MD5_STATE_SAY_STATUS:
      PRINTF("STATUS\r\n");	  
          
      if (input_word_index == strlen(current_word)) {
	    PRINTF("DONE HERE DONE HERE DONE HERE\r\n");
        input_word_index = 0;
		if (num_mistakes > 0) {
          request_to_play_mp3_file("and_mstk.mp3");
          md5_current_state = MD5_STATE_SAY_MISTAKES;
		  //alex changed
        } else
          md5_current_state = MD5_STATE_ASK_FOR_GUESS;
      } else {
        if (input_word[input_word_index] != '0') {
	      char buf[10];
          sprintf(buf, "%c.mp3", input_word[input_word_index]);
          request_to_play_mp3_file(buf);
        } else {
          request_to_play_mp3_file("blank.mp3");
        }
    
        input_word_index++;
	  }
      
      break;

    case MD5_STATE_SAY_MISTAKES:
      PRINTF("MISTAKES\r\n");
      
	  char bufff[10];
      sprintf(bufff, "%d_mstks.mp3", num_mistakes);
      request_to_play_mp3_file(bufff);
      //alex changed
      md5_current_state = MD5_STATE_ASK_FOR_GUESS;
          
      break;

    case MD5_STATE_ASK_FOR_GUESS:
      request_to_play_mp3_file("guess.mp3");
	  //alex changed
      md5_current_state = MD5_STATE_WAIT_INPUT_2;

	  break;

    case MD5_STATE_WAIT_INPUT_2:
      if(got_input){
          got_input = false;
		  //alex changed
          md5_current_state = MD5_STATE_PROC_INPUT_2;
      }
    
      break;
    
    case MD5_STATE_PROC_INPUT_2:
      PRINTF("PROCESS2\r\n");

      // nothing was entered so we repeat the word
      if (!last_cell) {
	    md5_current_state = MD5_STATE_EVALUATE_GAME;
      } else if (md5_valid_letter(last_cell)) {  // set entered_letter in valid_letter(), but return true or false
        char buff[7];
        sprintf(buff, "%c.mp3", entered_letter);
        request_to_play_mp3_file(buff);
        md5_current_state = MD5_STATE_CHECK_MATCH;
      } else {
        request_to_play_mp3_file("INVPAT.mp3");
        num_mistakes++;
        md5_current_state = MD5_STATE_EVALUATE_GAME;
      }

      break;
    
    case MD5_STATE_CHECK_MATCH:
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
          
      md5_current_state = MD5_STATE_EVALUATE_GAME;

      break;

    case MD5_STATE_EVALUATE_GAME:       
	  PRINTF("EVAL\r\n");   

      if (!strncmp(input_word, current_word, strlen(current_word))) {
	    game_status = 1;
        request_to_play_mp3_file("you_win.mp3");  // "you have guessed the word!"
      } else if (num_mistakes == 7) {
	    game_status = 1;
        request_to_play_mp3_file("you_lose.mp3"); // "you have made 7 mistakes the word you missed was"
      }
    
	  if (game_status == 0) {
	    request_to_play_mp3_file("so_far.mp3");
        md5_current_state = MD5_STATE_SAY_STATUS;
	  } else if (game_status == 1) {
        md5_current_state = MD5_STATE_READ_WORD;
	  }

      break;   

    case MD5_STATE_READ_WORD:
      PRINTF("READ\r\n");
          
      if (input_word_index == strlen(current_word)) {
        input_word_index = 0;
		request_to_play_mp3_file("new_word.mp3");
		//alex changed - need to just reset and ask for more input
        md5_current_state = MD5_STATE_INITIAL;
      } else {
        char nom[10];
        sprintf(nom, "%c.mp3", current_word[input_word_index]);
        request_to_play_mp3_file(nom);

        input_word_index++;
	  }

	  break;
  }
}

void md5_call_mode_yes_answer(void)
{
}

void md5_call_mode_no_answer(void)
{
}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void md5_input_dot(char this_dot)
{
  md5_last_dot = this_dot;
  md5_play_requested_dot();
}

/**
 * @brief
 * @return Void
 */
void md5_input_cell(char this_cell)
{
  if(md5_last_dot != 0)
  {
    last_cell = this_cell;
    got_input = true;
  }
}
