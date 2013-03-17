/**
 * @file Timer.h
 * @brief definitions for the timer
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER_INTERRUPT_RATE  390 

volatile bool timer_interrupt;  // Indicate interrupt should be processed
volatile bool led_toggle;       // Toggle LEDs on and off

void init_timer(void);
void timer_routine(void);

#endif /* _TIMER_H_ */
