/**
 * @file Timer.c
 * @brief handles timer interrupts for the user interface
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "GlobalsUI.h"

/**
 * @brief Initialize the timer
 * @return Void
 * The timer is set up in compare mode - an interrupt fires when the counter
 * value is the same as the value in OCR1A
 */
void init_timer(void)
{
  timer_interrupt = false;
  TCCR1A = 0x00;                // TCCR is Timer Counter Control Register
  TCCR1B = 0x0D;
  OCR1A = TIMER_INTERRUPT_RATE; // Set timer interrupt rate
  TIMSK1 |= (1<<OCIE1A);        // Enable timer interrupt
}

/**
 * @brief interrupt handler for TIMER1_COMPA_vect
 * @return Void
 */
ISR(_VECTOR(11)){
  timer_interrupt = true;
};

/**
 * @brief routine executed on timer interrupt (during main loop)
 * @return Void
 */
void timer_routine(void)
{
  int i = 0;

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
  check_command_buttons();
  run_command_tasks();
  process_the_dot();

  for (i = 0; i < NUM_DOTS; i++)
  {
    if(recently_pressed[i] != 0)
	{
		recently_pressed[i] = (recently_pressed[i] + 1) % TIMER_PRESS_DELAY_RATE;
	}
  }
}
