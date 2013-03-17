/**
 * @file Timer.c
 * @brief handles timer interrupts for the user interface
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "GlobalsUI.h"
int TmrCntADC;          // TODO what is this

/**
 * @brief Initialize the timer
 * @return Void
 * The timer is set up in compare mode - an interrupt fires when the counter
 * value is the same as the value in OCR1A
 * TODO enumerate the ports being set
 */
void init_timer(void)
{
  timer_interrupt = false;
  TCCR1A = 0x00;                // TCCR is Timer Counter Control Register
  TCCR1B = 0x0D;
  OCR1A = TIMER_INTERRUPT_RATE; // Set timer interrupt rate
  TIMSK1 |= (1<<OCIE1A);        // Enable timer interrupt
  TmrCntADC = 0;                // TODO what is this?
}

/**
 * @brief interrupt handler for TIMER1_COMPA_vect
 * @return Void
 */
ISR(/*TIMER1_COMPA_vect*/ _VECTOR(11)){
  timer_interrupt = true;
};

/**
 * @brief routine executed on timer interrupt (during main loop)
 * @return Void
 */
void timer_routine(void)
{
  timer_interrupt = false;

  // Toggle LEDs to show action
  if(!led_toggle)
  {
    set_stat_led1(false);
    led_toggle=true;
  }
  else
  {
    set_stat_led1(true);
    led_toggle=false;
  }
 
  // Handle button presses
  process_the_dot();
  check_command_buttons();
  run_command_tasks();
  
  /*
  if(TmrCntADC<2)
  {
    TmrCntADC++;
  } 
  else
  {
    //DetectTheDot();
    ProcessTheDot();
    TmrCntADC = 0;
  }*/
}
