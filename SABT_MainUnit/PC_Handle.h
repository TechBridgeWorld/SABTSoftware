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

#define PC_CMD_INIT         'x'    //'x' for Init command
#define PC_CMD_NEWMODES     'M'    //'M' followed  by new modes string

//Dealing with the user data
//uint16_t PC_calculate_CRC(unsigned char* pstrMsg);
void pc_requests_to_modify_modes_file(void);
void pc_parse_message(void);
void pc_control_key_pressed(void);

#endif /* _PC_HANDLE_H_ */
