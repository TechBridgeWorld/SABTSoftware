/** 
 * @file USART_Keypad.c
 * @brief Deals with USART communication from the main board - please update 
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 * @author Nick LaGrow (nlagrow)
 */

#include "Globals.h"

bool usart_ui_header_received, usart_ui_length_received;
unsigned char usart_ui_prefix[3];
unsigned char usart_ui_receive_msgcnt;
unsigned char usart_ui_received_payload_len;


/**
 * @brief Initializes the baud communication over USART.
 * @return Void
 */
void init_usart_keypad(void) {
    UCSR1B = 0x00; //disable while setting baud rate
    UCSR1A = 0x00;
    UCSR1C = 0x06;
    UBRR1L = 0x19; //set baud rate lo
    UBRR1H = 0x00; //set baud rate hi 19,200 baud with 8MHz clock
    UCSR1B = 0x98; //RXCIE1=1, RXEN1=1, TXEN1=1
    usart_ui_length_received = false;
    usart_ui_header_received = false;
    usart_ui_message_ready = false;
}


/**
 * @brief   Receives message stored in globabl usart_keypad_received_data
 *          Then proceeds to decode message, use its value, and allow for more 
 *          messages to be sent. usart_ui_prefix - mini shift register which shifts
 *          from 2 -> 1 -> 0
 *          Transfers data from USART_Keypad_Reiceved_Data->usart_ui_received_packet
 * @ref  tech_report.pdf
 * @return Void
 */
void usart_keypad_receive_action(void) {

    usart_keypad_data_ready = false;

    // If no header has been found, build it
    if(!usart_ui_header_received) {
        usart_ui_prefix[2] = usart_keypad_received_data;
        usart_ui_prefix[0] = usart_ui_prefix[1];
        usart_ui_prefix[1] = usart_ui_prefix[2];

        if((usart_ui_prefix[0] == 'U') && (usart_ui_prefix[1] == 'I')) {
            usart_ui_header_received = true;
            usart_ui_received_packet[0] = usart_ui_prefix[0];
            usart_ui_received_packet[1] = usart_ui_prefix[1];
            usart_ui_receive_msgcnt = 2;
            usart_ui_length_received = false;
        }
    }
    // Get the length of the payload
    else if(!usart_ui_length_received) {
        if(usart_ui_receive_msgcnt == 2) {
            usart_ui_received_payload_len = usart_keypad_received_data;
            usart_ui_received_packet[2] = usart_keypad_received_data;
            usart_ui_length_received = true;
            usart_ui_receive_msgcnt++;
        }
        else {
            usart_ui_header_received = false;
        }
    }
    // Build the actual message
    else {
        usart_ui_received_packet[usart_ui_receive_msgcnt++] = usart_keypad_received_data;

        if (usart_ui_receive_msgcnt >= 19) {
            usart_ui_message_ready = false;
            usart_ui_header_received = false;
            usart_ui_length_received = false;
        }

        // Full message has been received
        if(usart_ui_receive_msgcnt == usart_ui_received_payload_len) {
            usart_ui_message_ready = true;
            usart_ui_header_received = false;
            usart_ui_length_received = false;
        }
    }
}

/**
 * @brief Transmits byte data from MC --> UI over UDR1
 * @param data - unsigned char, byte to transmit to UI
 * @return Void
 */
void usart_transmit_byte_to_keypad(unsigned char data) {
    while (!(UCSR1A & (1<<UDRE1)));   // Wait for empty transmit buffer
    UDR1 = data;                      // Start transmition
}

/**
 * @brief Transmits string data from MC Flash --> UI over UDR1
 * @param string - char*, String to transmit to UI
 * @return Void
 */
void usart_transmit_string_to_keypad_from_flash(char* string) {
    while (pgm_read_byte(&(*string)))
        usart_transmit_byte_to_keypad(pgm_read_byte(&(*string++)));
}

/**
 * @brief Transmits string data from MC --> UI over UDR1
 * @param string - char*, String to transmit to UI
 * @return Void
 */
void usart_transmit_string_to_keypad(unsigned char* string) {
    while (*string)
        usart_transmit_byte_to_keypad(*string++);
}
