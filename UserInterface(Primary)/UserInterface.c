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
 * @bug This indicates a race condition somewhere!
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
 */
ISR(_VECTOR(20)){
  transmit_complete = true;
  set_stat_led2(true);
}

/**
 * @brief The primary user interface's main routine.
 * @return void, but should never return
 * @bug "delay" call between sending enter and the cell.
 * @warning Is it necessary to send the entire cell with
 * the enter, since we have already sent the dots that
 * make up that cell? This work could be done by the
 * main board instead.
 *
 * Initializes the system. Interrupt handlers are set to
 * flip bools if data has been (sent to? or) received from
 * the main board, and a timer interrupt is set to flip
 * a timer bool every 780 cycles (set in timer.h).
 * 
 * In an infinite loop, check whether the timer interrupt
 * has gone off (i.e. 780 cycles have elapsed). If so:
 * 
 * 1. Check each command button in turn to see if it has been
 * pressed. (This info is stored at PINC.) If a button has been
 * pressed, set BUTTON_ON for that command. If "cancel"
 * was long-pressed, set BUTTON_ON for that, too.
 * 
 * 2. Iterate through the command buttons' BUTTON_ON. If it
 * has been set, send that command to the MCU. If cancel and
 * the long-cancel were both detected, send a long cancel.
 * If enter was detected, send "enter," delay, create a bitmap
 * representing the entire cell, and send that cell.
 * 
 * 3. Iterate through the six dots. If that dot has been
 * pressed, add it to the current cell *and* send the dot
 * to the MCU. Row and column are both set to 1 since the primary
 * board only has one cell. A dot is NOT sent if it has already
 * been sent in the last 10 iterations through the loop -- this
 * is presumably for debounce.
 *
 * 4. Increment each dot's iteration counter (for the 10-iteration
 * check in step 3).
 *
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
