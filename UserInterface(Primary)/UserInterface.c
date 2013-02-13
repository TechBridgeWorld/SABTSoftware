/**
 * @file UserInterface.c
 * @brief contains main loop for primary interface board
 * @author Nick LaGrow (nlagrow)
 */

#define F_CPU 8000000UL

#include "GlobalsUI.h"
#include <avr/wdt.h>

void InitializeUI(void);

void delay10()
{
	int i = 0;
	int j=0;
	for(i=0;i<100;i++){
		for(j=0;j<100;j++){
			DDRD &= ~_BV(UI_VOLDOWN);
//			wdt_reset();	
		}
	}
	//PORTD &= ~_BV(UI_STAT1);
	return;
}

/**
 * @brief the main execution loop for the primary ui board
 * Executes an infinite loop, should never return
 * @return 0
 */
int main(void)
{
	InitializeUI();

	//SendMsgPayLoad[0]=0x01;
    //SendPacket('D',(char *)SendMsgPayLoad,5);
	//USART_transmitByteToMCU('Q');
	//while(1){}
	//wdt_disable();

	/*DDRC &= ~_BV(UI_BR1);	
	DDRC &= ~_BV(UI_BR2);
	DDRC &= ~_BV(UI_BR3);
	DDRC &= ~_BV(UI_BR4);
	DDRC &= ~_BV(UI_BR5);
	DDRC &= ~_BV(UI_BR6);

	DDRB &= ~_BV(UI_ENTER1);
	DDRB &= ~_BV(UI_ENTER2);
	DDRB &= ~_BV(UI_MODE1);
	DDRB &= ~_BV(UI_MODE2);

	DDRD &= ~_BV(UI_VOLUP);
	DDRD &= ~_BV(UI_VOLDOWN);
	DDRD |= _BV(UI_STAT1)|_BV(UI_STAT2);
	

	PORTD |= _BV(UI_STAT1);
	//WDTCSR = 0;
//	int i = 0;
//	int j=0;

	for(i=0;i<1000;i++){
		for(j=0;j<1000;j++){
			DDRD &= ~_BV(UI_VOLDOWN);
//			wdt_reset();	
		}
	}
	PORTD &= ~_BV(UI_STAT1);
	for(i=0;i<1000;i++){
		for(j=0;j<1000;j++){
			DDRD &= ~_BV(UI_VOLDOWN);
//			wdt_reset();	
		}
	}*/

	//PORTD |= _BV(UI_STAT1);
	//delay10();
	delay10();
	//SetStatLED2(false);
	SetStatLED1(false);
	//USART_transmitStringToMCU((unsigned char*)"SMALL WAVES IRONICALLY\r\n");

	//while(1){}


	//SetStatLED2(false);
	//TIFR1 |= (1 << 1);
	while(1) //Main loop
	/*
	This loop will take care of all the signal handling from the 
	*/
 	{
		//SetStatLED2(false);
	    //SetStatLED1(false);

	//USART_transmitByteToMCU('Q');

	  //SendPacket('D',SendMsgPayLoad,5);
    // Test the transmission to MCU 
    //	USART_transmitByteToMCU('P');
    
    // Check to see if timer interrupt has occurred
		if(TMR1_INT)
		{
			//SetStatLED2(false);
			TimerRoutine();
		}
	}
	return 0;
}

/**
 * @brief initialize basic elements of the UI board
 */
void InitializeUI(void)
{
	//Initialize the Serial comm with the main module
	init_USART_MCU();

	//Initialize the analog ports
  //TODO: why is this commented out?
	//InitializeAnalogPorts();

	//Initialize the digital input/output pins
	Initialize_Digital_IO();

	// Initialize the timer
	initTimer();

	// Enable interrupts
	sei();

	InterfaceType=1;
}
