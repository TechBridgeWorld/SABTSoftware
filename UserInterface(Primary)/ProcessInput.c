/**
 * @file ProcessInput.c
 * @brief process input from dots
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "GlobalsUI.h"

char ProcessedCellValue;
uint8_t PI_LastDotPressed;

/**
 * @brief resets cell state - none pressed
 * @return Void
 */
void ResetCellState(void)
{
  int i = 0;
  
  for(i=0; i < 6; i++)
  {
    DotsPressed[i]=false;
  }

  PI_LastDotPressed=0;
}

/**
 * @brief Search for the pressed dot values and update the pressed dots array
 *        In the primary UI the braille inputs are configured as digital inputs
 *        Sends a packet in the form [U][I][A]
 * @return Void
 */
void ProcessTheDot(void)
{
  uint8_t temp_dot = 0;
  bool new_dot_detected = false;

  // PINC: the address of where dots are stored

  // Left column of dots (4-6)
  if(!(PINC & (1 << UI_BR1))) // Dot1
  {
    new_dot_detected = true;
    temp_dot = 1;
  }

  if(!(PINC & (1 << UI_BR2))) // Dot2
  {
    new_dot_detected = true;
    temp_dot = 2;
  }

  if(!(PINC & (1 << UI_BR3))) // Dot3
  {
    new_dot_detected = true;
    temp_dot = 3;
  }

  // Right column of dots (1-3)
  if(!(PINC & (1 << UI_BR4))) // Dot4
  {
    new_dot_detected = true;
    temp_dot = 4;
  }

  if(!(PINC & (1 << UI_BR5))) // Dot5
  {
    new_dot_detected = true;
    temp_dot = 5;
  }

  if(!(PINC & (1 << UI_BR6))) // Dot6
  {
    new_dot_detected = true;
    temp_dot = 6;
  }

  // We got an actual input
  if(new_dot_detected)
  {
    // Switch to the correct dot formation (writing mode)
    if(temp_dot > 3) temp_dot -= 3;
    else temp_dot += 3;

    //if(PI_LastDotPressed == temp_dot) return;

    PI_LastDotPressed = temp_dot;
    DotsPressed[temp_dot - 1] = true;

    // Send the numeral of the dot pressed (as character)
    SendMsgPayLoad[0] = '0' + PI_LastDotPressed;

    // The next two characters are for the row / column position of the braille
    // cell. The primary board only has 1 cell, so these are both '1'
    SendMsgPayLoad[1] = '1';
    SendMsgPayLoad[2] = '1';

    // Send the packet (form A = dot input)
    send_packet('A', (char*)&SendMsgPayLoad, 3);
  }
}

/**
 * @brief Get entire cell value
 *        Processes the stored dot values and generates a 6-bit number to
 *        send over the serial interface.
 * @return Void
 */
void CaptureCellValue(void)
{
  int i = 0;

  ProcessedCellValue = 0x00;
  
  for(i=0; i<6; i++)
  {
    if(DotsPressed[i])
    {
      ProcessedCellValue |= _BV(i);
    }
  }

  SendMsgPayLoad[0]=ProcessedCellValue;          // 6-bit number representing the pressed dots
  SendMsgPayLoad[1]=1;                           // Always 1 for Primary UI - only 1 cell
  send_packet('B',(char*)&SendMsgPayLoad,2);      // Send the two bytes with message type of 'B'
  ResetCellState();                              // Reset the cell sate for new letter
}
