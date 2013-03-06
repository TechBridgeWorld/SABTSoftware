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
 * TODO enumerate the ports being set
 */
void initTimer(void)
{
  TMR1_INT = false;
  TCCR1A = 0x00;
  TCCR1B = 0x0D;
  OCR1A = 390;            // 0.5s interval
  TIMSK1 |= (1<<OCIE1A);  // Enable timer interrupt
  TmrCntADC = 0;  // TODO what is this?
}

/**
 * @brief interrupt handler for TIMER1_COMPA_vect
 * @return Void
 */
ISR(/*TIMER1_COMPA_vect*/ _VECTOR(11)){ // should be TIMER1_CAPT_vect
  TMR1_INT = true;
};

/**
 * @brief routine executed on timer interrupt (during main loop)
 * @return Void
 */
void TimerRoutine(void)
{
  TMR1_INT = false;
  // Test string
  // USART_transmitStringToMCU((unsigned char*)"hello small waves");

  if(!LED_STAT)
  {
    //SetStatLED2(true);
    SetStatLED1(false);
    LED_STAT=true;
  }
  else
  {
    //SetStatLED2(false);
    SetStatLED1(true);
    LED_STAT=false;
  }
  ProcessTheDot();
  
  CheckCommandButtons();
  RunCommandTasks();
  /*
  if(TmrCntADC<2)
  {
    TmrCntADC++;
  } else
  {
    //DetectTheDot();
    ProcessTheDot();
    TmrCntADC = 0;
  }*/
}
