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

//Dealing with the user data
void ui_check_modes(void);
uint16_t ui_calculate_crc(unsigned char* message);
bool ui_parse_message(bool mp3_is_playing);
void ui_control_key_pressed(void);

//Current mode related functions
void ui_play_intro_current_mode(void);
void ui_call_mode_yes_answer(void);
void ui_call_mode_no_answer(void);
void ui_input_dot_to_current_mode(char this_dot);
void ui_input_cell_to_current_mode(char this_cell);
void ui_run_main_of_current_mode(void);
void ui_reset_the_current_mode(void);
void ui_call_mode_left(void);
void ui_call_mode_right(void);

//MP3 play related functions
//void RequestToPlayMP3file(const char* thisFile);

#endif /* _UI_HANDLE_H_ */
