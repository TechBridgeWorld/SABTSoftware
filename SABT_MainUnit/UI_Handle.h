/**
 * @file UI_Handle.h
 * @brief Handle UI-MCU interactions
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _UI_HANDLE_H_
#define _UI_HANDLE_H_

#include <stdint.h>
#include <stdbool.h> 
#include <stdlib.h>

#define UI_CMD_NONE 0
#define UI_CMD_ENT1 1
#define UI_CMD_ENT2 2
#define UI_CMD_MFOR 3
#define UI_CMD_MREV 4
#define UI_CMD_VOLU 5
#define UI_CMD_VOLD 6

volatile bool UI_MP3_file_Pending;
volatile bool UI_MODE_SELECTED;
volatile char UI_Current_Mode;
volatile char UI_Selected_Mode;

//Dealing with the user data
bool UI_CheckModes(void);
uint16_t UI_calculate_CRC(unsigned char* pstrMsg);
bool UI_parse_message(bool mp3_is_playing);
void UI_ControlKeyPressed(void);

//Current mode related functions
void UI_Play_Intro_Currentmode(void);
void UI_CallModeYesAnswer(void);
void UI_CallModeNoAnswer(void);
void UI_InputDotToCurrentMode(char thisDot);
void UI_InputCellToCurrentMode(char thisCell);
void UI_RunMainOfCurrentMode(void);
void UI_ResetTheCurrentMode(void);

//MP3 play related functions
//void RequestToPlayMP3file(const char* thisFile);

#endif /* _UI_HANDLE_H_ */
