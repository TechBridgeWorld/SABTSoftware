/**
 * @file MD3.c
 * @brief mode 3 code
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

int MD3_current_state;
char MD3_Last_Dot;
char expected_dot;
//Current_State - Defines the status of the mode
/* Vales and meanings
0 - Just started (in the begining, play the Welcome message)
1 - Waiting for user input
2 - Last user input processed
*/

/**
 * @brief Based off of the internal timer (TCNT1) - we generate
 *        a psuedo-random number. Turn that into a number from 1 - 11
 *        which corresponds to 1 of 11 different animals. Check which
 *        animals have been played already to be sure to play all of
 *        the different animals before repeating the list.
 * @return int - number between 1 - 11 corresponding to the animal file to play
 */
/*int choose_animal()
{
  char ret_char;
  int num = TCNT1;
  
  num *= PRIME;
  num = (abs(num) % 11) + 1;

  while(animal_used(num)) {
    num = TCNT1;
	num *= PRIME;
    num = (abs(num) % 11) + 1;
  }  

  // increment animals_used until we've used all 11 animals then reset everything
  animals_used++;
  if (animals_used == 11) {
    int i;
    animals_used = 0;
	for (i=0; i<11; i++)
	  animals_used_list[i] = 0;
  }

  return num;
}*/

/**
 * @brief Helper function to choose_animal(). Ensures that the same animal isn't
 *        called twice before going through the entire list first.
 * @return int - 0 means no (hasn't been used yet)
 *               1 means yes (has been used)
 */
/*int animal_used(int num) {
  int i;

  for (i=0; i<11; i++) {
    if (animals_used_list[i] == num)
	  return 1;
  }

  return 0;
}*/

/**
 * @brief  Given a char, in Last_Cell, play the corresponding number
 *         sound file
 * @return Void
 */
void MD3_PlayRequestedDot(void)
{
  switch(MD3_Last_Dot)
  {
    case '1':
      RequestToPlayMP3file("MD3_1.MP3");
      break;
    case '2':
      RequestToPlayMP3file("MD3_2.MP3");
      break;
    case '3':
      RequestToPlayMP3file("MD3_3.MP3");
      break;
    case '4':
      RequestToPlayMP3file("MD3_4.MP3");
      break;
    case '5':
      RequestToPlayMP3file("MD3_5.MP3");
      break;
    case '6':
      RequestToPlayMP3file("MD3_6.MP3");
      break;
  }
}

void MD3_Reset(void)
{
  MD3_current_state=0;
}

/**
 * @brief  Step through the main stages in the code.
 *         Note : We need 2 STATE_REQUEST_INPUT states because RequestToPlayMP3file
 *         cannot handle 2 calls in quick succession.
 * @return Void
 * @TODO record MD3INT.MP3
 */
void MD3_Main(void)
{/*
	char button_bits;
  switch(MD3_current_state)
  {
    case STATE_INITIAL:
      RequestToPlayMP3file("MD3INT.MP3");
      MD3_Current_State = STATE_REQUEST_INPUT1;
	  animals_used = 0;
	  if (choose_animal() == 1) {
        animal = "bee";
	  } else if (choose_animal() == 2) {
		animal = "camel";
	  } else if (choose_animal() == 3) {
		animal = "cat";
	  } else if (choose_animal() == 4) {
        animal = "cow";
	  } else if (choose_animal() == 5) {
		animal = "dog";
	  } else if (choose_animal() == 6) {
		animal = "horse";
	  } else if (choose_animal() == 7) {
		animal = "hyena";
	  } else if (choose_animal() == 8) {
		animal = "pig";
	  } else if (choose_animal() == 9) {
		animal = "rooster";
	  } else if (choose_animal() == 10) {
		animal = "sheep";
	  } else if (choose_animal() == 11) {
		animal = "zebra";
	  }
      break;
    case STATE_REQUEST_INPUT1:
	  RequestToPlayMP3file("please_write_the_animal.mp3");
	  MD3_Current_State = STATE_REQUEST_INPUT2;
      break;
    case STATE_REQUEST_INPUT2:
	  RequestToPlayMP3file(strcat(animal,".mp3"));
      MD3_Current_State = STATE_WAIT_INPUT;
      break;
    case STATE_WAIT_INPUT:
	  if (last_dot != 0) {
        if (last_dot == ENTER) {
          MD3_Current_State = STATE_PROC_INPUT;
		} else if (last_dot >= '1' && last_dot <='6') {
          button_bits != (1<<atoi(last_dot)-1);
		  MD3_PlayRequestedDot();
		}
	  }
	  break;
    case STATE_PROC_INPUT:
	  if (validLetter(button_bits)) {
        
      } else {
        RequestToPlayMP3file("invalid_pattern.mp3");
        MD3_Current_State = STATE_WAIT_INPUT;
	  }
	  break;
  }


switch(MD3_current_state)
  {
    case STATE_INITIAL:
      PRINTF("[MD1] Entering MD1\n");

      // Play the introductory message for Mode 1
      RequestToPlayMP3file("MD1INT.MP3");
      MD3_current_state = STATE_REQUEST_INPUT1;
      break;
    case STATE_REQUEST_INPUT1:
      RequestToPlayMP3file("find_dot.MP3");
      MD3_current_state = STATE_REQUEST_INPUT2;
      break;
    case STATE_REQUEST_INPUT2:
      // Generate a random char from '1' to '6'
      expected_dot = random_number_as_char(); 
      DPRINTF("dot requested = %c\r\n", expected_dot);
      PlayRequestedDot(expected_dot);
      MD3_current_state = STATE_WAIT_INPUT;
      break;
    case STATE_WAIT_INPUT:
      if(last_dot != 0)
        MD3_current_state = STATE_PROC_INPUT;
        break;
    case STATE_PROC_INPUT:
      if(last_dot != expected_dot)
      {
        RequestToPlayMP3file("no.MP3");
        last_dot = 0;
        MD3_current_state = STATE_WAIT_INPUT;
      }
      else
      {
        RequestToPlayMP3file("good.MP3");
        last_dot = 0;
        MD3_current_state = STATE_REQUEST_INPUT1;
      }      
      break;
    default:
      break;
  }


*/
}

void MD3_CallModeYesAnswer(void){}
void MD3_CallModeNoAnswer(void){}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void MD3_InputDot(char thisDot)
{
  MD3_Last_Dot=thisDot;
  MD3_current_state=2;
}

/**
 * @brief
 * @return Void
 */
void MD3_InputCell(char thisCell)
{

}
