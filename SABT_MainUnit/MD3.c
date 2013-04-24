/**
 * @file MD3.c
 * @brief Mode 3 code - Animal Game
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#include "letter_globals.h"

int md3_current_state;
char md3_last_dot, last_cell, expected_dot;


//@TODO   MAKE THIS BETTER
//char *animal_list[11] = {"bee", "camel", "cat", "cow", "dog", "horse",
//    "hyena", "pig", "rooster", "sheep", "zebra"};

//@TODO temporarily changed to use only easy animal names
char *animal_list[11] = {"bee", "bee", "cat", "cat", "dog", "dog",
    "pig", "pig", "rooster", "rooster", "dog"};

int animal_used_list[11] = {0,0,0,0,0,0,0,0,0,0,0};


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
  num = (abs(num) % 11);

   char buf[10];
  sprintf(buf, "num=%i\r\n", num);
  PRINTF(buf);

  while(animals_used_list[num]) 
  {
    num = TCNT1;
    num *= PRIME;
    num = (abs(num) % 11);
  }  
 
  animals_used_list[num] = 1;

  for(i = 0; i < 11; i ++){
    sprintf(buf, "arr=%i, ",animals_used_list[i] );
    PRINTF(buf);

  }
  TX_NEWLINE_PC;
  sprintf(buf, "cnt=%i", animals_used+1);
  PRINTF(buf);
  TX_NEWLINE_PC;

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
    // This will hold formatted file to access
    char req_mp3[10];
    sprintf((char*)req_mp3, "dot_%c.MP3", md3_last_dot);
    request_to_play_mp3_file(req_mp3);
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
  char letter_from_bits = get_letter_from_bits(button_bits);
  if((letter_from_bits >= 'a') && (letter_from_bits <= 'z')){
    entered_letter = letter_from_bits;
	return true;
  }
  return false;
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
      got_input = false;

      break;
    
    case STATE_REQUEST_INPUT1:
      PRINTF("INPUT1\r\n");
      request_to_play_mp3_file("MD3PLS.mp3");

      length_entered_word = 0;
	  current_word_index = 0;

      animal = animal_list[choose_animal()];

      md3_current_state = STATE_REQUEST_INPUT2;
      break;
    
    case STATE_REQUEST_INPUT2:
      PRINTF("INPUT2\r\n");

      char animal_file[16];
      sprintf(animal_file, "%s.mp3", animal);
      request_to_play_mp3_file(animal_file);

      md3_current_state = STATE_WAIT_INPUT;

      break;
    
    case STATE_WAIT_INPUT:
      if(got_input){
          got_input = false;
          md3_current_state = STATE_PROC_INPUT;
      }
      break;
    
    case STATE_PROC_INPUT:
      PRINTF("PROCESS\r\n");
      // set entered_letter in valid_letter(), but return 1 or 0

     if (last_cell == 0) {
	   md3_current_state = STATE_READ_ENTERED_LETTERS;
     } else if (valid_letter(last_cell)) {
        char buf[16];
        sprintf(buf, "%c.mp3", entered_letter);
        //usart_transmit_string_to_pc((unsigned char*)buf);
        request_to_play_mp3_file(buf);
        md3_current_state = STATE_CHECK_IF_CORRECT;
      } else {
        request_to_play_mp3_file("INVPAT.mp3");
        md3_current_state = STATE_READ_ENTERED_LETTERS;
      }
      break;
    
    case STATE_READ_ENTERED_LETTERS:
      PRINTF("INVALID\r\n");

      if(length_entered_word > 0) {
        char buf[16];
        sprintf(buf, "%c.mp3", animal[current_word_index]);
        request_to_play_mp3_file(buf);
        current_word_index++;
      }

      if (current_word_index == length_entered_word) {
        md3_current_state = STATE_WAIT_INPUT;
        current_word_index = 0; 
      }
      break;
    
    case STATE_CHECK_IF_CORRECT:
      PRINTF("CHECK\r\n");
      if (animal[length_entered_word] == entered_letter) {
        length_entered_word++;
        
        if (length_entered_word != strlen(animal))
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
      md3_current_state = STATE_READ_ENTERED_LETTERS;
      break;
    
    case STATE_CORRECT_INPUT:
      PRINTF("CORRECT\r\n");
      request_to_play_mp3_file("good.mp3");
      md3_current_state = STATE_WAIT_INPUT;
      break;
    
    case STATE_DONE_WITH_CURRENT_ANIMAL:
      PRINTF("DONE\r\n");
      request_to_play_mp3_file("nc_wrk.mp3");
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
    got_input = true;
  }
}
