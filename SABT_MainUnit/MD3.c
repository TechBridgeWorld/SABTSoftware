/**
 * @file MD3.c
 * @brief mode 3 code
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#include "letter_globals.h"

int MD3_Current_State;
char MD3_Last_Dot;
char Last_Cell;
char expected_dot;
//Current_State - Defines the status of the mode
/* Vales and meanings
0 - Just started (in the begining, play the Welcome message)
1 - Waiting for user input
2 - Last user input processed
*/

/**
 * @brief Helper function to choose_animal(). Ensures that the same animal isn't
 *        called twice before going through the entire list first.
 * @return int - 0 means no (hasn't been used yet)
 *               1 means yes (has been used)
 */
int animal_used(int num) {
  int i;

  for (i=0; i<11; i++) {
    if (animals_used_list[i] == num)
	  return 1;
  }

  return 0;
}

/**
 * @brief Based off of the internal timer (TCNT1) - we generate
 *        a psuedo-random number. Turn that into a number from 1 - 11
 *        which corresponds to 1 of 11 different animals. Check which
 *        animals have been played already to be sure to play all of
 *        the different animals before repeating the list.
 * @return int - number between 1 - 11 corresponding to the animal file to play
 */
int choose_animal()
{
  int num = TCNT1;
  
  num *= PRIME;
  num = (abs(num) % 11) + 1;

  while(animal_used(num)) {
    num = TCNT1;
	num *= PRIME;
    num = (abs(num) % 11) + 1;
  }  
  animals_used_list[animals_used] = num;

  // increment animals_used until we've used all 11 animals then reset everything
  animals_used++;
  if (animals_used == 11) {
    int i;
    animals_used = 0;
	for (i=0; i<11; i++)
	  animals_used_list[i] = 0;
  }

  return num;
}

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

void MD3_Reset(void)
{
  MD3_Current_State = 0;
  MD3_Last_Dot = 0;
}

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool validLetter(char button_bits){
    switch(button_bits)
    {
        case A_BITS:
		  entered_letter = 'a';
          return true;
          break;
        case B_BITS:
		  entered_letter = 'b';
          return true;
          break;
        case C_BITS:
		  entered_letter = 'c';
          return true;
          break;
        case D_BITS:
		  entered_letter = 'd';
          return true;
          break;
        case E_BITS:
		  entered_letter = 'e';
          return true;
          break;
        case F_BITS:
		  entered_letter = 'f';
          return true;
          break;
        case G_BITS:
		  entered_letter = 'g';
          return true;
          break;
        case H_BITS:
		  entered_letter = 'h';
          return true;
          break;
        case I_BITS:
		  entered_letter = 'i';
          return true;
          break;
        case J_BITS:
		  entered_letter = 'j';
          return true;
          break;
        case K_BITS:
		  entered_letter = 'k';
          return true;
          break;
        case L_BITS:
		  entered_letter = 'l';
          return true;
          break;
        case M_BITS:
		  entered_letter = 'm';
          return true;
          break;
        case N_BITS:
		  entered_letter = 'n';
          return true;
          break;
        case O_BITS:
		  entered_letter = 'o';
          return true;
          break;
        case P_BITS:
		  entered_letter = 'p';
          return true;
          break;
        case Q_BITS:
		  entered_letter = 'q';
          return true;
          break;
        case R_BITS:
		  entered_letter = 'r';
          return true;
          break;
        case S_BITS:
		  entered_letter = 's';
          return true;
          break;
        case T_BITS:
		  entered_letter = 't';
          return true;
          break;
        case U_BITS:
		  entered_letter = 'u';
          return true;
          break;
        case V_BITS:
		  entered_letter = 'v';
          return true;
          break;
        case W_BITS:
		  entered_letter = 'w';
          return true;
          break;
        case X_BITS:
		  entered_letter = 'x';
          return true;
          break;
        case Y_BITS:
		  entered_letter = 'y';
          return true;
          break;
        case Z_BITS:
		  entered_letter = 'z';
          return true;
          break;
        default:
          return false;
          break;
    }
    
}

/**
 * @brief  Step through the main stages in the code.
 *         Note : We need 2 STATE_REQUEST_INPUT states because RequestToPlayMP3file
 *         cannot handle 2 calls in quick succession.
 * @return Void
 * @TODO record MD3INT.MP3
 */
void MD3_Main(void)
{
  switch(MD3_Current_State)
  {
    case STATE_INITIAL:
	  PRINTF("INITIAL\r\n");
      //RequestToPlayMP3file("MD2INT.MP3");
      RequestToPlayMP3file("MD3INT.mp3");
      
	  MD3_Current_State = STATE_REQUEST_INPUT1;
  	  animals_used = 0;

	  // initialize animal_list
	  animal_list[0] = "bee";
	  animal_list[1] = "camel";
	  animal_list[2] = "cat";
	  animal_list[3] = "cow";
	  animal_list[4] = "dog";
	  animal_list[5] = "horse";
	  animal_list[6] = "hyena";
	  animal_list[7] = "pig";
	  animal_list[8] = "rooster";
	  animal_list[9] = "sheep";
	  animal_list[10] = "zebra";

      break;
    case STATE_REQUEST_INPUT1:
	  PRINTF("INPUT1\r\n");
	  RequestToPlayMP3file("MD3PLS.mp3");

      length_current_word = 0;

      animal = animal_list[choose_animal()-1];

	  MD3_Current_State = STATE_REQUEST_INPUT2;
      break;
    case STATE_REQUEST_INPUT2:
	  PRINTF("INPUT2\r\n");
	  RequestToPlayMP3file(strcat(animal, ".mp3"));
      MD3_Current_State = STATE_WAIT_INPUT;
      break;
    case STATE_WAIT_INPUT:
      // Progress to next state when MD3_Input_Cell is called below

	  //PRINTF("WAIT\r\n");
	  /*if(last_dot != 0){
        if (last_dot == ENTER) {
          MD3_Current_State = STATE_PROC_INPUT;
	    } else if (last_dot >= '1' && last_dot <='6') {
          button_bits |= (1<<(atoi(&last_dot)-1));
	      MD3_PlayRequestedDot();		
	    }
		last_dot = 0;
      }*/
	  break;
    case STATE_PROC_INPUT:
	  PRINTF("PROCESS\r\n");
      // set entered_letter in validLetter(), but return 1 or 0
	  if (validLetter(Last_Cell)) {
	    char buf[16];
		sprintf(buf, "MD2_%c.mp3", entered_letter);
		USART_transmitStringToPC((unsigned char*)buf);
        RequestToPlayMP3file(buf);
        MD3_Current_State = STATE_CHECK_IF_CORRECT;
      } else {
        RequestToPlayMP3file("INVPAT.mp3");
        MD3_Current_State = STATE_INVALID_INPUT;
	  }
	  break;
    case STATE_INVALID_INPUT:
	  PRINTF("INVALID\r\n");
      
      //RequestToPlayMP3file("currently_entered.mp3");
	  char buf[16];
	  sprintf(buf, "MD2_%c.mp3", animal[current_word_index]);
      RequestToPlayMP3file(buf);
	  current_word_index++;

	  if (current_word_index == length_current_word) {
        MD3_Current_State = STATE_WAIT_INPUT;
		current_word_index = 0; 
	  }
      break;
    case STATE_CHECK_IF_CORRECT:
	  PRINTF("CHECK\r\n");
      if (animal[length_current_word] == entered_letter) {
		length_current_word++;
		char buf[8];
		sprintf(buf, "%d %d\r\n", length_current_word, strlen(animal));
		USART_transmitStringToPC(buf);
        if (length_current_word != strlen(animal))
          MD3_Current_State = STATE_CORRECT_INPUT;
        else
          MD3_Current_State = STATE_DONE_WITH_CURRENT_ANIMAL;
      } else {
        MD3_Current_State = STATE_WRONG_INPUT;
      }
      break;
    case STATE_WRONG_INPUT:
	  PRINTF("WRONG\r\n");
	  RequestToPlayMP3file("no.mp3");
      MD3_Current_State = STATE_WAIT_INPUT;
      break;
    case STATE_CORRECT_INPUT:
	  PRINTF("CORRECT\r\n");
      RequestToPlayMP3file("good.mp3");
      MD3_Current_State = STATE_WAIT_INPUT;
      break;
    case STATE_DONE_WITH_CURRENT_ANIMAL:
	  PRINTF("DONE\r\n");
      RequestToPlayMP3file("nice_work.mp3");
      MD3_Current_State = STATE_REQUEST_INPUT1;
      break;
  }
}

void MD3_CallModeYesAnswer(void){}
void MD3_CallModeNoAnswer(void){}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void MD3_InputDot(char thisDot)
{
  MD3_Last_Dot = thisDot;
  MD3_PlayRequestedDot();
}

/**
 * @brief
 * @return Void
 */
void MD3_InputCell(char thisCell)
{
  if(MD3_Last_Dot != 0)
  {
    Last_Cell = thisCell;
	MD3_Current_State = STATE_PROC_INPUT;
  }
}
