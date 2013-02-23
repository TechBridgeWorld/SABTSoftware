/**
 * @file PC_handle.h
 * @brief definitions for handling PC-MCU interactions
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _PC_HANDLE_H_
#define _PC_HANDLE_H_

#include <stdint.h>
#include <stdbool.h> 

#define PC_CMD_INIT         120   //'x' for Init command
#define PC_CMD_NEWMODES     77    //'M' followed  by new modes string

//Dealing with the user data
//uint16_t PC_calculate_CRC(unsigned char* pstrMsg);
void PC_RequestsToModifyModesFile(void);
void PC_parse_message(void);
void PC_ControlKeyPressed(void);

#endif /* _PC_HANDLE_H_ */
