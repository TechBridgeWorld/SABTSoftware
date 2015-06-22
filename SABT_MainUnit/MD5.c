/**
 * @file MD5.c
 * @brief Mode 5 code - Hangman (two player)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#include "letter_globals.h"
#include "audio.h"

  // State definitions
#define MD5_STATE_NULL            0
#define MD5_STATE_INTRO           1          // Just started, load dictionary if needed
#define MD5_STATE_INPUT_SOLUTION      2          // Player 1 enters word
#define MD5_STATE_PROC_INPUT_1      3          // Check for valid input word
#define MD5_STATE_WAIT4P2           4          // Wait for transition to player2 to press enter
#define MD5_STATE_PROMPT            5          // Read out current word
#define MD5_STATE_SAY_MISTAKES      6          // Read out current mistakes
#define MD5_STATE_ASK_FOR_GUESS     7          // Prompt user for input
#define MD5_STATE_INPUT_GUESS      8          // Waiting for user input
#define MD5_STATE_CHECKANS          9          // Process user input
#define MD5_STATE_CHECK_MATCH       10         // Valid input was entered
#define MD5_STATE_EVALUATE_GAME     11         // Check for win or lose
#define MD5_STATE_READ_WORD         12         // User has finished the game
#define MD5_STATE_PLAY_INTRO        13         // Plays "Welcome..." intro
#define MD5_STATE_READ_DICT         14         // Load dictionary at beginning

#define MP3_INTRO "INT"
#define MP3_WAIT "WAIT"
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

#define PRIME                       53   
#define MAX_LEN                     10

#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD5_"

// State variables
static char md5_next_state = MD5_STATE_NULL;
static char md5_last_dot = NO_DOTS;
//static bool md5_input_ready = false;
//static bool md5_input_valid = false;
static int md5_incorrect_tries = 0;

// Globals

char* item;
char chosen_word[MAX_LEN + 1];       // Holds the current word being guessed
char input_word[MAX_LEN + 1];         // Holds the correctly input letters
int input_word_index;                 // Used to read out the status of the current input word
bool is_game_over;

char last_cell, expected_dot;

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool md5_valid_letter(char button_bits) {
  char letter_from_bits = get_letter_from_bits(button_bits);
  if((letter_from_bits >= 'a') && (letter_from_bits <= 'z'))
  {
    entered_letter = letter_from_bits;
    return true;
  }
  return false;
}

void md5_init_char_arr(char* arr, int len){
    for (int i = 0; i < len; i++){
        arr[i] = '\0';
    }
}

/*
 * @brief Try to place global entered_letter into global input_word. 
 * @return bool - True on success (finds a match and places letter), 
 *                false failure (doesn't find a match)
 */
bool md5_place_letter() {
  bool found_match = false;

  for (int i = 0; i < strlen(chosen_word); i++) {
    if (entered_letter == chosen_word[i]) {
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
void md5_play_requested_dot(void) {
  // This will hold formatted file to access
  char req_mp3[10];
  sprintf((char*)req_mp3, "DOT%c", md5_last_dot);
  play_mp3(LANG_FILESET,req_mp3);
}

void md5_reset(void) {
  md5_init_char_arr(chosen_word, MAX_LEN + 1);
  md5_init_char_arr(input_word,  MAX_LEN + 1);
  input_word_index = 0;
  md5_incorrect_tries = 0;
  is_game_over = false;
  md5_next_state = MD5_STATE_NULL;
  md5_last_dot = 0;
}

/*
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md5_main(void) {
  char huff[10];
  char bufff[10];

  switch(md5_next_state) {
    case MD5_STATE_INTRO:   // load dictionary, welcome players
      if (!done_rd_dict) {
        PRINTF("Reading dictionary file...");
        play_mp3(MODE_FILESET, MP3_WAIT);
        init_read_dict((unsigned char *)"wordsEn.txt");
        while(!done_rd_dict)
        read_dict_file();
      }

      play_mp3(MODE_FILESET,MP3_INTRO);
      md5_reset();
      md5_next_state = MD5_STATE_INPUT_SOLUTION;
      break;

    case MD5_STATE_INPUT_SOLUTION:
      if(got_input)
      {
        got_input = false;
        md5_next_state = MD5_STATE_PROC_INPUT_1;
      }
      break;

    case MD5_STATE_PROC_INPUT_1:
      // Enter was pressed, but nothing was entered so we check to see if it was a valid word
      if (last_cell == 0)
      {
        chosen_word[input_word_index] = '\0';
        //@todo - find better way to inlude dictionary file name
        if (bin_srch_dict((unsigned char *)chosen_word))
        {
          // valid word so move on to player 2's turn
          play_mp3(MODE_FILESET, MP3_FOUND_WORD); // @TODO "valid word, please hand device to player 2 and press enter when ready"
          input_word_index = 0;
          md5_next_state = MD5_STATE_WAIT4P2;
        } else 
        {
          // word not found in dictionary, clear variables and try again
          play_mp3(MODE_FILESET, MP3_NOT_FOUND); // @TODO "word not found in dictionary, please try again"

          int i;
          for (i = 0; i < MAX_LEN + 1; i++)
          {
            chosen_word[i] = '0';
          }
          input_word_index = 0;

          md5_next_state = MD5_STATE_INPUT_SOLUTION;
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
            chosen_word[i] = '0';
          }
          input_word_index = 0;

          md5_next_state = MD5_STATE_INPUT_SOLUTION;
          break;
        }

        chosen_word[input_word_index] = entered_letter;
        input_word_index++;

        md5_next_state = MD5_STATE_INPUT_SOLUTION;
      } else
      {
        play_mp3(MODE_FILESET, MP3_INVALID);  // @TODO "invalid pattern, please enter another letter"
        md5_next_state = MD5_STATE_INPUT_SOLUTION;
      }
      break;

    case MD5_STATE_WAIT4P2:
      if(got_input)
      {
        got_input = false;
        play_mp3(MODE_FILESET, MP3_YOUR_WORD); // @ TODO "your word is"
        md5_next_state = MD5_STATE_PROMPT;
      }
      break;

    case MD5_STATE_PROMPT:
      sprintf(huff, "%s\r\n", chosen_word);
      PRINTF(huff);
      if (input_word_index == strlen(chosen_word))
      {
        input_word_index = 0;
        if (md5_incorrect_tries > 0)
        {
          play_mp3(MODE_FILESET, MP3_AND_MISTAKES);
          md5_next_state = MD5_STATE_SAY_MISTAKES;
        } else
          md5_next_state = MD5_STATE_ASK_FOR_GUESS;
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
      sprintf(bufff, "#%d", md5_incorrect_tries);
      play_mp3(LANG_FILESET,bufff);
      play_mp3(MODE_FILESET,MP3_MISTAKES);
      md5_next_state = MD5_STATE_ASK_FOR_GUESS;
      break;

    case MD5_STATE_ASK_FOR_GUESS:
      play_mp3(MODE_FILESET, MP3_GUESS);
      md5_next_state = MD5_STATE_INPUT_GUESS;
      break;

    case MD5_STATE_INPUT_GUESS:
      if(got_input)
      {
        got_input = false;
        md5_next_state = MD5_STATE_CHECKANS;
      }
      break;

    case MD5_STATE_CHECKANS:
      // nothing was entered so we repeat the word
      if (last_cell == 0)
      {
        md5_next_state = MD5_STATE_EVALUATE_GAME;
      } else if (md5_valid_letter(last_cell))
      {  // set entered_letter in valid_letter(), but return true or false
        char buff[7];
        sprintf(buff, "%c", entered_letter);
        play_mp3(LANG_FILESET,buff);
        md5_next_state = MD5_STATE_CHECK_MATCH;
      } else
      {
        play_mp3(LANG_FILESET, MP3_INVALID_PATTERN);
        md5_incorrect_tries++;
        md5_next_state = MD5_STATE_EVALUATE_GAME;
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
        md5_incorrect_tries++;
      }
      md5_next_state = MD5_STATE_EVALUATE_GAME;
      break;

    case MD5_STATE_EVALUATE_GAME:
      if (!strncmp(input_word, chosen_word, strlen(chosen_word)))
      {
        is_game_over = true;
        play_mp3(MODE_FILESET, MP3_YOU_WIN);  // "you have guessed the word!"
      } else if (md5_incorrect_tries == 7)
      {
        is_game_over = true;
        play_mp3(MODE_FILESET, MP3_YOU_LOSE); // "you have made 7 mistakes the word you missed was"
      }
      if (is_game_over == false)
      {
        play_mp3(MODE_FILESET, MP3_SO_FAR);
        md5_next_state = MD5_STATE_PROMPT;
      } else if (is_game_over == true)
      {
        md5_next_state = MD5_STATE_READ_WORD;
      }
      break;

    case MD5_STATE_READ_WORD:
      if (input_word_index == strlen(chosen_word))
      {
        input_word_index = 0;
        play_mp3(MODE_FILESET, MP3_NEW_GAME);
        md5_reset();
        md5_next_state = MD5_STATE_INPUT_SOLUTION;
      } else
      {
        char nom[10];
        sprintf(nom, "%c", chosen_word[input_word_index]);
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
