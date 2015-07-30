/**
 * @file PC_Handle.c
 * @brief  This file deals with interacting with the PC, send/ receive messages.  It
 *         also allows you to overwrite the modes used in the function
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "globals.h"

/**
 * @brief This fucntion reads the message in USART_PC_RecievedPacket. It determines
 *        its type and sends the appropriate message to PC
 *        The two possibilities are that you sent 'x' - PC_CMD_INIT - this just gets
 *        response from the system. The other message is 'M' - PC_CMD_NEWMODES
 *        this message type will change the mode file
 * @return Void
 */
void pc_parse_message() {
    unsigned char message_type;
    usart_pc_message_ready = false;
    message_type = usart_pc_received_packet[2];

    switch(message_type) {
        // Send a confirmation that the board received the message
        case PC_CMD_INIT:
            usart_transmit_string_to_pc_from_flash(PSTR("SABT-v2.1"));
            TX_NEWLINE_PC;      
            break;
            // Modify the current modes
        case PC_CMD_NEWMODES:
            pc_requests_to_modify_modes_file();
            break;
            // Incorrect message type
        default:
            log_msg("SABT-INCORRECT MESSAGE TYPE! MUST BE 'M' OR 'x'.");
            break;
    }
}

/**
 * @brief This function will replace the MODES.DAT file with new modes from the 
 *        message variable USART_PC_RecievedPacket.  
 *        The message size can at most be 20 charachters - writing_file_content
 * @return Void
 */
void pc_requests_to_modify_modes_file(void) {
    int t;
    const char* modes_file = "MODES.DAT";
    unsigned char writing_file_content[20];

    // Clear the buffer
    for (t = 0; t < 20; t++)
        writing_file_content[t] = 0x00;

    init_sd_card(false);

    // Copy over the modes in the form <1><2>...<n>. Ignoring the "PCM" header
    for (t = 3; t < usart_pc_received_payload_len; t++)
        writing_file_content[t - 3] = usart_pc_received_packet[t];

    if (replace_the_contents_of_this_file_with(
                (unsigned char*)modes_file, writing_file_content) == 0) {
        usart_transmit_string_to_pc_from_flash(PSTR("SABT-OK"));
        TX_NEWLINE_PC;
    }
    else
    {
        usart_transmit_string_to_pc_from_flash(PSTR("SABT-FAIL"));
        TX_NEWLINE_PC;
    }
}
