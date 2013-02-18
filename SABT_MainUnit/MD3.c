/**
 * @file MD3.c
 * @brief mode 3 code
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

int MD3_Current_State;
char MD3_Last_Dot;
//Current_State - Defines the status of the mode
/* Vales and meanings
0 - Just started (in the begining, play the Welcome message)
1 - Waiting for user input
2 - Last user input processed
*/

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
  MD3_Current_State=0;
}

/**
 * @brief  Step through the main stages in the code.
 * @return Void
 */
void MD3_Main(void)
{
  switch(MD3_Current_State)
  {
    case 0:
      RequestToPlayMP3file("MD3INT.MP3");
      MD3_Current_State=1;
      break;
    case 1:
      break;
    case 2:
      MD3_PlayRequestedDot();
      MD3_Current_State=1;
      break;
  }
}

void MD3_CallModeYesAnswer(void)
{

}

void MD3_CallModeNoAnswer(void)
{

}

/**
 * @brief  Set the dot the from input
 * @return Void
 */
void MD3_InputDot(char thisDot)
{
  MD3_Last_Dot=thisDot;
  MD3_Current_State=2;
}

/**
 * @brief
 * @return Void
 */
void MD3_InputCell(char thisCell)
{

}
