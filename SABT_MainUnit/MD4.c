/**
 * @file MD4.c
 * @brief Mode 4 code - Hangman (single player)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#include "letter_globals.h"

int md4_current_state;
char md4_last_dot, last_cell, expected_dot;

char *item_list[11] = {"rock", "tree", "sky", "apple", "car", "dog",
  "cat", "water", "floor", "leaf", "road"};

int items_used_list[11] = {0,0,0,0,0,0,0,0,0,0,0};

/*
 * @brief Choose random word from item list
 * @return char* - Random chosen word
 */
int choose_word()
{
  int num = TCNT1;
  int i;

  num *= PRIME;
  num = (abs(num) % 11);

  char buf[10];
  sprintf(buf, "num=%i\r\n", num);
  PRINTF(buf);

  TX_NEWLINE_PC;
  sprintf(buf, "cnt=%i", items_used + 1);
  PRINTF(buf);
  TX_NEWLINE_PC;

  // increment items_used until we've used all 11 items then reset everything
  items_used++;
  if (items_used == 11)
  {
    items_used = 0;
    for (i = 0; i < 11; i++)
      items_used_list[i] = 0;
  }

  return num;
}

/*
 * @brief Try to place global entered_letter into global input_word. 
 * @return bool - True on success (finds a match and places letter), 
 *                false failure (doesn't find a match)
 */
bool place_letter()
{
  int i;
  bool found_match = false;

  for (i = 0; i < strlen(current_word); i++)
  {
    if (entered_letter == current_word[i])
    {
      input_word[i] = entered_letter;
      found_match = true;
    }
  }

  return found_match;
}

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool md4_valid_letter(char button_bits)
{
  char letter_from_bits = get_letter_from_bits(button_bits);
  if((letter_from_bits >= 'a') && (letter_from_bits <= 'z'))
  {
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
  char bufff[10];
  switch(md4_current_state)
  {
    case MD4_STATE_INITIAL:
      request_to_play_mp3_file("MD4INT.mp3");
      md4_current_state = MD4_STATE_CHOOSE_WORD;
      break;

    case MD4_STATE_CHOOSE_WORD:
      current_word = item_list[choose_word()];
      input_word_index = 0;
      num_mistakes = 0;
      game_status = 0;

      int i;
      for (i = 0; i < 11; i++) {
        input_word[i] = '0';
      }

      md4_current_state = MD4_STATE_SAY_STATUS;
      break;

    case MD4_STATE_SAY_STATUS:
      if (input_word_index == strlen(current_word))
      {
        input_word_index = 0;
        if (num_mistakes > 0)
        {
          request_to_play_mp3_file("and_mstk.mp3");
          md4_current_state = MD4_STATE_SAY_MISTAKES;
        } else
          md4_current_state = MD4_STATE_ASK_FOR_GUESS;
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

    case MD4_STATE_SAY_MISTAKES:
      sprintf(bufff, "%d_mstks.mp3", num_mistakes);
      request_to_play_mp3_file(bufff);
      md4_current_state = MD4_STATE_ASK_FOR_GUESS;
      break;

    case MD4_STATE_ASK_FOR_GUESS:
      request_to_play_mp3_file("guess.mp3");
      md4_current_state = MD4_STATE_WAIT_INPUT;
      break;

    case MD4_STATE_WAIT_INPUT:
      if(got_input)
      {
        got_input = false;
        md4_current_state = MD4_STATE_PROC_INPUT;
      }
      break;

    case MD4_STATE_PROC_INPUT:
      // nothing was entered so we repeat the word
      if (last_cell == 0)
      {
        md4_current_state = MD4_STATE_EVALUATE_GAME;
      } else if (md4_valid_letter(last_cell))
      { // set entered_letter in valid_letter(), but return true or false
        char buff[7];
        sprintf(buff, "%c.mp3", entered_letter);
        request_to_play_mp3_file(buff);
        md4_current_state = MD4_STATE_CHECK_MATCH;
      } else
      {
        request_to_play_mp3_file("INVPAT.mp3");
        num_mistakes++;
        md4_current_state = MD4_STATE_EVALUATE_GAME;
      }
      break;

    case MD4_STATE_CHECK_MATCH:
      // place_letter() returns true if the letter guessed is found in
      // the word, false otherwise. If true, it will place the entered_letter
      // into input_word.
      if (place_letter())
      {
        request_to_play_mp3_file("yes.mp3");
      } else
      {
        request_to_play_mp3_file("no.mp3");
        num_mistakes++;
      }

      md4_current_state = MD4_STATE_EVALUATE_GAME;
      break;

    case MD4_STATE_EVALUATE_GAME:
      if (!strncmp(input_word, current_word, strlen(current_word)))
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
        md4_current_state = MD4_STATE_SAY_STATUS;
      } else if (game_status == 1)
      {
        md4_current_state = MD4_STATE_READ_WORD;
      }
      break;

    case MD4_STATE_READ_WORD:
      if (input_word_index == strlen(current_word))
      {
        input_word_index = 0;
        request_to_play_mp3_file("new_word.mp3");
        md4_current_state = MD4_STATE_CHOOSE_WORD;
      } else
      {
        char nom[10];
        sprintf(nom, "%c.mp3", current_word[input_word_index]);
        request_to_play_mp3_file(nom);
        input_word_index++;
      }
      break;
  }
}

/**
 * @brief Handle pressing enter in mode 4
 * @return Void
 */
void md4_call_mode_yes_answer(void)
{
}

/**
 * @brief Handle pressing exit in mode 4
 * @return Void
 */
void md4_call_mode_no_answer(void)
{
}

/**
 * @brief  Set the dot the from input
 * @param this_dot the input dot
 * @return Void
 */
void md4_input_dot(char this_dot)
{
  md4_last_dot = this_dot;
  md4_play_requested_dot();
}

/**
 * @brief handle cell input
 * @param this_cell the input cell
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
