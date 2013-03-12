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

int md3_current_state;
char md3_last_dot, last_cell, expected_dot;

/**
 * @brief Helper function to choose_animal(). Ensures that the same animal isn't
 *        called twice before going through the entire list first.
 * @return int - 0 means no (hasn't been used yet)
 *               1 means yes (has been used)
 */
int animal_used(int num) {
  int i;

  for (i = 0; i < 11; i++) {
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
  int i;

  num *= PRIME;
  num = (abs(num) % 11) + 1;

  while(animal_used(num)) 
  {
    num = TCNT1;
    num *= PRIME;
    num = (abs(num) % 11) + 1;
  }  
  animals_used_list[animals_used] = num;

  // increment animals_used until we've used all 11 animals then reset everything
  animals_used++;
  if (animals_used == 11)
  {
    animals_used = 0;
    for (i = 0; i < 11; i++)
      animals_used_list[i] = 0;
  }

  return num;
}

/**
 * @brief  Given a char, in last_cell, play the corresponding number
 *         sound file
 * @return Void
 */
void play_requested_dot(void)
{
  switch(md3_last_dot)
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
}

void md3_reset(void)
{
  md3_current_state = 0;
  md3_last_dot = 0;
}

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool valid_letter(char button_bits){
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
 *         Note : We need 2 STATE_REQUEST_INPUT states because request_to_play_mp3_file
 *         cannot handle 2 calls in quick succession.
 * @return Void
 * @TODO record MD3INT.MP3
 */
void md3_main(void)
{
  switch(md3_current_state)
  {
    case STATE_INITIAL:
      PRINTF("INITIAL\r\n");
      //request_to_play_mp3_file("MD2INT.MP3");
      request_to_play_mp3_file("MD3INT.mp3");

      md3_current_state = STATE_REQUEST_INPUT1;
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
      request_to_play_mp3_file("MD3PLS.mp3");

      length_current_word = 0;

      animal = animal_list[choose_animal() - 1];

      md3_current_state = STATE_REQUEST_INPUT2;
      break;
    
    case STATE_REQUEST_INPUT2:
      PRINTF("INPUT2\r\n");
      request_to_play_mp3_file(strcat(animal, ".mp3"));
      md3_current_state = STATE_WAIT_INPUT;
      break;
    
    case STATE_WAIT_INPUT:
      // Progress to next state when MD3_Input_Cell is called below

      //PRINTF("WAIT\r\n");
      /*if(last_dot != 0){
        if (last_dot == ENTER) {
        md3_current_state = STATE_PROC_INPUT;
        } else if (last_dot >= '1' && last_dot <='6') {
        button_bits |= (1<<(atoi(&last_dot)-1));
        play_requested_dot();    
        }
        last_dot = 0;
        }*/
      break;
    
    case STATE_PROC_INPUT:
      PRINTF("PROCESS\r\n");
      // set entered_letter in valid_letter(), but return 1 or 0
      if (valid_letter(last_cell)) {
        char buf[16];
        sprintf(buf, "MD2_%c.mp3", entered_letter);
        USART_transmitStringToPC((unsigned char*)buf);
        request_to_play_mp3_file(buf);
        md3_current_state = STATE_CHECK_IF_CORRECT;
      } else {
        request_to_play_mp3_file("INVPAT.mp3");
        md3_current_state = STATE_INVALID_INPUT;
      }
      break;
    
    case STATE_INVALID_INPUT:
      PRINTF("INVALID\r\n");

      //request_to_play_mp3_file("currently_entered.mp3");
      char buf[16];
      sprintf(buf, "MD2_%c.mp3", animal[current_word_index]);
      request_to_play_mp3_file(buf);
      current_word_index++;

      if (current_word_index == length_current_word) {
        md3_current_state = STATE_WAIT_INPUT;
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
          md3_current_state = STATE_CORRECT_INPUT;
        else
          md3_current_state = STATE_DONE_WITH_CURRENT_ANIMAL;
      } else {
        md3_current_state = STATE_WRONG_INPUT;
      }
      break;
    
    case STATE_WRONG_INPUT:
      PRINTF("WRONG\r\n");
      request_to_play_mp3_file("no.mp3");
      md3_current_state = STATE_WAIT_INPUT;
      break;
    
    case STATE_CORRECT_INPUT:
      PRINTF("CORRECT\r\n");
      request_to_play_mp3_file("good.mp3");
      md3_current_state = STATE_WAIT_INPUT;
      break;
    
    case STATE_DONE_WITH_CURRENT_ANIMAL:
      PRINTF("DONE\r\n");
      request_to_play_mp3_file("nice_work.mp3");
      md3_current_state = STATE_REQUEST_INPUT1;
      break;
  }
}

void md3_call_mode_yes_answer(void)
{
}

void md3_call_mode_no_answer(void)
{
}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void md3_input_dot(char this_dot)
{
  md3_last_dot = this_dot;
  play_requested_dot();
}

/**
 * @brief
 * @return Void
 */
void md3_input_cell(char this_cell)
{
  if(md3_last_dot != 0)
  {
    last_cell = this_cell;
    md3_current_state = STATE_PROC_INPUT;
  }
}
