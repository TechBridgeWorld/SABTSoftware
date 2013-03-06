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
  set_stat_led2(true);
}*/

/* TEST VECTORS
ISR(_VECTOR(2)){
  set_stat_led2(true);
}
ISR(_VECTOR(3)){
  set_stat_led2(true);
}
ISR(_VECTOR(4)){
  set_stat_led2(true);
}
ISR(_VECTOR(5)){
  set_stat_led2(true);
}
ISR(_VECTOR(6)){
  set_stat_led2(true);
}
ISR(_VECTOR(7)){
  set_stat_led2(true);
}
ISR(_VECTOR(8)){
  set_stat_led2(true);
}
ISR(_VECTOR(9)){
  set_stat_led2(true);
}
ISR(_VECTOR(10)){
  set_stat_led2(true);
}*/
/*ISR(_VECTOR(11)){
  if(!LED_STAT){
    set_stat_led2(true);
    set_stat_led1(false);
    LED_STAT=true;
  }else{
    set_stat_led2(false);
    set_stat_led1(true);
    LED_STAT=false;
  }
}*/
/*ISR(_VECTOR(12)){
  set_stat_led2(true);
}
ISR(_VECTOR(13)){
  set_stat_led2(true);
}
ISR(_VECTOR(14)){
  set_stat_led2(true);
}
ISR(_VECTOR(15)){
  set_stat_led2(true);
}
ISR(_VECTOR(16)){
  set_stat_led2(true);
}
ISR(_VECTOR(17)){
  set_stat_led2(true);
}
ISR(_VECTOR(18)){
  set_stat_led2(true);
}
ISR(_VECTOR(19)){
  set_stat_led2(true);
}*/
ISR(_VECTOR(20)){
  transmit_complete = true;
  set_stat_led2(true);
  //set_stat_led2(true);
}/*
ISR(_VECTOR(21)){
  set_stat_led2(true);
}
ISR(_VECTOR(22)){
  set_stat_led2(true);
}
ISR(_VECTOR(23)){
  set_stat_led2(true);
}
ISR(_VECTOR(24)){
  set_stat_led2(true);
}
ISR(_VECTOR(25)){
  set_stat_led2(true);
}
ISR(_VECTOR(26)){
  set_stat_led2(true);
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
    if(timer_interrupt)
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
