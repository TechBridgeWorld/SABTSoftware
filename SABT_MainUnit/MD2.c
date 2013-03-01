/**
 * @file MD2.c
 * @brief mode 2 code
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"
#define ENTER 140

#define A_BITS 0b00000001
#define B_BITS 0b00000011
#define C_BITS 0b00001001
#define D_BITS 0b00011001
#define E_BITS 0b00010001
#define F_BITS 0b00001011
#define G_BITS 0b00011011
#define H_BITS 0b00010011
#define I_BITS 0b00001010
#define J_BITS 0b00011010
#define K_BITS 0b00000101
#define L_BITS 0b00000111
#define M_BITS 0b00001101
#define N_BITS 0b00011101
#define O_BITS 0b00010101
#define P_BITS 0b00001111
#define Q_BITS 0b00011111
#define R_BITS 0b00010111
#define S_BITS 0b00001110
#define T_BITS 0b00011110
#define U_BITS 0b00100101
#define V_BITS 0b00100111
#define W_BITS 0b00111010
#define X_BITS 0b00101101
#define Y_BITS 0b00111101
#define Z_BITS 0b00110101



int Current_State;
char Last_Cell;
char MD2_Last_Dot;
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
  switch(last_cell)
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
  }
}

/**
 * @brief  Given a bitmask, in Last_Cell, play the corresponding letter
 *         sound file
 * @param bits - char, bitmask that should representa  braile letter 
 * @return Void
 */
void PlayRequestedBits(char bits)
{
    switch(bits)
    {
        case A_BITS:
            RequestToPlayMP3file("MD2_a.MP3");
            break;
        case B_BITS:
            RequestToPlayMP3file("MD2_b.MP3");
            break;
        case C_BITS:
            RequestToPlayMP3file("MD2_c.MP3");
            break;
        case D_BITS:
            RequestToPlayMP3file("MD2_d.MP3");
            break;
        case E_BITS:
            RequestToPlayMP3file("MD2_e.MP3");
            break;
        case F_BITS:
            RequestToPlayMP3file("MD2_f.MP3");
            break;
        case G_BITS:
            RequestToPlayMP3file("MD2_g.MP3");
            break;
        case H_BITS:
            RequestToPlayMP3file("MD2_h.MP3");
            break;
        case I_BITS:
            RequestToPlayMP3file("MD2_i.MP3");
            break;
        case J_BITS:
            RequestToPlayMP3file("MD2_j.MP3");
            break;
        case K_BITS:
            RequestToPlayMP3file("MD2_k.MP3");
            break;
        case L_BITS:
            RequestToPlayMP3file("MD2_l.MP3");
            break;
        case M_BITS:
            RequestToPlayMP3file("MD2_m.MP3");
            break;
        case N_BITS:
            RequestToPlayMP3file("MD2_n.MP3");
            break;
        case O_BITS:
            RequestToPlayMP3file("MD2_o.MP3");
            break;
        case P_BITS:
            RequestToPlayMP3file("MD2_p.MP3");
            break;
        case Q_BITS:
            RequestToPlayMP3file("MD2_q.MP3");
            break;
        case R_BITS:
            RequestToPlayMP3file("MD2_r.MP3");
            break;
        case S_BITS:
            RequestToPlayMP3file("MD2_s.MP3");
            break;
        case T_BITS:
            RequestToPlayMP3file("MD2_t.MP3");
            break;
        case U_BITS:
            RequestToPlayMP3file("MD2_u.MP3");
            break;
        case V_BITS:
            RequestToPlayMP3file("MD2_v.MP3");
            break;
        case W_BITS:
            RequestToPlayMP3file("MD2_w.MP3");
            break;
        case X_BITS:
            RequestToPlayMP3file("MD2_x.MP3");
            break;
        case Y_BITS:
            RequestToPlayMP3file("MD2_y.MP3");
            break;
        case Z_BITS:
            RequestToPlayMP3file("MD2_z.MP3");
            break;
        default:
            RequestToPlayMP3file("MD2ER1.MP3");
            break;
    }
}


/**
 * @brief  Given a char, in Last_Cell, play the corresponding number
 *         sound file
 * @return Void
 */
void MD2_PlayRequestedDot(char MD2_Last_Dot)
{
  switch(MD2_Last_Dot)
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
    switch(current_letter)
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
    }
    
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
      button_bits = 0;
      letter_set = 0;
      current_count = 0;
      random_count = 0;
      current_random_letter = 0;
      initial_letter = 'a';
      use_random_letter = 0;
      RequestToPlayMP3file("MD2INT.MP3");
      Current_State = SET_LETTER_VALS;
      break;
          
    case SET_LETTER_VALS:
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
        Current_State = STATE_WAIT_INPUT;
      }
      break;
          
    case STATE_WAIT_INPUT:
      if(last_dot != 0){
        //The user just input their word

        if(last_dot == ENTER){
          //they got the word right, change letter unless you are at 5
          //alredy then enter random mode.

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
