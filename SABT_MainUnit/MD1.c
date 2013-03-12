/**
 * @file MD1.c
 * @brief code for the Primary User Interface Mode
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "MD1.h"

/**
 * @brief based off of the internal timer (TCNT1) - we generate
 *        a psuedo-random number. Turn that into a number from 1 - 6
 *        return the charachter of that number;
 * @return char - charachter representation of a number from 1 - 6
 */
char random_number_as_char()
{
  int num = TCNT1;
  
  num *= PRIME;
  num = (abs(num) % 6) + 1;

  // Return the number as a character
  if (num >= 1 && num <= 6) return '0' + num;
  
  // Default behavior, could not generate a valid char
  return '0';
}

/**
 * @brief echo the number of the dot pressed
 * @param last_dot - unsigned char.   Which dot to play 
 * @return Void
 */
void play_requested_dot(unsigned char last_dot)
{
  char buf[10];

  // Check for validity of number
  if (last_dot >= '1' && last_dot <= '6')
    sprintf(buf, "MD1_%c.MP3", last_dot);

  // TODO default case?
  // TODO(nick) make sure this works
  /*switch(last_dot)
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
  }*/
}

/**
 * @brief Reset mode 1 to starting state
 *        Should be useful for error handling
 * @return Void
 */
void md1_reset(void)
{
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

      // Play the introductory message for Mode 1
      request_to_play_mp3_file("MD1INT.MP3");
      current_state = STATE_REQUEST_INPUT1;
      break;
    case STATE_REQUEST_INPUT1:
      request_to_play_mp3_file("find_dot.MP3");
      current_state = STATE_REQUEST_INPUT2;
      break;
    case STATE_REQUEST_INPUT2:
      // Generate a random char from '1' to '6'
      expected_dot = random_number_as_char();
      play_requested_dot(expected_dot);
      current_state = STATE_WAIT_INPUT;
      break;
    case STATE_WAIT_INPUT:
      if(last_dot != 0)
        current_state = STATE_PROC_INPUT;
      break;
    case STATE_PROC_INPUT:
      if(last_dot != expected_dot)
      {
        request_to_play_mp3_file("no.MP3");
        last_dot = 0;
        current_state = STATE_WAIT_INPUT;
      }
      else
      {
        request_to_play_mp3_file("good.MP3");
        last_dot = 0;
        current_state = STATE_REQUEST_INPUT1;
      }      
      break;
    default:
      break;
  }
}

void md1_call_mode_yes_answer(void) {}

void md1_call_mode_no_answer(void) {}

/**
 * @brief register dot input
 *        Sets the program to STATE_PROC_INPUT
 * @param thisDot the dot being input
 * @return Void
 */
void md1_input_dot(char this_dot)
{
  last_dot = this_dot;
  current_state = STATE_PROC_INPUT;
}

void md1_input_cell(char this_cell) {}
