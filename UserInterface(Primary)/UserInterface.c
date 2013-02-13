/**
 * @file UserInterface.c
 * @brief contains main loop for primary interface board
 * @author Nick LaGrow (nlagrow)
 */

#define F_CPU 8000000UL

#include "GlobalsUI.h"

void InitializeUI(void);

/**
 * @brief the main execution loop for the primary ui board
 * Executes an infinite loop, should never return
 * @return 0
 */
int main(void)
{
  InitializeUI();
  SendMsgPayLoad[0]=0x01;
  //SendPacket('D',SendMsgPayLoad,1);
  while(1) //Main loop
  /*
  This loop will take care of all the signal handling from the 
  */
   {
    // Test the transmission to MCU 
    USART_transmitStringToMCU("A message from the Primary UI Board \n");
    
    // TODO:  remove these
    //SetStatLED1(true);
    //SetStatLED2(true); 
    
    // Check to see if timer interrupt has occurred
    if(TMR1_INT)
    {
      TimerRoutine();
    }
  }

return 0;
}

/**
 * @brief initialize basic elements of the UI board
 */
void InitializeUI(void)
{
  //Initialize the Serial comm with the main module
  init_USART_MCU();

  //Initialize the analog ports
  //TODO: why is this commented out?
  //InitializeAnalogPorts();

  //Initialize the digital input/output pins
  Initialize_Digital_IO();

  // Initialize the timer
  initTimer();

  // Enable interrupts
  sei();

  InterfaceType=1;
}
