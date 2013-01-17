#define F_CPU 8000000UL

#include "GlobalsUI.h"

void InitializeUI(void);

int main(void)
{
	InitializeUI();
	SendMsgPayLoad[0]=0x01;
//	SendPacket('D',SendMsgPayLoad,1);
	while(1) //Main loop
	/*
	This loop will take care of all the signal handling from the 
	*/
 	{
		if(TMR1_INT)
		{
			TimerRoutine();
		}
	}

}

void InitializeUI(void)
{
	//Initialize the Serial comm with the main module
	init_USART_MCU();
	//Initialize the analog ports
	InitializeAnalogPorts();
	//Initialize the digital input/output pins
	Initialize_Digital_IO();
	initTimer();
	sei();
	InterfaceType=1;
}

