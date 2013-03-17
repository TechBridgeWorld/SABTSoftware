/**
 * @file MD2.c
 * @brief mode 2 code
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"

int Current_State;
char Last_Cell;
char MD2_Last_Dot;
//@TODO   MAKE THIS BETTER
char letter_bits_arr[26] = {A_BITS, B_BITS, C_BITS, D_BITS, E_BITS, F_BITS, G_BITS, H_BITS, I_BITS, J_BITS,
        K_BITS, L_BITS, M_BITS, N_BITS, O_BITS, P_BITS, Q_BITS, R_BITS, S_BITS, T_BITS,
        U_BITS, V_BITS, W_BITS, X_BITS, Y_BITS, Z_BITS};
char letter_arr[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
//Current_State - Defines the status of the mode
/* Vales and meanings
0 - Just started (in the begining, play the Welcome message)
1 - Waiting for user input
2 - Last user input processed
*/

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
int generateRandomNumber(){
    int ret = TCNT1;
    ret *= PRIME;
    return ret;
}

/**
 * @brief  Given a char, in Last_Cell, play the corresponding letter 
 *         sound file 
 * @return Void
 */
void PlayRequestedCell(char last_cell)
{
    //this will hold formatted file to access
	if((last_cell >= 'a') && (last_cell <= 'z')){
      char req_MP3[10];
      sprintf((char*)req_MP3, "MD2_%c.MP3", last_cell);
      RequestToPlayMP3file(req_MP3);
    }
	
	else{
	  RequestToPlayMP3file("MD2ER1.MP3");
	} 

    /*switch(last_cell)
  {
    case 'a':
      RequestToPlayMP3file("MD2_a.MP3");
      break;
    case 'b':
      RequestToPlayMP3file("MD2_b.MP3");
      break;
    case 'c':
      RequestToPlayMP3file("MD2_c.MP3");
      break;
    case 'd':
      RequestToPlayMP3file("MD2_d.MP3");
      break;
    case 'e':
      RequestToPlayMP3file("MD2_e.MP3");
      break;
    case 'f':
      RequestToPlayMP3file("MD2_f.MP3");
      break;
    case 'g':
      RequestToPlayMP3file("MD2_g.MP3");
      break;
    case 'h':
      RequestToPlayMP3file("MD2_h.MP3");
      break;
    case 'i':
      RequestToPlayMP3file("MD2_i.MP3");
      break;
    case 'j':
      RequestToPlayMP3file("MD2_j.MP3");
      break;
    case 'k':
      RequestToPlayMP3file("MD2_k.MP3");
      break;
    case 'l':
      RequestToPlayMP3file("MD2_l.MP3");
      break;
    case 'm':
      RequestToPlayMP3file("MD2_m.MP3");
      break;
    case 'n':
      RequestToPlayMP3file("MD2_n.MP3");
      break;
    case 'o':
      RequestToPlayMP3file("MD2_o.MP3");
      break;
    case 'p':
      RequestToPlayMP3file("MD2_p.MP3");
      break;
    case 'q':
      RequestToPlayMP3file("MD2_q.MP3");
      break;
    case 'r':
      RequestToPlayMP3file("MD2_r.MP3");
      break;
    case 's':
      RequestToPlayMP3file("MD2_s.MP3");
      break;
    case 't':
      RequestToPlayMP3file("MD2_t.MP3");
      break;
    case 'u':
      RequestToPlayMP3file("MD2_u.MP3");
      break;
    case 'v':
      RequestToPlayMP3file("MD2_v.MP3");
      break;
    case 'w':
      RequestToPlayMP3file("MD2_w.MP3");
      break;
    case 'x':
      RequestToPlayMP3file("MD2_x.MP3");
      break;
    case 'y':
      RequestToPlayMP3file("MD2_y.MP3");
      break;
    case 'z':
      RequestToPlayMP3file("MD2_z.MP3");
      break;
    default:
      RequestToPlayMP3file("MD2ER1.MP3");
      break;
  }*/
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
char getLetterFromBits(char bits){
    int alphbt_len = 26;
    int i;
    for(i = 0; i < alphbt_len; i ++){
        if(letter_bits_arr[i] == bits)
            return letter_arr[i];
        
        
    }
    
    /*switch(bits)
    {
        case A_BITS:
            return 'a';
            break;
        case B_BITS:
            return 'b';
            break;
        case C_BITS:
            return 'c';
            break;
        case D_BITS:
            return 'd';
            break;
        case E_BITS:
            return 'e';
            break;
        case F_BITS:
            return 'f';
            break;
        case G_BITS:
            return 'g';
            break;
        case H_BITS:
            return 'h';
            break;
        case I_BITS:
            return 'i';
            break;
        case J_BITS:
            return 'j';
            break;
        case K_BITS:
            return 'k';
            break;
        case L_BITS:
            return 'l';
            break;
        case M_BITS:
            return 'm';
            break;
        case N_BITS:
            return 'n';
            break;
        case O_BITS:
            return 'o';
            break;
        case P_BITS:
            return 'p';
            break;
        case Q_BITS:
            return 'q';
            break;
        case R_BITS:
            return 'r';
            break;
        case S_BITS:
            return 's';
            break;
        case T_BITS:
            return 't';
            break;
        case U_BITS:
            return 'u';
            break;
        case V_BITS:
            return 'v';
            break;
        case W_BITS:
            return 'w';
            break;
        case X_BITS:
            return 'x';
            break;
        case Y_BITS:
            return 'y';
            break;
        case Z_BITS:
            return 'z';
            break;
        default:
            return '0';
            break;
    }*/
    return -1;
}

/**
 * @brief  Given a bitmask, in Last_Cell, play the corresponding letter
 *         sound file
 * @param bits - char, bitmask that should representa  braile letter 
 * @return Void
 */
void PlayRequestedBits(char bits)
{
    PlayRequestedCell(getLetterFromBits(bits));
}


/**
 * @brief  Given a char, in Last_Cell, play the corresponding number
 *         sound file
 * @return Void
 */
void MD2_PlayRequestedDot(char MD2_Last_Dot)
{
    //this will hold formatted file to access
    char req_MP3[10];
    sprintf((char*)req_MP3, "MD1_%c.MP3", MD2_Last_Dot);
    RequestToPlayMP3file(req_MP3);
    
}

/**
 * @brief  Reset the current state
 * @return Void
 */
void MD2_Reset(void)
{
  Current_State=0;
  MD2_Last_Dot=0;
}


/** 
 * @brief compares button_bits and what current_letter is to see
 *        if they correspond
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @param current_letter - char, letter that the bits should be matching
 * @return bool - return true if they match, and false, if they do not
 */
bool checkIfCorrect(char button_bits, char current_letter){
    char letter_from_bits = getLetterFromBits(button_bits);
    return (letter_from_bits == current_letter);
    
    /*switch(current_letter)
    {
        case 'a':
            if(button_bits == A_BITS)
              return true;
            else
              return false;
            break;
        case 'b':
            if(button_bits == B_BITS)
                return true;
            else
                return false;
            break;
        case 'c':
            if(button_bits == C_BITS)
                return true;
            else
                return false;
            break;
        case 'd':
            if(button_bits == D_BITS)
                return true;
            else
                return false;
            break;
        case 'e':
            if(button_bits == E_BITS)
                return true;
            else
                return false;
            break;
        case 'f':
            if(button_bits == F_BITS)
                return true;
            else
                return false;
            break;
        case 'g':
            if(button_bits == G_BITS)
                return true;
            else
                return false;
            break;
        case 'h':
            if(button_bits == H_BITS)
                return true;
            else
                return false;
            break;
        case 'i':
            if(button_bits == I_BITS)
                return true;
            else
                return false;
            break;
        case 'j':
            if(button_bits == J_BITS)
                return true;
            else
                return false;
            break;
        case 'k':
            if(button_bits == K_BITS)
                return true;
            else
                return false;
            break;
        case 'l':
            if(button_bits == L_BITS)
                return true;
            else
                return false;
            break;
        case 'm':
            if(button_bits == M_BITS)
                return true;
            else
                return false;
            break;
        case 'n':
            if(button_bits == N_BITS)
                return true;
            else
                return false;
            break;
        case 'o':
            if(button_bits == O_BITS)
                return true;
            else
                return false;
            break;
        case 'p':
            if(button_bits == P_BITS)
                return true;
            else
                return false;
            break;
        case 'q':
            if(button_bits == Q_BITS)
                return true;
            else
                return false;
            break;
        case 'r':
            if(button_bits == R_BITS)
                return true;
            else
                return false;
            break;
        case 's':
            if(button_bits == S_BITS)
                return true;
            else
                return false;
            break;
        case 't':
            if(button_bits == T_BITS)
                return true;
            else
                return false;
            break;
        case 'u':
            if(button_bits == U_BITS)
                return true;
            else
                return false;
            break;
        case 'v':
            if(button_bits == V_BITS)
                return true;
            else
                return false;
            break;
        case 'w':
            if(button_bits == W_BITS)
                return true;
            else
                return false;
            break;
        case 'x':
            if(button_bits == X_BITS)
                return true;
            else
                return false;
            break;
        case 'y':
            if(button_bits == Y_BITS)
                return true;
            else
                return false;
            break;
        case 'z':
            if(button_bits == Z_BITS)
                return true;
            else
                return false;
            break;
        default:
            return false;
            break;
    }*/
    
}


/**
 * @brief This function sets up in the initial values needed to make the program work
 * @return Void
 */
void setup_initial(){
    button_bits = 0;
    letter_set = 0;
    current_count = 0;
    random_count = 0;
    current_random_letter = 0;
    initial_letter = 'a';
    use_random_letter = 0;
	curr_button = '0';
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void MD2_Main(void)
{
  switch(Current_State)
  {
    case STATE_INITIAL:
      setup_initial();
      RequestToPlayMP3file("MD2INT.MP3");
      Current_State = SET_LETTER_VALS;
      break;
          
    case SET_LETTER_VALS:
	  curr_button = '0';
	  current_random_letter = (initial_letter + ((letter_set*5 + (generateRandomNumber()%5)) % 26));
      current_letter = (initial_letter + ((letter_set*5 + current_count) % 26));
	  Current_State = STATE_REQUEST_INPUT1;
	  break;

	case STATE_REQUEST_INPUT1:
      RequestToPlayMP3file("pl_wrt.MP3");
      Current_State = STATE_REQUEST_INPUT2;
      break;
          
    case STATE_REQUEST_INPUT2:
      if(use_random_letter){
        PlayRequestedCell(current_random_letter);
		Current_State = STATE_WAIT_INPUT;
	  }
      else{
        PlayRequestedCell(current_letter);
        Current_State = STATE_BUTT_TO_PRESS_1;
      }
      break;
          
    case STATE_BUTT_TO_PRESS_1:
      RequestToPlayMP3file("press.MP3");
      Current_State = STATE_BUTT_TO_PRESS_2;
	  break;

	case STATE_BUTT_TO_PRESS_2:
      curr_button += 1;
      
	  char bits = get_bits_from_letters(current_letter);
	  char curr_bit = (bits >> (atoi(&curr_button) - 1)) & 1;
	  //get the bits for each depending on button count - and play sound if bit is set
	  if(curr_bit){
         MD2_PlayRequestedDot(curr_button);
	  }
	  
	  if(atoi(&curr_button) == NUM_BUT)	  
	     Current_State = STATE_WAIT_INPUT;
      
	  break;

    case STATE_WAIT_INPUT:
      if(last_dot != 0){
        //The user just input their word

        if(last_dot == ENTER){
          //they got the word right, change letter unless you are at 5
          //already then enter random mode.

          if(!use_random_letter){
            if(checkIfCorrect(button_bits, current_letter)){
              RequestToPlayMP3file("good.MP3");
              //if you have successfully completed this letter set
              if(current_count == 4){
                use_random_letter = 1;
				current_count = 0;
				random_count = 0;
                Current_State = SET_LETTER_VALS;
              }
              //successfully completed a letter in letter set
              else{
                current_count ++;
                Current_State = SET_LETTER_VALS;
              }
			  button_bits = 0;
            }
          
            //move to an error state so you can tell the user what they input
            else{
              RequestToPlayMP3file("no.MP3");
              Current_State = STATE_ERROR_1;
            }
          }

          else
		  {
            if(checkIfCorrect(button_bits, current_random_letter))
			{
              RequestToPlayMP3file("good.MP3");
              //if you have successfully completed this letter set
              if(random_count == 4)
			  {
                use_random_letter = 0;
				current_count = 0;
				random_count = 0;
				letter_set ++;
                Current_State = SET_LETTER_VALS;
              }
              //successfully completed a letter in letter set
              else
			  {
                random_count ++;
                Current_State = SET_LETTER_VALS;
              }
			  button_bits = 0;
            }
              
            //move to an error state so you can tell the user what they input
            else
			{
              RequestToPlayMP3file("no.MP3");
              Current_State = STATE_ERROR_1;
            }  
          }
        }
        else if((last_dot >= '1') && (last_dot <= '6'))
		{
          button_bits |= (1 << (atoi(&last_dot) - 1));
		  // TODO change play requested dot to take 1 param
          MD2_PlayRequestedDot(last_dot);
		  

        }
        last_dot = 0;      
      }
      break;
               
      case STATE_ERROR_1:
	  //@TODO- change to pressed
        RequestToPlayMP3file("press.MP3");
        Current_State = STATE_ERROR_2;
      break;
          
      case STATE_ERROR_2:
        PlayRequestedBits(button_bits);
		button_bits = 0;
        Current_State = STATE_REQUEST_INPUT_1;
      break;

    

  }
   
}


/**
 * @brief this function will be called when enter is pressed during mode
 * @return Void
 */
void MD2_CallModeYesAnswer(void)
{
  last_dot = ENTER;
}

void MD2_CallModeNoAnswer(void)
{

}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void MD2_InputDot(char thisDot)
{
  last_dot = thisDot;
  MD2_Last_Dot=thisDot;
  //Current_State=3;
}

/**
 * @brief  
 * @return Void
 */
void MD2_InputCell(char thisCell)
{
  if(MD2_Last_Dot!=0)
  {
    Last_Cell=thisCell;
    //Current_State=2;
  }
}
