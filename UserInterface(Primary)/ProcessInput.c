/**
 * @file ProcessInput.c
 * @brief process input from dots
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "GlobalsUI.h"

/**
 * @brief resets cell state - none pressed
 * @return Void
 */
void reset_cell_state(void)
{
  int i = 0;
  
  for(i = 0; i < NUM_DOTS; i++) dots_pressed[i] = false;

  pi_last_dot_pressed = 0;
}

/**
 * @brief Search for the pressed dot values and update the pressed dots array
 *        In the primary UI the braille inputs are configured as digital inputs
 *        Sends a packet in the form [U][I][A]
 * @return Void
 */
void process_the_dot(void)
{
  uint8_t temp_dot = 0;
  bool new_dot_detected = false;

  // PINC: the address of where dots are stored

  // TODO condense all of this

  // Left column of dots (4-6)
  if(!(PINC & (1 << UI_BR1))) // Dot1
  {
    if(!recently_pressed[0])
	{
      new_dot_detected = true;
      temp_dot = 1;
	}
  }

  if(!(PINC & (1 << UI_BR2))) // Dot2
  {
    if(!recently_pressed[1])
	{
      new_dot_detected = true;
      temp_dot = 2;
	}
  }

  if(!(PINC & (1 << UI_BR3))) // Dot3
  {
    if(!recently_pressed[2])
	{
      new_dot_detected = true;
      temp_dot = 3;
	}
  }

  // Right column of dots (1-3)
  if(!(PINC & (1 << UI_BR4))) // Dot4
  {
    if(!recently_pressed[3])
	{
      new_dot_detected = true;
      temp_dot = 4;
	}
  }

  if(!(PINC & (1 << UI_BR5))) // Dot5
  {
    if(!recently_pressed[4])
	{
      new_dot_detected = true;
      temp_dot = 5;
	}
  }

  if(!(PINC & (1 << UI_BR6))) // Dot6
  {
    if(!recently_pressed[5])
	{
      new_dot_detected = true;
      temp_dot = 6;
	}
  }

  // We got an actual input
  if(new_dot_detected)
  {
    // Switch to the correct dot formation (writing mode)
    if(temp_dot > 3) temp_dot -= 3;
    else temp_dot += 3;

    //if(pi_last_dot_pressed == temp_dot) return;

    pi_last_dot_pressed = temp_dot;
    dots_pressed[temp_dot - 1] = true;

    // Send the numeral of the dot pressed (as character)
    mcu_message_payload[0] = '0' + pi_last_dot_pressed;

    // The next two characters are for the row / column position of the braille
    // cell. The primary board only has 1 cell, so these are both '1'
    mcu_message_payload[1] = '1';
    mcu_message_payload[2] = '1';

    // Send the packet (form A = dot input)
    send_packet('A', (char*)&mcu_message_payload, 3);
  }
}

/**
 * @brief Get entire cell value
 *        Processes the stored dot values and generates a 6-bit number to
 *        send over the serial interface.
 * @return Void
 */
void capture_cell_value(void)
{
  int i = 0;

  processed_cell_value = 0x00;
  
  // Get the value of each dot in the cell
  for(i = 0; i < NUM_DOTS; i++)
  {
    if(dots_pressed[i]) processed_cell_value |= _BV(i);
  }

  // 6-bit number representing the pressed dots
  mcu_message_payload[0] = processed_cell_value; 
  
  // Always 1 for Primary UI - only 1 cell
  mcu_message_payload[1] = 1;
  
  // Send the two bytes with message type of 'B' (cell value)
  send_packet('B',(char*)&mcu_message_payload, 2);      
  
  // Reset the cell sate for new letter
  reset_cell_state();
}
