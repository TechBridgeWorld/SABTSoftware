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
 * @brief Sets the given input to the file's last_dot
 * @return Void
 */
void set_last_dot(char dot)
{
  last_dot = dot;
}

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

  switch(num){
    case 1:
    return '1';
    break;
  case 2:
    return '2';
    break;
  case 3:
    return '3';
    break;
  case 4:
    return '4';
    break;
  case 5:
    return '5';
    break;
  case 6:
    return '6';
    break;
  default:
    return '0';
    break;
  }
}

/**
 * @brief echo the number of the dot pressed
 * @param last_dot - unsigned char.   Which dot to play 
 * @return Void
 */
void PlayRequestedDot(unsigned char last_dot)
{
  // TODO default case?
  switch(last_dot)
  {
    case '1':
      RequestToPlayMP3file("MD1_1.MP3");
      break;
    case '2':
      RequestToPlayMP3file("MD1_2.MP3");
      break;
    case '3':
      RequestToPlayMP3file("MD1_3.MP3");
      break;
    case '4':
      RequestToPlayMP3file("MD1_4.MP3");
      break;
    case '5':
      RequestToPlayMP3file("MD1_5.MP3");
      break;
    case '6':
      RequestToPlayMP3file("MD1_6.MP3");
      break;
  }
}

/**
 * @brief Reset mode 1 to starting state
 *        Should be useful for error handling
 * @return Void
 */
void MD1_Reset(void)
{
  current_state = STATE_INITIAL;
}

/**
 * @brief main code for Mode 1
 * Prompts the user to press a certain random dot, gives feedback based on
 * whether they pressed the correct dot
 * @return Void
 */
void MD1_Main(void)
{
  switch(current_state)
  {
    case STATE_INITIAL:
      PRINTF("[MD1] Entering MD1\n");

      // Play the introductory message for Mode 1
      RequestToPlayMP3file("MD1INT.MP3");
      current_state = STATE_REQUEST_INPUT1;
      break;
    case STATE_REQUEST_INPUT1:
      RequestToPlayMP3file("find_dot.MP3");
      current_state = STATE_REQUEST_INPUT2;
      break;
    case STATE_REQUEST_INPUT2:
      // Generate a random char from '1' to '6'
      expected_dot = random_number_as_char(); 
      DPRINTF("dot requested = %c\r\n", expected_dot);
      PlayRequestedDot(expected_dot);
      current_state = STATE_WAIT_INPUT;
      break;
    case STATE_WAIT_INPUT:
      if(last_dot != 0)
        current_state = STATE_PROC_INPUT;
        break;
    case STATE_PROC_INPUT:
      if(last_dot != expected_dot)
      {
        RequestToPlayMP3file("no.MP3");
        last_dot = 0;
        current_state = STATE_WAIT_INPUT;
      }
      else
      {
        RequestToPlayMP3file("good.MP3");
        last_dot = 0;
        current_state = STATE_REQUEST_INPUT1;
      }      
      break;
    default:
      break;
  }
}

void MD1_CallModeYesAnswer(void) {}

void MD1_CallModeNoAnswer(void) {}

/**
 * @brief register dot input
 *        Sets the program to STATE_PROC_INPUT
 * @param thisDot the dot being input
 * @return Void
 */
void MD1_InputDot(char this_dot)
{
  last_dot = this_dot;
  current_state = STATE_PROC_INPUT;
}

void MD1_InputCell(char thisCell) {}
