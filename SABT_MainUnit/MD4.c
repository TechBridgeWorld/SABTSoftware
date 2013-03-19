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

/**
 * @brief Choose random word from dictionary
 * @return char* - Random chosen word
 */
char* choose_word()
{
  return "word";
}

/**
 * @brief  Given a char, in last_cell, play the corresponding number
 *         sound file
 * @return Void
 */
 /*
void play_requested_dot(void)
{
  switch(md4_last_dot)
  {
    case '1':
      request_to_play_mp3_file("MD1_1.MP3");
      break;
    case '2':
      request_to_play_mp3_file("MD1_2.MP3");
      break;
    case '3':
      request_to_play_mp3_file("MD1_3.MP3");
      break;
    case '4':
      request_to_play_mp3_file("MD1_4.MP3");
      break;
    case '5':
      request_to_play_mp3_file("MD1_5.MP3");
      break;
    case '6':
      request_to_play_mp3_file("MD1_6.MP3");
      break;
  }
}*/

void md4_reset(void)
{
  md4_current_state = 0;
  md4_last_dot = 0;
}

/**
 * @brief  Step through the main stages in the code.
 *         Note : We need 2 STATE_REQUEST_INPUT states because request_to_play_mp3_file
 *         cannot handle 2 calls in quick succession.
 * @return Void
 * @TODO record MD4INT.MP3
 */
void md4_main(void)
{
  switch(md4_current_state)
  {
    case STATE_INITIAL:
      PRINTF("INITIAL\r\n");
      request_to_play_mp3_file("MD4INT.mp3");

      md4_current_state = STATE_CHOOSE_WORD;

      break;
    
    case STATE_CHOOSE_WORD:
      PRINTF("CHOOSING\r\n");
      md4_current_state = STATE_SAY_STATUS;
      break;
    
    case STATE_SAY_STATUS:
      PRINTF("STATUS\r\n");
      md4_current_state = STATE_WAIT_INPUT;
      break;
    
    case STATE_WAIT_INPUT:
      // Progress to next state when md4_input_cell is called below
      break;
    
    case STATE_PROC_INPUT:
      PRINTF("PROCESS\r\n");
      // set entered_letter in valid_letter(), but return 1 or 0
      if (valid_letter(last_cell)) {
        char buf[16];
        sprintf(buf, "MD2_%c.mp3", entered_letter);
        //usart_transmit_string_to_pc((unsigned char*)buf);
        request_to_play_mp3_file(buf);
        md4_current_state = STATE_CHECK_MATCH;
      } else {
        request_to_play_mp3_file("INVPAT.mp3");
        md4_current_state = STATE_SAY_STATUS;
      }
      break;
    
    case STATE_CHECK_MATCH:
      PRINTF("CHECKING\r\n");


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
  play_requested_dot();
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
    md4_current_state = STATE_PROC_INPUT;
  }
}
