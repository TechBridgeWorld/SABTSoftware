/**
 * @file Timer.h
 * @brief definitions for the timer
 * @author Nick LaGrow (nlagrow)
 */

#ifndef _TIMER_H_
#define _TIMER_H_

volatile bool TMR1_INT;
void initTimer(void);

void TimerRoutine(void);

#endif /* _TIMER_H_ */