#ifndef _PCHANDLE_H_
#define _PCHANDLE_H_

#include <stdint.h>
#include <stdbool.h> 

#define PC_CMD_INIT 		120	//'x' for Init command
#define PC_CMD_NEWMODES		77  //'M' followed  by new modes string


//Dealin with the user data
//uint16_t PC_calculate_CRC(unsigned char* pstrMsg);
void PC_RequestsToModifyModesFile(void);
bool PC_parse_message(void);
void PC_ControlKeyPressed(void);




#endif
