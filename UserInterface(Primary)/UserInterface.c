/**
 * @file UserInterface.c
 * @brief contains main loop for primary interface board
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#define F_CPU 8000000UL

#include "GlobalsUI.h"

void initialize_ui(void);

/**
 * @brief debugging function to add delay
 * @return Void
 */
void delay(void)
{
  int i = 0;
  int j = 0;

  for(i = 0; i < 100; i++){
    for(j = 0; j < 100; j++){
    }
  }
  return;
}

/**
 * @brief interrupt handler 
 * @TODO - NEED TO FIGURE OUT WHAT INTERRUPT
 * @return Void
 */
ISR(_VECTOR(20)){
  transmit_complete = true;
  set_stat_led2(true);
}

/**
 * @brief the main execution loop for the primary ui board
 * Executes an infinite loop, should never return
 * @return Void
 */
int main(void)
{
  // Set up interfaces 
  initialize_ui();

  // Infinite processing loop
  while(1)
  {
    // Check to see if timer interrupt has occurred
    if(timer_interrupt) timer_routine();
  }

  return 0;
}

/**
 * @brief initialize basic elements of the UI board
 * @return Void
 */
void initialize_ui(void)
{
  // Initialize the serial communication with the main module
  init_usart_mcu();

  // Initialize the digital input/output pins
  initialize_digital_io();

  // Initialize the timer
  init_timer();

  // Enable interrupts
  sei();

  // Start in writing mode
  interface_type = INTERFACE_WRITING;
}
