/**
 * @file UI_Handle.c
 * @brief This file controls both User interface to Main control and
 *        computer ineractions
 * @ref   tech_report.pdf
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "globals.h"
#include "Modes.h"
#include "audio.h"
#include "common.h"
#include "io.h"
#include "script_common.h"

#include <string.h>

#define MAX_MODE_FILE_LENGTH 128
#define MAX_MODE_NUMBER_DIGITS 2

/**
 * @brief  reads modes from MODES.DAT file and tells computer how many modes and 
 *         what they are
 * @return bool  - return true on succesful completion of transmission, false on 
 *         failure
 */
void ui_check_modes(void) {
    /*
     * The modes.dat contains the numbers of modes that 
     * need to be activated in this device. It contains the 
     * mode numbers of the required modes as follows:
     *    MODENO#MODENO#$ ETC. (Last character is $)
     * For example if we need modes 2, 5 and 14 to be activated:
     * <2><5><14>$
     */
    char file_content[MAX_MODE_FILE_LENGTH];
    int i = 0;
    //const char* modes_file = "MODES.DAT";
    char mode_number[MAX_MODE_NUMBER_DIGITS + 1] = "";
    short mode_number_index = 0;
    int parsed_mode_number = 0;

    //Global variable being initialised
    number_of_modes = 0;

    // Clear file content array  
    for (i = 0; i < MAX_MODE_FILE_LENGTH; i++)
        file_content[i] = 0;

    /* Populate file contents;
    if (read_and_retrieve_file_contents((unsigned char*)modes_file, &file_content[0]) > 0) {
        log_msg("Mode file could not be read\n\r");
        while (1);
    }*/

    /** ACTIVATED MODES ON SABT **/
    strcpy(file_content, "1,2,7,12,6,3,11,4,5,8,9,13,14,15,;");

    // Print file contents to debug stream
    log_msg("Mode file contents: %s", file_content);

    for (i = 0;
            (i < MAX_MODE_FILE_LENGTH) && (file_content[i] != ';') &&
                (number_of_modes < MAX_NUMBER_OF_MODES);
            i++) {

        switch (file_content[i]) {
            case ',':
                //Add NULL terminator to mode number string to be parsed
                mode_number[mode_number_index] = 0;
                mode_number_index = 0;

                parsed_mode_number = atoi(mode_number);
                if (parsed_mode_number == 0)
                    //If "mode number" could not be parsed or is 0, discard it
                    break;

                //Update global mode array
                ui_modes[number_of_modes] = parsed_mode_number;
                number_of_modes++;
                break;
            
            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
            case '8': case '9': case '0':
                mode_number[mode_number_index] = file_content[i];
                mode_number_index++;
                if (mode_number_index > MAX_MODE_NUMBER_DIGITS)
                    // Mode numbers cannot be longer than a certain number of digits
                    break;
                break;

            default:
                log_msg_no_newline("Invalid character in mode file: ");
                SENDBYTE(file_content[i]);
                NEWLINE;
                break;
        }
    }

    // Print the number of modes found to a string
    log_msg("%d modes detected", number_of_modes);

    //Send the actual modes to debug stream
    log_msg_no_newline("Modes enabled: ");
    for (i = 0; i < number_of_modes; i++) {
        log_msg_no_newline("%d, ", ui_modes[i]);
    }
    NEWLINE;

    ui_is_mode_selected = false;
    ui_current_mode_index = -1;
    ui_current_mode_number = -1;
}

/**
 * @brief calculate CRC (cyclic redundancy check)
 * @param message message (?)
 * @return CRC value
 */
uint16_t ui_calculate_crc(unsigned char* message) {
    unsigned char msglen = *(message + 2) - 5; // Not including the checksum bytes
    uint16_t chksum = 0;

    message += 3;
    while (msglen > 1) {
        chksum += (*(message) << 8) | *(message + 1);
        chksum = chksum & 0xffff;
        msglen -= 2;
        message += 2;
    }
    if (msglen > 0) //If the packet size is odd numbered
        chksum = chksum ^ (int)*(message++);
    return chksum;
}

/**
 * @brief  Reads a message in USART_UI_ReceivedPacket. Then determines what type of
 *         message it has received. It then interacts with the mode accordingly and
 *         sends a message of recieval at the end. 
 *
 * UI string: [U][I][msglen][msg_number][msgtype][payload][CRC1][CRC2]
 * msgtypes: 
 * @param  IsPlaying -  bool    determines whether or not an MP3 Files is playing
 * @return  bool - returns true if succesfully parsed, understood, and used message
 */
bool ui_parse_message(bool mp3_is_playing) {
    //First things first, check the CRC
    unsigned char message_len = usart_ui_received_packet[2];
    unsigned char message_type;
    unsigned char adc_message[10];
    uint16_t chksum = ui_calculate_crc((unsigned char*)&usart_ui_received_packet);


    // Check the checksum
    if (chksum == (usart_ui_received_packet[message_len - 2] << 8 
                | usart_ui_received_packet[message_len - 1])) {
        // If correct, store the message elements
        message_type = usart_ui_received_packet[4];

        // Process the message
        if (mp3_is_playing) { //If a MP3 file is being played, only the commands are processed
            if (message_type == 'D') {              // Control key pressed
                ui_control_key_pressed();
                usart_ui_message_ready = false;      // Once control handled, exit
                return true;
            }
        }

        // Handle each type of message separately
        switch(message_type) {
            case 'A':                             // Single braille dot
                io_dot = usart_ui_received_packet[5];
                ui_input_dot_to_current_mode(usart_ui_received_packet[5]);
                break;

            case 'B':  
                ui_input_cell_to_current_mode(usart_ui_received_packet[5]);
                break;

            case 'C':                             // Error message
                log_msg("[ui_parse_message] An error occurred in the UI.");
                break;

            case 'D':                             // Control button
                ui_control_key_pressed();
                break;

            case 'E':                             // Acknowledgement
                // TODO what does this section do? 'E' marked misceaneous in doc
                adc_message[0] = usart_ui_received_packet[5];
                adc_message[1] = usart_ui_received_packet[6];
                adc_message[2] = usart_ui_received_packet[7];
                #ifdef DEBUGMODE
                    log_msg("Analog Input channel stuff.");
                #else
                    usart_transmit_string_to_pc_from_flash(PSTR("Analog Input channel,MSB,LSB :"));
                    sprintf((char*)adc_message,
                        "%d,%d,%d",
                        usart_ui_received_packet[5],
                        usart_ui_received_packet[6],
                        usart_ui_received_packet[7]);
                    usart_transmit_string_to_pc((unsigned char*)&adc_message);
                    TX_NEWLINE_PC;
                    TX_NEWLINE_PC;
                #endif
                break;

            default:
                break;
        }
        // In the end: send acknowledgement to the sender 
        // (with the message number, of course !!!)
    } 
    else { // Checksum not valid
        log_msg("[IO] CRC failed");
        usart_ui_message_ready = false;
        return false;
    }

    usart_ui_message_ready = false;
    return true;
}

/**
 * @brief Handle pressing command buttons (mode control, enter/exit, volume
 * control)
 * @return Void
 */
void ui_control_key_pressed(void) {
    switch(usart_ui_received_packet[5]) {
        case UI_CMD_NONE:
            break;
        
        case UI_CMD_ENT1: // Enter a mode
            io_dot = ENTER;
            if (ui_is_mode_selected)  // then this is a YES command in the mode
                ui_call_mode_yes_answer();
            else {        // Then this command is to select the mode
                if (ui_current_mode_index >= 0) {
                    ui_is_mode_selected = true;
                    io_init();
                    set_language();
                    ui_reset_the_current_mode();
                }
            }
            break;
        
        case UI_CMD_ENT2: // Exit a mode
            io_dot = CANCEL;
            //This might be an exit from mode command or "NO" command in the mode

            //Cancel MP3 prompt
            if (playing_sound) {
                vs1053_skip_play = true;
                io_dot = NO_DOTS;
                return;
            }

            if (usart_ui_received_packet[6] == 69) {
                log_msg("[UI] Long CANCEL detected, going to main menu");
                // @todo free memory here
                io_init();
                quit_mode();
                return;
            }

            else if (ui_is_mode_selected) {   // this is a NO answer to mode
                log_msg("[UI] Short CANCEL detected, calling mode NO function");
                ui_call_mode_no_answer();
            }

            //This has no effect when no mode is selected
            break;
        
        case UI_CMD_MFOR: // Move forward in list of modes
            io_dot = RIGHT;
            if (ui_is_mode_selected)
                ui_call_mode_right();
            else {
                //ui_current_mode_index = (ui_current_mode_index + 1) % number_of_modes;
                ui_current_mode_index = ui_current_mode_index + 1 > number_of_modes - 1 ? 0 : ui_current_mode_index + 1;
                ui_current_mode_number = ui_modes[ui_current_mode_index];
                vs1053_skip_play = true;
                ui_play_intro_current_mode();
            }
            break;
        
        case UI_CMD_MREV: // Move backwards in list of modes
            io_dot = LEFT;
            if (ui_is_mode_selected)
                ui_call_mode_left();
            else {
                ui_current_mode_index = ui_current_mode_index - 1 < 0 ? number_of_modes - 1 : ui_current_mode_index - 1;
                ui_current_mode_number = ui_modes[ui_current_mode_index];
                vs1053_skip_play = true;
                ui_play_intro_current_mode();
            }
            break;

        case UI_CMD_VOLU: // Volume Up
            //only increase sound if you are not playing a sound
            log_msg("[UI] Volume up");
            vs1053_increase_vol();
            break;
        case UI_CMD_VOLD: // Volume down
            //only increase sound if you are not playing a sound
            log_msg("[UI] Volume down");
            vs1053_decrease_vol();
            break;
        default:
            break;
    }
}

/**
 * @brief play the MP3 associated with the current mode
 * @return Void
 */
void ui_play_intro_current_mode(void) {
    char filename[5];
    sprintf(filename, "m%d", ui_current_mode_number);
    play_mp3("",filename);
}

/**
 * @brief input the enter button to the current mode
 * @return Void
 * @todo: use function pointers instead for this!
 */
void ui_call_mode_yes_answer(void) {
    switch(ui_current_mode_number) {
        case 1:
            mode_1_call_mode_yes_answer();
            break;
        case 10:
            mode_10_call_mode_yes_answer();
            break;
        default:
            break;
    }
}

/**
 * @brief input the exit command to the current mode
 * @return Void
 */
void ui_call_mode_no_answer(void) {
    switch(ui_current_mode_number) {
        case 1:
            mode_1_call_mode_no_answer();
            break;
        case 10:
            mode_10_call_mode_no_answer();
            break;
        default:
            break;
    }
}

/**
 * @brief input a dot to the current mode
 * @return Void
 */
void ui_input_dot_to_current_mode(char this_dot) {
    if (ui_is_mode_selected) {
        switch(ui_current_mode_number) {
            case 1:
                mode_1_input_dot(this_dot);
                break;
            case 5:
                mode_5_input_dot(this_dot);
                break;
            case 10:
                mode_10_input_dot(this_dot);
                break;
            default:
                break;
        }
    }
    else
        play_feedback(MP3_INVALID_PATTERN);
}

/**
 * @brief input a cell to the current mode
 * @return Void
 */
void ui_input_cell_to_current_mode(char this_cell)
{
    if (ui_is_mode_selected) {
        switch(ui_current_mode_number) {
            case 1:
                mode_1_input_cell(this_cell);
                break;
            case 5:
                mode_5_input_cell(this_cell);
                break;
            case 10:
                mode_10_input_cell(this_cell);
                break;
            default:
                break;
        }
    }
    else
        play_feedback(MP3_INVALID_PATTERN);
}

/**
 * @brief   Decides which of the three UI modes to go into based on ui_current_mode_number
 * @return  Void
 */
void ui_run_main_of_current_mode(void) {

    if (ui_is_mode_selected){
        switch(ui_current_mode_number) {
            case 1:
                mode_1_main();
                break;
            case 2:
                mode_2_main();
                break;
            case 3:
                mode_3_main();
                break;
            case 4:
                mode_4_main();
                break;
            case 5:
                mode_5_main();
                break;
            case 6:
                mode_6_main();
                break;
            case 7:
                mode_7_main();
                break;
            case 8:
                mode_8_main();
                break;
            case 9:
                mode_9_main();
                break;
            case 10:
                mode_10_main();
                break;
            case 11:
                mode_11_main();
                break;
            case 12:
                mode_12_main();
                break;
            case 13:
                mode_13_main();
                break;
            case 14:
                mode_14_main();
                break;
            case 15:
                mode_15_main();
                break;

            default:
                break;
        }
    }
}

/**
 * @brief input the reset command to the current mode
 * @return Void
 */
void ui_reset_the_current_mode(void) {
    if (ui_is_mode_selected) {
        switch(ui_current_mode_number) {
            case 1:
                mode_1_reset();
                break;
            case 2:
                mode_2_reset();
                break;
            case 3:
                mode_3_reset();
                break;
            case 4:
                mode_4_reset();
                break;
            case 5:
                mode_5_reset();
                break;
            case 6:
                mode_6_reset();
                break;
            case 7:
                mode_7_reset();
                break;
            case 8:
                mode_8_reset();
                break;
            case 9:
                mode_9_reset();
                break;
            case 10:
                mode_10_reset();  
                break;
            case 11:
                mode_11_reset();  
                break;
            case 12:
                mode_12_reset();
                break;
            case 13:
                mode_13_reset();
                break;
            case 14:
                mode_14_reset();
                break;
            case 15:
                mode_15_reset();
                break;

            default:
                break;
        }
    }
}


/**
 * @brief Calls left button handler in mode
 * @param void
 * @return void
 */
void ui_call_mode_left(void) {
    switch (ui_current_mode_number) {
        case 10:
            mode_10_call_mode_left();
            break;
        default:
            break;
    }
}

/**
 * @brief Calls right button handler in mode
 * @param void
 * @return void
 */
void ui_call_mode_right(void) {
    switch (ui_current_mode_number) {
        case 10:
            mode_10_call_mode_right();
            break;  
        default:
            break;
    }
}
