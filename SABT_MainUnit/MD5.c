/**
 * @file MD5.c
 * @brief Mode 5 code - Hangman (two player)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#include "letter_globals.h"
#include "audio.h"

#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD5_"

#define MP3_INTRO "INT"
#define MP3_FOUND_WORD "FWRD"
#define MP3_NOT_FOUND "NFND"
#define MP3_INVALID "INV"
#define MP3_YOUR_WORD "YWRD"
#define MP3_AND_MISTAKES "AMSK"
#define MP3_BLANK "BLNK"
#define MP3_MISTAKES "MSTK"
#define MP3_GUESS "GAL"
#define MP3_INVALID_PATTERN "INVP"
#define MP3_YOU_LOSE "YOLO"
#define MP3_YOU_WIN "YOWI"
#define MP3_SO_FAR "SOFA"
#define MP3_NEW_GAME "NGAM"

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
  sprintf((char*)req_mp3, "DOT%c", md5_last_dot);
  play_mp3(LANG_FILESET,req_mp3);
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
      play_mp3(MODE_FILESET,MP3_INTRO);
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
          play_mp3(MODE_FILESET, MP3_FOUND_WORD); // @TODO "valid word, please hand device to player 2 and press enter when ready"
          input_word_index = 0;
          md5_current_state = MD5_STATE_WAIT4P2;
        } else 
        {
          // word not found in dictionary, clear variables and try again
          play_mp3(MODE_FILESET, MP3_NOT_FOUND); // @TODO "word not found in dictionary, please try again"

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
        sprintf(buff, "%c", entered_letter);
        play_mp3(LANG_FILESET,buff);

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
        play_mp3(MODE_FILESET, MP3_INVALID);  // @TODO "invalid pattern, please enter another letter"
        md5_current_state = MD5_STATE_WAIT_INPUT_1;
      }
      break;

    case MD5_STATE_WAIT4P2:
      if(got_input)
      {
        got_input = false;
        play_mp3(MODE_FILESET, MP3_YOUR_WORD); // @ TODO "your word is"
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
          play_mp3(MODE_FILESET, MP3_AND_MISTAKES);
          md5_current_state = MD5_STATE_SAY_MISTAKES;
        } else
          md5_current_state = MD5_STATE_ASK_FOR_GUESS;
      } else
      {
        if (input_word[input_word_index] != '0')
        {
          char buf[10];
          sprintf(buf, "%c", input_word[input_word_index]);
          play_mp3(LANG_FILESET,buf);
        } else
        {
          play_mp3(LANG_FILESET, MP3_BLANK);
        }

        input_word_index++;
      }
      break;

    case MD5_STATE_SAY_MISTAKES:
      sprintf(bufff, "#%d", num_mistakes);
      play_mp3(LANG_FILESET,bufff);
      play_mp3(MODE_FILESET,MP3_MISTAKES);
      md5_current_state = MD5_STATE_ASK_FOR_GUESS;
      break;

    case MD5_STATE_ASK_FOR_GUESS:
      play_mp3(MODE_FILESET, MP3_GUESS);
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
        sprintf(buff, "%c", entered_letter);
        play_mp3(LANG_FILESET,buff);
        md5_current_state = MD5_STATE_CHECK_MATCH;
      } else
      {
        play_mp3(LANG_FILESET, MP3_INVALID_PATTERN);
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
        play_mp3("","yes");
      } else
      {
        play_mp3("","no");
        num_mistakes++;
      }
      md5_current_state = MD5_STATE_EVALUATE_GAME;
      break;

    case MD5_STATE_EVALUATE_GAME:
      if (!strncmp(input_word, player1_word, strlen(player1_word)))
      {
        game_status = 1;
        play_mp3(MODE_FILESET, MP3_YOU_WIN);  // "you have guessed the word!"
      } else if (num_mistakes == 7)
      {
        game_status = 1;
        play_mp3(MODE_FILESET, MP3_YOU_LOSE); // "you have made 7 mistakes the word you missed was"
      }
      if (game_status == 0)
      {
        play_mp3(MODE_FILESET, MP3_SO_FAR);
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
        play_mp3(MODE_FILESET, MP3_NEW_GAME);
        md5_current_state = MD5_STATE_SETUP_VARS;
      } else
      {
        char nom[10];
        sprintf(nom, "%c", player1_word[input_word_index]);
        play_mp3(LANG_FILESET,nom);
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
