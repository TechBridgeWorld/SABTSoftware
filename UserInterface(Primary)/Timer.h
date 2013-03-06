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

volatile bool timer_interrupt;
void init_timer(void);

volatile bool led_toggle;     // Toggle LEDs on and off

void timer_routine(void);

#endif /* _TIMER_H_ */
