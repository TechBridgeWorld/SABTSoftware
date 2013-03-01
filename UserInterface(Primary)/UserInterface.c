/**
 * @file UserInterface.c
 * @brief contains main loop for primary interface board
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#define F_CPU 8000000UL

#include "GlobalsUI.h"

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

//reset
/*ISR(_VECTOR(1)){
  SetStatLED2(true);
}*/

/* TEST VECTORS
ISR(_VECTOR(2)){
  SetStatLED2(true);
}
ISR(_VECTOR(3)){
  SetStatLED2(true);
}
ISR(_VECTOR(4)){
  SetStatLED2(true);
}
ISR(_VECTOR(5)){
  SetStatLED2(true);
}
ISR(_VECTOR(6)){
  SetStatLED2(true);
}
ISR(_VECTOR(7)){
  SetStatLED2(true);
}
ISR(_VECTOR(8)){
  SetStatLED2(true);
}
ISR(_VECTOR(9)){
  SetStatLED2(true);
}
ISR(_VECTOR(10)){
  SetStatLED2(true);
}*/
/*ISR(_VECTOR(11)){
  if(!LED_STAT){
    SetStatLED2(true);
    SetStatLED1(false);
    LED_STAT=true;
  }else{
    SetStatLED2(false);
    SetStatLED1(true);
    LED_STAT=false;
  }
}*/
/*ISR(_VECTOR(12)){
  SetStatLED2(true);
}
ISR(_VECTOR(13)){
  SetStatLED2(true);
}
ISR(_VECTOR(14)){
  SetStatLED2(true);
}
ISR(_VECTOR(15)){
  SetStatLED2(true);
}
ISR(_VECTOR(16)){
  SetStatLED2(true);
}
ISR(_VECTOR(17)){
  SetStatLED2(true);
}
ISR(_VECTOR(18)){
  SetStatLED2(true);
}
ISR(_VECTOR(19)){
  SetStatLED2(true);
}*/
ISR(_VECTOR(20)){
  transmit_complete = true;
  SetStatLED2(true);
  //SetStatLED2(true);
}/*
ISR(_VECTOR(21)){
  SetStatLED2(true);
}
ISR(_VECTOR(22)){
  SetStatLED2(true);
}
ISR(_VECTOR(23)){
  SetStatLED2(true);
}
ISR(_VECTOR(24)){
  SetStatLED2(true);
}
ISR(_VECTOR(25)){
  SetStatLED2(true);
}
ISR(_VECTOR(26)){
  SetStatLED2(true);
}
*/

/**
 * @brief the main execution loop for the primary ui board
 * Executes an infinite loop, should never return
 * @return 0
 */
int main(void)//
{
  InitializeUI();

  while(1) // Main loop
  // This loop will take care of all the signal handling from the 
  {
    // Test the transmission to MCU 
    //USART_transmitStringToMCU((unsigned char*)"A message from the Primary UI Board.\r\n");

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
 * @return Void
 */
void InitializeUI(void)
{
  //Initialize the Serial comm with the main module
  init_USART_MCU();

  //Initialize the analog ports for intermediate/advanced boards only (double check?)
  //InitializeAnalogPorts();

  //Initialize the digital input/output pins
  Initialize_Digital_IO();

  // Initialize the timer
  initTimer();

  // Enable interrupts
  sei();

  InterfaceType=1;
}
