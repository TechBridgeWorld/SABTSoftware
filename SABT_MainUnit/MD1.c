/**
 * @file MD1.c
 * @brief code for the Primary User Interface Mode
 * @author Nick LaGrow (nlagrow)
 */

#include "Globals.h"
#include "MD1.h"

int Current_State;  // current state of the mode
char Last_Dot;      // char representing last big dot pressed

/**
 * @brief echo the number of the dot pressed
 * @return Void
 */
void PlayRequestedDot(void)
{
  // TODO default case?
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

/**
 * @brief Reset mode 1 to starting state
 * Should be useful for error handling
 * @return Void
 */
void MD1_Reset(void)
{
	Current_State = STATE_INITIAL;
}

/**
 * @brief main code for Mode 1
 * @return Void
 */
void MD1_Main(void)
{
	switch(Current_State)
	{
		case STATE_INITIAL:
      DPRINTF("[MD1] Entering MD1\n");
      // Play the introductory message for Mode 1
			RequestToPlayMP3file("MD1INT.MP3");
			Current_State = STATE_WAIT_INPUT;
			break;
		case STATE_WAIT_INPUT:
      DPRINTF("[MD1] State 1\n");
			break;
		case STATE_PROC_INPUT:
      DPRINTF("[MD1] State 2 >> 1: playing dot name\n");
			PlayRequestedDot();
			Current_State = STATE_WAIT_INPUT;
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
	Current_State = STATE_PROC_INPUT;
}

void MD1_InputCell(char thisCell)
{

}
