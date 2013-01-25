/**
 * @file MD1.c
 * @brief code for the Primary User Interface Mode
 * @author Nick LaGrow (nlagrow)
 */

#include "Globals.h"

int Current_State;
char Last_Dot;
//Current_State - Defines the status of the mode
/* Vales and meanings
0 - Just started (in the begining, play the Welcome message)
1 - Waiting for user input
2 - Last user input processed

*/

void PlayRequestedDot(void)
{
	switch(Last_Dot)
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

void MD1_Reset(void)
{
	Current_State=0;
}

void MD1_Main(void)
{
	switch(Current_State)
	{
		case 0:
      // Play the inrtoductory message for Mode 1
			RequestToPlayMP3file("MD1INT.MP3");
			Current_State = 1;
			break;
		case 1:
			break;
		case 2:
			PlayRequestedDot();
			Current_State = 1;
			break;
	}
}

void MD1_CallModeYesAnswer(void)
{

}

void MD1_CallModeNoAnswer(void)
{

}

void MD1_InputDot(char thisDot)
{
	Last_Dot=thisDot;
	Current_State=2;
}

void MD1_InputCell(char thisCell)
{

}
