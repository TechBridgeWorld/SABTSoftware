/**
 * @file MD1.c
 * @brief Mode 1 code - Dot Practice
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "MD1.h"

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

//while you are looking at a full section of teh array
  while(used_number[num]){
    num = TCNT1;
    num *= PRIME;
    num = (abs(num) % 6);
  }

  used_number[num] = 1;
  used_num_cnt ++;

  //if you find that you ahve used all of the letters, clear both the array and the count
  if(used_num_cnt == NUM_DOTS){
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
 * @brief echo the number of the dot pressed
 * @param last_dot - unsigned char.   Which dot to play 
 * @return Void
 */
void md1_play_requested_dot(unsigned char last_dot)
{
  char buf[10];

  // Check for validity of number
  if (last_dot >= '1' && last_dot <= '6') {
    sprintf(buf, "dot_%c.MP3", last_dot);
    request_to_play_mp3_file(buf);
  }

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
      used_num_cnt = 0;
      // Play the introductory message for Mode 1
      request_to_play_mp3_file("MD1INT.MP3");
      current_state = STATE_REQUEST_INPUT1;
      break;
    case STATE_REQUEST_INPUT1:
      request_to_play_mp3_file("find_dot.MP3");
      expected_dot = random_number_as_char();
      current_state = STATE_REQUEST_INPUT2;
      break;
    case STATE_REQUEST_INPUT2:
      // Generate a random char from '1' to '6'
      md1_play_requested_dot(expected_dot);
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

/**
 * @brief in mode 1, enter button just replays the current prompt
 * @return Void
 */
void md1_call_mode_yes_answer(void)
{
  request_to_play_mp3_file("find_dot.MP3");
  current_state = STATE_REQUEST_INPUT2;
}

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
