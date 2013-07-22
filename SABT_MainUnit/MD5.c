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
bool md5_valid_letter(char button_bits)
{
  char letter_from_bits = get_letter_from_bits(button_bits);
  if((letter_from_bits >= 'a') && (letter_from_bits <= 'z'))
  {
    entered_letter = letter_from_bits;
    return true;
  }
  return false;
}

/*
 * @brief Try to place global entered_letter into global input_word. 
 * @return bool - True on success (finds a match and places letter), 
 *                false failure (doesn't find a match)
 */
bool md5_place_letter()
{
  int i;
  bool found_match = false;

  for (i = 0; i < strlen(player1_word); i++)
  {
    if (entered_letter == player1_word[i])
    {
      input_word[i] = entered_letter;
      found_match = true;
    }
  }

  return found_match;
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
  char huff[10];
  char bufff[10];
  int i;

  switch(md5_current_state)
  {
    case MD5_STATE_INITIAL:
      request_to_play_mp3_file("MD5INT.mp3");
      md5_current_state = MD5_STATE_SETUP_VARS;
      break;

    case MD5_STATE_SETUP_VARS:
      for (i = 0; i < MAX_LEN + 1; i++)
      {
        player1_word[i] = '0';
        input_word[i] = '0';
      }
      input_word_index = 0;
      num_mistakes = 0;
      game_status = 0;
      md5_current_state = MD5_STATE_WAIT_INPUT_1;
      break;

    case MD5_STATE_WAIT_INPUT_1:
      if(got_input)
      {
        got_input = false;
        md5_current_state = MD5_STATE_PROC_INPUT_1;
      }
      break;

    case MD5_STATE_PROC_INPUT_1:
      // Enter was pressed, but nothing was entered so we check to see if it was a valid word
      if (last_cell == 0)
      {
        player1_word[input_word_index] = '\0';
        //@todo - find better way to inlude dictionary file name
        if (bin_srch_dict((unsigned char *)player1_word))
        {
          // valid word so move on to player 2's turn
          request_to_play_mp3_file("fnd_wrd.mp3"); // @TODO "valid word, please hand device to player 2 and press enter when ready"
          input_word_index = 0;
          md5_current_state = MD5_STATE_WAIT4P2;
        } else 
        {
          // word not found in dictionary, clear variables and try again
          request_to_play_mp3_file("not_fnd.mp3"); // @TODO "word not found in dictionary, please try again"

          int i;
          for (i = 0; i < MAX_LEN + 1; i++)
          {
            player1_word[i] = '0';
          }
          input_word_index = 0;

          md5_current_state = MD5_STATE_WAIT_INPUT_1;
        }
      } else if (md5_valid_letter(last_cell))
      {  // set entered_letter in valid_letter(), but return true or false
        char buff[7];
        sprintf(buff, "%c.mp3", entered_letter);
        request_to_play_mp3_file(buff);

        // reset because too many letters were input
        if (input_word_index == MAX_LEN)
        {
          int i;
          for (i = 0; i < MAX_LEN + 1; i++)
          {
            player1_word[i] = '0';
          }
          input_word_index = 0;

          md5_current_state = MD5_STATE_WAIT_INPUT_1;
          break;
        }

        player1_word[input_word_index] = entered_letter;
        input_word_index++;

        md5_current_state = MD5_STATE_WAIT_INPUT_1;
      } else
      {
        request_to_play_mp3_file("MD5_INV.mp3");  // @TODO "invalid pattern, please enter another letter"
        md5_current_state = MD5_STATE_WAIT_INPUT_1;
      }
      break;

    case MD5_STATE_WAIT4P2:
      if(got_input)
      {
        got_input = false;
        request_to_play_mp3_file("your_wrd.mp3"); // @ TODO "your word is"
        md5_current_state = MD5_STATE_SAY_STATUS;
      }
      break;

    case MD5_STATE_SAY_STATUS:
      sprintf(huff, "%s\r\n", player1_word);
      PRINTF(huff);
      if (input_word_index == strlen(player1_word))
      {
        input_word_index = 0;
        if (num_mistakes > 0)
        {
          request_to_play_mp3_file("and_mstk.mp3");
          md5_current_state = MD5_STATE_SAY_MISTAKES;
        } else
          md5_current_state = MD5_STATE_ASK_FOR_GUESS;
      } else
      {
        if (input_word[input_word_index] != '0')
        {
          char buf[10];
          sprintf(buf, "%c.mp3", input_word[input_word_index]);
          request_to_play_mp3_file(buf);
        } else
        {
          request_to_play_mp3_file("blank.mp3");
        }

        input_word_index++;
      }
      break;

    case MD5_STATE_SAY_MISTAKES:
      sprintf(bufff, "%d_mstks.mp3", num_mistakes);
      request_to_play_mp3_file(bufff);
      md5_current_state = MD5_STATE_ASK_FOR_GUESS;
      break;

    case MD5_STATE_ASK_FOR_GUESS:
      request_to_play_mp3_file("guess.mp3");
      md5_current_state = MD5_STATE_WAIT_INPUT_2;
      break;

    case MD5_STATE_WAIT_INPUT_2:
      if(got_input)
      {
        got_input = false;
        md5_current_state = MD5_STATE_PROC_INPUT_2;
      }
      break;

    case MD5_STATE_PROC_INPUT_2:
      // nothing was entered so we repeat the word
      if (last_cell == 0)
      {
        md5_current_state = MD5_STATE_EVALUATE_GAME;
      } else if (md5_valid_letter(last_cell))
      {  // set entered_letter in valid_letter(), but return true or false
        char buff[7];
        sprintf(buff, "%c.mp3", entered_letter);
        request_to_play_mp3_file(buff);
        md5_current_state = MD5_STATE_CHECK_MATCH;
      } else
      {
        request_to_play_mp3_file("INVPAT.mp3");
        num_mistakes++;
        md5_current_state = MD5_STATE_EVALUATE_GAME;
      }
      break;

    case MD5_STATE_CHECK_MATCH:
      // place_letter() returns true if the letter guessed is found in
      // the word, false otherwise. If true, it will place the entered_letter
      // into input_word.
      if (md5_place_letter())
      {
        request_to_play_mp3_file("yes.mp3");
      } else
      {
        request_to_play_mp3_file("no.mp3");
        num_mistakes++;
      }
      md5_current_state = MD5_STATE_EVALUATE_GAME;
      break;

    case MD5_STATE_EVALUATE_GAME:
      if (!strncmp(input_word, player1_word, strlen(player1_word)))
      {
        game_status = 1;
        request_to_play_mp3_file("you_win.mp3");  // "you have guessed the word!"
      } else if (num_mistakes == 7)
      {
        game_status = 1;
        request_to_play_mp3_file("you_lose.mp3"); // "you have made 7 mistakes the word you missed was"
      }
      if (game_status == 0)
      {
        request_to_play_mp3_file("so_far.mp3");
        md5_current_state = MD5_STATE_SAY_STATUS;
      } else if (game_status == 1)
      {
        md5_current_state = MD5_STATE_READ_WORD;
      }
      break;

    case MD5_STATE_READ_WORD:
      if (input_word_index == strlen(player1_word))
      {
        input_word_index = 0;
        request_to_play_mp3_file("new_game.mp3");
        md5_current_state = MD5_STATE_SETUP_VARS;
      } else
      {
        char nom[10];
        sprintf(nom, "%c.mp3", player1_word[input_word_index]);
        request_to_play_mp3_file(nom);
        input_word_index++;
      }
      break;
  }
}

/**
 * @brief Handle pressing the enter button in mode 5
 * @return Void
 */
void md5_call_mode_yes_answer(void)
{
}

/**
 * @brief Handle pressing the exit button in mode 5
 * @return Void
 */
void md5_call_mode_no_answer(void)
{
}

/**
 * @brief  Set the dot the from input
 * @param this_dot the dot input
 * @return Void
 */
void md5_input_dot(char this_dot)
{
  md5_last_dot = this_dot;
  md5_play_requested_dot();
}

/**
 * @brief handle cell input
 * @param this_cell the cell input
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
