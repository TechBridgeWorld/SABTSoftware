/**
 * @file ProcessInput.c
 * @brief process input from dots
 * @author Nick LaGrow (nlagrow)
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
 * In the primary UI the braille inputs are configured as digital inputs
 * @return Void
 */
void ProcessTheDot(void)
{
  uint8_t TempDot = 0;
  bool NewDotDetected = false;

  // TODO what is PINC?
  // PINC: the address of where dots are stored

  if(!(PINC & (1 << UI_BR1))) // Dot1
  {
    NewDotDetected = true;
    TempDot = 1;
	SetStatLED1(true);
	USART_transmitByteToMCU('1');
  }

  if(!(PINC & (1 << UI_BR2))) // Dot2
  {
    NewDotDetected = true;
    TempDot = 2;
	USART_transmitByteToMCU('2');
  }

  if(!(PINC & (1 << UI_BR3))) // Dot3
  {
    NewDotDetected = true;
    TempDot = 3;
	USART_transmitByteToMCU('3');
  }

  if(!(PINC & (1 << UI_BR4))) // Dot4
  {
    NewDotDetected = true;
    TempDot = 4;
	USART_transmitByteToMCU('4');
  }

  if(!(PINC & (1 << UI_BR5))) // Dot5
  {
    NewDotDetected = true;
    TempDot = 5;
	USART_transmitByteToMCU('5');
  }

  if(!(PINC & (1 << UI_BR6))) // Dot6
  {
    NewDotDetected = true;
    TempDot = 6;
	USART_transmitByteToMCU('6');
  }

  // TODO what is this doing

/*
  if(NewDotDetected)
  {
    if(InterfaceType == 1)
    {
      //OK this is the default      
    } else if(InterfaceType == 2)
    {
      //OK Flip the pattern
      if(TempDot > 3) TempDot -= 3;
      else TempDot += 3;
    }
    if(PI_LastDotPressed == TempDot) return;

    PI_LastDotPressed = TempDot;
    DotsPressed[TempDot-1] = true;
    // TODO: what are 0x30,0x31,0x31 representing?
    SendMsgPayLoad[0] = 0x30 + PI_LastDotPressed;
    SendMsgPayLoad[1] = 0x31;
    SendMsgPayLoad[2] = 0x31;
    SendPacket('A',(char*)&SendMsgPayLoad,3);

    //SendPacket('E',&SendMsgPayLoad,3);
  }
*/


}

/**
 * @brief Get entire cell value
 * Processes the stored dot values and generates a 6-bit number to
 * send over the serial interface.
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

  SendMsgPayLoad[0]=ProcessedCellValue;   // 6-bit number representing the pressed dots
  SendMsgPayLoad[1]=1;                    // Always 1 for Primary UI - only 1 cell
  SendPacket('B',(char*)&SendMsgPayLoad,2);      // Send the two bytes with messege type of 'B'
  ResetCellState();                       // Reset the cell sate for new letter
}
