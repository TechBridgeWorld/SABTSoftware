/**
 * @file MD2.c
 * @brief  Mode 2 code - Letter Practice
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"

int current_state;
char last_cell;
char md2_last_dot;

//@TODO   MAKE THIS BETTER
char letter_bits_arr[26] = 
{
  A_BITS, B_BITS, C_BITS, D_BITS, E_BITS, F_BITS, G_BITS, 
  H_BITS, I_BITS, J_BITS, K_BITS, L_BITS, M_BITS, N_BITS, 
  O_BITS, P_BITS, Q_BITS, R_BITS, S_BITS, T_BITS, U_BITS, 
  V_BITS, W_BITS, X_BITS, Y_BITS, Z_BITS
};

char letter_arr[26] = 
{
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
  'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
  'w', 'x', 'y', 'z'
};

/**
 * @brief Sets the given input to the file's last_dot
 * @return Void
 */
void set_last_dot2(char dot)
{
  last_dot = dot;
}


/**
 * @brief generates a psuedo random number based on the system clock
 * @return int- random number
 */
int generate_random_number()
{
  int ret = TCNT1;

  ret *= PRIME;
  return ret;
}

/**
 * @brief  Given a char, in last_cell, play the corresponding letter 
 *         sound file 
 * @return Void
 */
void play_requested_cell(char last_cell)
{
  // This will hold formatted file to access
  char req_mp3[10];

  if (last_cell >= 'a' && last_cell <= 'z') {
    sprintf((char*)req_mp3, "%c.MP3", last_cell);
    request_to_play_mp3_file(req_mp3);
  } else
    request_to_play_mp3_file("INVPAT.MP3");
}



/**
 * @brief Changes letter into letter bits
 * @param let - char, charachter letter want to change to bits 
 * @return char - letter that corresponds to buttons pressed 
 *                on error - not found bits, return -1
 */
char get_bits_from_letters(char let){
    int alphbt_len = 26;
    int i;
    for(i = 0; i < alphbt_len; i ++){
        if(letter_arr[i] == let)
            return letter_bits_arr[i];
        
        
    }
  return -1;
}


/**
 * @brief Changes letter bits into an actual char letter
 * @param bits - char, bits that correspond to the buttons pressed
 * @return char - letter that corresponds to buttons pressed 
 *                on error - not found bits, return -1
 */
char get_letter_from_bits(char bits)
{
  int alphabet_len = 26;
  int i;
  
  for(i = 0; i < alphabet_len; i++){
    if(letter_bits_arr[i] == bits)
      return letter_arr[i];
  }

  // Return error on failure (letter not present in array)
  return -1;
}

/**
 * @brief  Given a bitmask, in last_cell, play the corresponding letter
 *         sound file
 * @param bits - char, bitmask that should representa  braile letter 
 * @return Void
 */
void play_requested_bits(char bits)
{
  char letter = get_letter_from_bits(bits);

  if (letter == -1)
    request_to_play_mp3_file("INVPAT.MP3");
  else
    play_requested_cell(letter);
}

/**
 * @brief  Given a char, in last_cell, play the corresponding number
 *         sound file
 * @return Void
 */
void md2_play_requested_dot(char md2_last_dot)
{
  // This will hold formatted file to access
  char req_mp3[10];
  sprintf((char*)req_mp3, "dot_%c.MP3", md2_last_dot);
  request_to_play_mp3_file(req_mp3);
}

/**
 * @brief  Reset the current state
 * @return Void
 */
void md2_reset(void)
{
  current_state = 0;
  md2_last_dot = 0;
}

/** 
 * @brief compares button_bits and what current_letter is to see
 *        if they correspond
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @param current_letter - char, letter that the bits should be matching
 * @return bool - return true if they match, and false, if they do not
 */
bool check_if_correct(char button_bits, char current_letter)
{
  char letter_from_bits = get_letter_from_bits(button_bits);
  
  char buf[10];
  sprintf(buf, "%c, %c, %c\r\n", button_bits, current_letter, letter_from_bits);
  PRINTF(buf);
  
  return (letter_from_bits == current_letter);
}

/**
 * @brief This function sets up in the initial values needed to make the program work
 * @return Void
 */
void setup_initial()
{
  button_bits = 0;
  letter_set = 0;
  current_count = 0;
  random_count = 0;
  current_random_letter = 0;
  initial_letter = 'a';
  use_random_letter = 0;
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void md2_main(void)
{
  switch(current_state)
  {
    case STATE_INITIAL:
      setup_initial();
      request_to_play_mp3_file("MD2INT.MP3");
      current_state = SET_LETTER_VALS;
      break;
    
    case SET_LETTER_VALS:
      curr_button = '0';
      current_random_letter = (initial_letter + ((letter_set * 5 
        + (generate_random_number()%5)) % 26));
      current_letter = (initial_letter + ((letter_set*5 + current_count) % 26));
      current_state = STATE_REQUEST_INPUT1;
      break;

  case STATE_REQUEST_INPUT1:
      request_to_play_mp3_file("pls_wrt.MP3");
      current_state = STATE_REQUEST_INPUT2;
      break;
    
    case STATE_REQUEST_INPUT2:
      if(use_random_letter){
        play_requested_cell(current_random_letter);
        current_state = STATE_WAIT_INPUT;
      }
      else
      {
        play_requested_cell(current_letter);
        current_state = STATE_BUTT_TO_PRESS_1;
      }
      break;
          
    case STATE_BUTT_TO_PRESS_1:
      request_to_play_mp3_file("press.MP3");
      current_state = STATE_BUTT_TO_PRESS_2;
    break;

  case STATE_BUTT_TO_PRESS_2:
      curr_button += 1;
      
    char bits = get_bits_from_letters(current_letter);
    char curr_bit = (bits >> (atoi(&curr_button) - 1)) & 1;
    //get the bits for each depending on button count - and play sound if bit is set
    if(curr_bit){
         md2_play_requested_dot(curr_button);
    }
    
    if(atoi(&curr_button) == NUM_BUT)    
       current_state = STATE_WAIT_INPUT;
      
    break;

    case STATE_WAIT_INPUT:
      if(last_dot != 0)
      {
        //The user just input their word
        if(last_dot == ENTER)
        {
          //they got the word right, change letter unless you are at 5
          //already then enter random mode.

          if(!use_random_letter)
          {
		    PRINTF("checkifcorrect");
			TX_NEWLINE_PC;
            if(check_if_correct(button_bits, current_letter))
            {
              request_to_play_mp3_file("good.MP3");
              //if you have successfully completed this letter set
              if(current_count == 4)
              {
                use_random_letter = 1;
                current_count = 0;
                random_count = 0;
                current_state = SET_LETTER_VALS;
              }
              //successfully completed a letter in letter set
              else
              {
                current_count++;
                current_state = SET_LETTER_VALS;
              }
              button_bits = 0;
            }
            //move to an error state so you can tell the user what they input
            else
            {
              request_to_play_mp3_file("no.MP3");
              current_state = STATE_ERROR_1;
            }
          }

          else
          {
            if(check_if_correct(button_bits, current_random_letter))
            {
              request_to_play_mp3_file("good.MP3");
              
              // If you have successfully completed this letter set
              if(random_count == 4)
              {
                use_random_letter = 0;
                current_count = 0;
                random_count = 0;
                letter_set++;
                current_state = SET_LETTER_VALS;
              }
              //successfully completed a letter in letter set
              else
              {
                random_count++;
                current_state = SET_LETTER_VALS;
              }
              button_bits = 0;
            }

            //move to an error state so you can tell the user what they input
            else
            {
              request_to_play_mp3_file("no.MP3");
              current_state = STATE_ERROR_1;
            }  
          }
        }
        else if((last_dot >= '1') && (last_dot <= '6'))
        {
		  int temp = atoi(&last_dot);
          button_bits |= (1 << (atoi(&last_dot) - 1));
          // TODO change play requested dot to take 1 param
          md2_play_requested_dot(last_dot);
        }
        last_dot = 0;      
      }
      break;
      
    case STATE_ERROR_1:
      //@TODO- change to pressed
      request_to_play_mp3_file("press.MP3");
      current_state = STATE_ERROR_2;
      break;
          
    case STATE_ERROR_2:
      play_requested_bits(button_bits);
      button_bits = 0;
      current_state = STATE_REQUEST_INPUT_1;
      break;
  }
}


/**
 * @brief this function will be called when enter is pressed during mode
 * @return Void
 */
void md2_call_mode_yes_answer(void)
{
  last_dot = ENTER;
}

void md2_call_mode_no_answer(void)
{
}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void md2_input_dot(char this_dot)
{
  last_dot = this_dot;
  md2_last_dot = this_dot;
  //current_state = 3;
}

/**
 * @brief  
 * @return Void
 */
void md2_input_cell(char this_cell)
{
  if(md2_last_dot != 0)
  {
    last_cell = this_cell;
    //current_state=2;
  }
}
