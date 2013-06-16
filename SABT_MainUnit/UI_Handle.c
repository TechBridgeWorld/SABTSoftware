/**
 * @file UI_Handle.c
 * @brief This file controls both User interface to Main control and
 *        computer ineractions
 * @ref   tech_report.pdf
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"
#include "Modes.h"

/**
 * @brief  reads modes from MODES.DAT file and tells computer how many modes and 
 *         what they are
 * @return bool  - return true on succesful completion of transmission, false on 
 *         failure
 */
bool ui_check_modes(void)
{
  /*
   * The modes.dat contains the numbers of modes that 
   * need to be activated in this device. It contains the 
   * mode numbers of the required modes as follows:
   *    MODENO#MODENO#$ ETC. (Last character is $)
   * For example if we need modes 2, 5 and 14 to be activated:
   * <2><5><14>$
   */
  unsigned char file_content[100];
  unsigned char mode_id[3];
  // TODO should pc_print_content be of length 4?
  unsigned char pc_print_content[10];
  int i = 0, mode_number = 0;
  bool parsing_mode_descriptor;
  const char* modes_file = "MODES.DAT";

  number_of_modes = 0;

  // Clear file content array  
  for(i = 0; i < 100; i++)
    file_content[i] = 0;

  // Populate file content
  if(read_and_retrieve_file_contents((unsigned char*)modes_file, &file_content[0]) > 0)
  {
    TX_NEWLINE_PC;
    return false;
  }

  // Print file contents to debug stream
  PRINTF(file_content);
  TX_NEWLINE_PC;

  parsing_mode_descriptor = false;

  i = 0;

  // '$' signifies end of file, <i> signifies mode i active
  while(file_content[i] != '$')
  {
    // at end of mode descriptor
    if(file_content[i] == '>')
    {
      ui_modes[number_of_modes] = atoi((char*)&mode_id[0]);
      number_of_modes++;
      parsing_mode_descriptor = false;
    }

    // if not at a mode descriptor, clear everything
    if(!parsing_mode_descriptor)
    {
      mode_id[0] = 0;
      mode_id[1] = 0;
      mode_id[2] = 0;
      mode_number = 0;
    }
    else
    {
      if(mode_number == 3)
      {
        return false;
      }
      mode_id[mode_number++] = file_content[i];
    }

    // if at new mode descriptor
    if(file_content[i] == '<')
    {
      parsing_mode_descriptor = true;
    }
    i++;    
  }

  pc_print_content[0] = 0;
  pc_print_content[1] = 0;

  // Print the number of modes found to a string
  sprintf((char*)&pc_print_content[0], "%d", number_of_modes);

  // Send the actual modes
  PRINTF("And the modes are: ");
  PRINTF(pc_print_content);
  for(i = 0; i < number_of_modes; i++)
  {
    sprintf((char*)&pc_print_content[0], "%d, ", ui_modes[i]);
    PRINTF(pc_print_content);
  }
  TX_NEWLINE_PC;

  return true; 
}

/**
 * @brief calculate CRC (cyclic redundancy check)
 * @param message message (?)
 * @return CRC value
 */
uint16_t ui_calculate_crc(unsigned char* message)
{
  unsigned char msglen = *(message + 2) - 5; // Not including the checksum bytes
  uint16_t chksum = 0;

  message += 3;
  while(msglen > 1)
  {
    chksum += (*(message) << 8) | *(message + 1);
    chksum = chksum & 0xffff;
    msglen -= 2;
    message += 2;
  }
  if(msglen > 0) //If the packet size is odd numbered
  {
    chksum = chksum ^ (int)*(message++);
  }
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
bool ui_parse_message(bool mp3_is_playing)
{
  //First things first, check the CRC
  unsigned char message_len = usart_ui_received_packet[2];
  unsigned char message_number;
  unsigned char message_type;
  unsigned char adc_message[10];
  uint16_t chksum = ui_calculate_crc((unsigned char*)&usart_ui_received_packet);

  // Check the checksum
  if (chksum == (usart_ui_received_packet[message_len - 2] << 8 
        | usart_ui_received_packet[message_len - 1]))
  {
    // If correct, store the message elements
    message_number = usart_ui_received_packet[3];
    message_type = usart_ui_received_packet[4];

    // Process the message
    if(mp3_is_playing) //If a MP3 file is being played, only the commands are processed
    {
      if(message_type == 'D')               // Control key pressed
      {
        ui_control_key_pressed();
        usart_ui_message_ready = true;      // Once control handled, exit
        return true;
      }
    }

    // Handle each type of message separately
    switch(message_type)
    {
      case 'A':                             // Single braille dot
        ui_input_dot_to_current_mode(usart_ui_received_packet[5]);
        break;
      case 'B':  
        ui_input_cell_to_current_mode(usart_ui_received_packet[5]);
        break;
      case 'C':                             // Error message
        PRINTF("[ui_parse_message] An error occurred in the UI.");
        break;
      case 'D':                             // Control button
        //PRINTF("CONTROL BUTTON PRESSED");
        ui_control_key_pressed();
        break;
      case 'E':                             // Acknowledgement
        // TODO what does this section do? 'E' marked misceaneous in doc
        adc_message[0] = usart_ui_received_packet[5];
        adc_message[1] = usart_ui_received_packet[6];
        adc_message[2] = usart_ui_received_packet[7];
        usart_transmit_string_to_pc_from_flash(PSTR("Analog Input channel,MSB,LSB :"));
        sprintf((char*)adc_message,
            "%d,%d,%d",
            usart_ui_received_packet[5],
            usart_ui_received_packet[6],
            usart_ui_received_packet[7]);
        usart_transmit_string_to_pc((unsigned char*)&adc_message);
        TX_NEWLINE_PC;
        TX_NEWLINE_PC;
        break;
      default:
        break;
    }
    // In the end: send acknowledgement to the sender 
    // (with the message number, of course !!!)
  } 
  else // Checksum not valid
  {
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
void ui_control_key_pressed(void)
{
  switch(usart_ui_received_packet[5])
  {
    case UI_CMD_NONE:
      break;
    
    case UI_CMD_ENT1: // Enter a mode
      if(!ui_mode_selected) //Then this command is to select the mode
      {
        if(ui_selected_mode >= 0)
        {
          ui_mode_selected = true;
          ui_reset_the_current_mode();
        }
        else
        {
          //request_to_play_mp3_file("ERR1.MP3");
        }
      }
      else //Then this the "YES" command in the mode, so call the function in the mode
      {
        ui_call_mode_yes_answer();
      }
      break;
    
    case UI_CMD_ENT2: // Exit a mode
      //This might be an exit from mode command or "NO" command in the mode
      PRINTF("Received CANCEL from keypad");
      NEWLINE;
      if(ui_mode_selected) 
      {
        PRINTF("A mode is selected");
        NEWLINE;
        // If the next byte is 'E', this is exit command 
        // (when the user pressed E2 for more than 5 secs)
        if(usart_ui_received_packet[6] == 69) 
        {
          PRINTF("Long press detected, going to main menu");
          NEWLINE;
          ui_mode_selected = false;
          request_to_play_mp3_file("MM.MP3");
        }
        else //Then this a "NO" answer, call the mode function for this
        {
          PRINTF("Short press detected, calling mode NO function");
          NEWLINE;
          ui_call_mode_no_answer();
        }
      }
      //This has no effect when no mode is selected
      break;
    
    case UI_CMD_MFOR: // Move forward in list of modes
      if(!ui_mode_selected)
      {
        //ui_selected_mode = (ui_selected_mode + 1) % number_of_modes;
        ui_selected_mode = ui_selected_mode + 1 > number_of_modes - 1 ? 0 : ui_selected_mode + 1;
        ui_current_mode = ui_modes[ui_selected_mode];
        vs1053_skip_play = true;
        ui_play_intro_current_mode();
      }
      break;
    
    case UI_CMD_MREV: // Move backwards in list of modes
      if(!ui_mode_selected)
      {
        ui_selected_mode = ui_selected_mode - 1 < 0 ? number_of_modes - 1 : ui_selected_mode - 1;
        ui_current_mode = ui_modes[ui_selected_mode];
        vs1053_skip_play = true;
        ui_play_intro_current_mode();
      }
      break;

    case UI_CMD_VOLU: // Volume Up
      //only increase sound if you are not playing a sound
      if(!playing_sound){
        usart_transmit_string_to_pc_from_flash(PSTR("Vol UP pressed"));
        TX_NEWLINE_PC;
        vs1053_increase_vol();
      }
      break;
    case UI_CMD_VOLD: // Volume down
      //only increase sound if you are not playing a sound
      if(!playing_sound){
        usart_transmit_string_to_pc_from_flash(PSTR("Vol DOWN pressed"));
        TX_NEWLINE_PC;
        vs1053_decrease_vol();
      }
      break;
    default:
      break;
  }
}

/**
 * @brief play the MP3 associated with the current mode
 * @return Void
 */
void ui_play_intro_current_mode(void)
{
  char buf[11];
  if(ui_current_mode <= number_of_modes)
  {
    sprintf(buf, "MD%i.MP3", ui_current_mode);
    request_to_play_mp3_file(buf);
  }
  else
    request_to_play_mp3_file("ERR1.MP3");
}

/**
 * @brief input the enter button to the current mode
 * @return Void
 */
void ui_call_mode_yes_answer(void)
{
  switch(ui_current_mode)
  {
    case 1:
      md1_call_mode_yes_answer();
      break;
    case 2:
      md2_call_mode_yes_answer();
      break;
    case 3:
      md3_call_mode_yes_answer();
      break;
    case 4:
      md4_call_mode_yes_answer();
      break;
    case 5:
      md5_call_mode_yes_answer();
      break;
    case 6:
      md6_call_mode_yes_answer();
      break;
    case 7:
      md7_call_mode_yes_answer();
      break;
    default:
      break;
  }
}

/**
 * @brief input the exit command to the current mode
 * @return Void
 */
void ui_call_mode_no_answer(void)
{
  switch(ui_current_mode)
  {
    case 1:
      md1_call_mode_no_answer();
      break;
    case 2:
      md2_call_mode_no_answer();
      break;
    case 3:
      md3_call_mode_no_answer();
      break;
    case 4:
      md4_call_mode_no_answer();
      break;
    case 5:
      md5_call_mode_no_answer();
      break;
    case 6:
      md6_call_mode_no_answer();
      break;
    case 7:
      md7_call_mode_no_answer();
      break;
    default:
      break;
  }
}

/**
 * @brief input a dot to the current mode
 * @return Void
 */
void ui_input_dot_to_current_mode(char this_dot)
{
  switch(ui_current_mode)
  {
    case 1:
      md1_input_dot(this_dot);
      break;
    case 2:
      md2_input_dot(this_dot);
      break;
    case 3:
      md3_input_dot(this_dot);
      break;
    case 4:
      md4_input_dot(this_dot);
      break;
    case 5:
      md5_input_dot(this_dot);
      break;
    case 6:
      md6_input_dot(this_dot);
      break;
    case 7:
      md7_input_dot(this_dot);
      break;
    default:
      break;
  }
}

/**
 * @brief input a cell to the current mode
 * @return Void
 */
void ui_input_cell_to_current_mode(char this_cell)
{
  switch(ui_current_mode)
  {
    case 1:
      md1_input_cell(this_cell);
      break;
    case 2:
      md2_input_cell(this_cell);
      break;
    case 3:
      md3_input_cell(this_cell);
      break;
    case 4:
      md4_input_cell(this_cell);
      break;
    case 5:
      md5_input_cell(this_cell);
      break;
    case 6:
      md6_input_cell(this_cell);
      break;
    case 7:
      md7_input_cell(this_cell);
      break;
    default:
      break;
  }
}

/**
 * @brief   Decides which of the three UI modes to go into based on ui_current_mode
 * @return  Void
 */
void ui_run_main_of_current_mode(void)
{
  if(ui_mode_selected){
    switch(ui_current_mode)
    {
      case 1:
        md1_main();
        break;
      case 2:
        md2_main();
        break;
      case 3:
        md3_main();
        break;
      case 4:
        md4_main();
        break;
      case 5:
        md5_main();
        break;
      case 6:
        md6_main();
        break;
      case 7:
        md7_main();
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
void ui_reset_the_current_mode(void)
{
  if(ui_mode_selected){
    switch(ui_current_mode)
    {
      case 1:
        md1_reset();
        break;
      case 2:
        md2_reset();
        break;
      case 3:
        md3_reset();
        break;
      case 4:
        md4_reset();
        break;
      case 5:
        md5_reset();
        break;
      case 6:
        md6_reset();
        break;
      case 7:
        md7_reset();
        break;
      default:
        break;
    }
  }
}
