/**
 * @file UserInterface.c
 * @brief contains main loop for primary interface board
 * @author Nick LaGrow (nlagrow)
 */

#define F_CPU 8000000UL

#include "GlobalsUI.h"
#include <avr/wdt.h>

void InitializeUI(void);

void delay10()
{
  int i = 0;
  int j=0;
  for(i=0;i<100;i++){
    for(j=0;j<100;j++){
      DDRD &= ~_BV(UI_VOLDOWN);
    }
  }
  //PORTD &= ~_BV(UI_STAT1);
  return;
}

//uint8_t mcusr_mirror __attribute__ ((section (".noinit")));


// watchdog timer interrupt handler (?)
// attempt to catch wdt interrupt
ISR(_VECTOR(7)){
  SetStatLED2(true);
}


/**
 * @brief the main execution loop for the primary ui board
 * Executes an infinite loop, should never return
 * @return 0
 */
int main(void)//
{
  // Attempt to turn WDT to interrupt instead of reset mode
  //MCUSR &= ~(1<<3);
  //WDTCSR |= (1<<6);
  //WDTCSR &= ~(1<<3);

  // Attempt to turn off wdt
  //wdt_reset();
  //wdt_disable();

  InitializeUI();

  // Enough delay to not allow LED2 to turn on unless wdt is handled
  // used for testing: if the second light turns on, we know we disabled watchdog
  delay10();
  delay10();
  delay10();
  delay10();
  delay10();
  delay10();
  delay10();
  delay10();
  delay10();
  delay10();

  SetStatLED2(true);

  while(1){};


/*
  InitializeUI();
  SendMsgPayLoad[0]=0x01;
  //SendPacket('D',SendMsgPayLoad,1);
  
  delay10();
  //SetStatLED2(false);
  SetStatLED1(false);
  
  while(1) //Main loop
  // This loop will take care of all the signal handling from the 
  {
    // Test the transmission to MCU 
    USART_transmitStringToMCU((unsigned char*)"A message from the Primary UI Board \n");
    
    // TODO:  remove these
    //SetStatLED1(true);
    //SetStatLED2(true); 
    
    // Check to see if timer interrupt has occurred
    if(TMR1_INT)
    {
      TimerRoutine();
    }
  }
*/
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
