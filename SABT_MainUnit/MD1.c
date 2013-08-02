/**
 * @file MD1.c
 * @brief Mode 1 code - Dot Practice
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

// MP3 prompts
#define MODE_FILESET "MD1_"
#define LANG_FILESET "ENG_"
#define MP3_INTRODUCTION "INT"
#define MP3_FIND_DOT "FNDT"

#include "Globals.h"
#include "MD1.h"
#include "audio.h"
#include "script_common.h"
#include "script_english.h"
#include "common.h"

static char last_dot;                   // char representing last big dot pressed
char used_number[6] = {0, 0, 0, 0, 0, 0};

/**
 * @brief based off of the internal timer (TCNT1) - we generate
 *        a psuedo-random number. Turn that into a number from 1 - 6
 *        return the charachter of that number;
 * @return char - charachter representation of a number from 1 - 6
 */
char random_number_as_char()
{
  int num = TCNT1;
  int i;

  num *= PRIME;
  num = (abs(num) % 6);

  //while you are looking at a full section of the array
  while(used_number[num])
  {
    num = TCNT1;
    num *= PRIME;
    num = (abs(num) % 6);
  }

  used_number[num] = 1;
  used_num_cnt ++;

  //if you find that you have used all of the letters, clear both the array and the count
  if(used_num_cnt == NUM_DOTS)
  {
    for(i = 0; i < NUM_DOTS; i ++)
      used_number[i] = 0;
    used_num_cnt = 0;
  }

  //change from range 0-5 to 1-6
  num += 1;

  // Return the number as a character
  if (num >= 1 && num <= 6) return '0' + num;

  // Default behavior, could not generate a valid char
  return '0';
}

/**
 * @brief Reset mode 1 to starting state
 *        Should be useful for error handling
 * @return Void
 */
void md1_reset(void)
{
  set_mode_globals(&script_english, "ENG_", "MD1_");
  current_state = STATE_INITIAL;
}

/**
 * @brief main code for Mode 1
 * Prompts the user to press a certain random dot, gives feedback based on
 * whether they pressed the correct dot
 * @return Void
 */
void md1_main(void)
{
  switch(current_state)
  {
    case STATE_INITIAL:
      PRINTF("[MD1] Entering MD1\n\r");
      used_num_cnt = 0;
      // Play the introductory message for Mode 1
      play_mp3(MODE_FILESET,MP3_INTRODUCTION);
      current_state = STATE_REQUEST_INPUT1;
      break;
    case STATE_REQUEST_INPUT1:
      play_mp3(MODE_FILESET,MP3_FIND_DOT);
      expected_dot = random_number_as_char();
      current_state = STATE_REQUEST_INPUT2;
      break;
    case STATE_REQUEST_INPUT2:
      // Generate a random char from '1' to '6'
      play_dot(expected_dot);
      current_state = STATE_WAIT_INPUT;
      break;
    case STATE_WAIT_INPUT:
      if(last_dot != 0)
        current_state = STATE_PROC_INPUT;
      break;
    case STATE_PROC_INPUT:
      if(last_dot != expected_dot)
      {
        play_mp3(LANG_FILESET,MP3_INCORRECT);
        play_mp3(MODE_FILESET, MP3_FIND_DOT);
        play_dot(expected_dot);
        last_dot = 0;
        current_state = STATE_WAIT_INPUT;
      }
      else
      {
        play_mp3(LANG_FILESET,MP3_CORRECT);
        play_mp3(SYS_FILESET, MP3_TADA);
        last_dot = 0;
        current_state = STATE_REQUEST_INPUT1;
      }
      break;
    default:
      break;
  }
}

/**
 * @brief in mode 1, enter button just replays the current prompt
 * @return Void
 */
void md1_call_mode_yes_answer(void)
{
  play_mp3(MODE_FILESET,MP3_FIND_DOT);
  current_state = STATE_REQUEST_INPUT2;
}

void md1_call_mode_no_answer(void) {}

/**
 * @brief register dot input
 *        Sets the program to STATE_PROC_INPUT
 * @param this_dot the dot being input
 * @return Void
 */
void md1_input_dot(char this_dot)
{
  last_dot = this_dot;
  current_state = STATE_PROC_INPUT;
}

/**
 * @brief register cell input
 * @param this_cell the cell being input
 * @return void
 */
void md1_input_cell(char this_cell) {}
