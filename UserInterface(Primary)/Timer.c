/**
 * @file Timer.c
 * @brief handles timer interrupts for the user interface
 * @author Nick LaGrow (nlagrow)
 */

#include "GlobalsUI.h"

volatile bool LED_STAT;

int TmrCntADC;

/**
 * @brief Initialize the timer
 * @return Void
 */
void initTimer(void)
{
	TMR1_INT=false;
	TCCR1A=0x00;
 	TCCR1B=0x0D;
  	OCR1A=780; //1s interval
 	TIMSK1 |= (1<<OCIE1A); //Enable interrupt
  
	TmrCntADC=0;
}

/**
 * @brief interrupt handler for TIMER1_COMPA_vect
 */
ISR(TIMER1_COMPA_vect){
	TMR1_INT=true;
	SetStatLED2(false);
};

void TimerRoutine(void)
{
	TMR1_INT=false;
	/*//USART_transmitStringToMCU("hello small waves");
	//
	if(!LED_STAT){
		SetStatLED2(true);
		SetStatLED1(false);
		LED_STAT=true;
	}else{
		SetStatLED2(false);
		SetStatLED1(true);
		LED_STAT=false;
	}
	//
	CheckCommandButtons();
	RunCommandTasks();
	if(TmrCntADC<2)
	{
		TmrCntADC++;
	}else
	{
		DetectTheDot();
		ProcessTheDot();
		TmrCntADC=0;
	}*/
	SetStatLED2(false);
}
