#ifndef _UIHANDLE_H_
#define _UIHANDLE_H_

#include <stdint.h>
#include <stdbool.h> 

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

//Dealin with the user data
bool UI_CheckModes(void);
uint16_t UI_calculate_CRC(unsigned char* pstrMsg);
bool UI_parse_message(bool IsPlaying);
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

#endif