#include "GlobalsUI.h"

char ProcessedCellValue;
uint8_t PI_LastDotPressed;

void ResetCellState(void)
{
	int i=0;
	for(i=0;i<6;i++)
	{
		DotsPressed[i]=false;
	}
	PI_LastDotPressed=0;
}

/*
This function search for the pressed dot values and update the pressed dots array
In the primary interface the Braille inputs ar configured as digital inputs
*/
void ProcessTheDot(void)
{
	uint8_t TempDot=0;
	uint8_t PI_ThisDot;
	bool NewDotDetected=false;
	if(!(PINC & (1<<UI_BR1))) //Dot1
	{
		NewDotDetected=true;
		TempDot=1;
	}
	if(!(PINC & (1<<UI_BR2))) //Dot1
	{
		NewDotDetected=true;
		TempDot=2;
	}
	if(!(PINC & (1<<UI_BR3))) //Dot1
	{
		NewDotDetected=true;
		TempDot=3;
	}
	if(!(PINC & (1<<UI_BR4))) //Dot1
	{
		NewDotDetected=true;
		TempDot=4;
	}
	if(!(PINC & (1<<UI_BR5))) //Dot1
	{
		NewDotDetected=true;
		TempDot=5;
	}
	if(!(PINC & (1<<UI_BR6))) //Dot1
	{
		NewDotDetected=true;
		TempDot=6;
	}

	if(NewDotDetected)
	{
		if(InterfaceType==1)
		{
			//OK this is the default			
		}else if(InterfaceType==2)
		{
			//OK Flip the pattern
			if(TempDot>3) TempDot-=3;
			else TempDot+=3;
		}
		if(PI_LastDotPressed==TempDot) return;
		PI_LastDotPressed=TempDot;
		DotsPressed[TempDot-1]=true;
		SendMsgPayLoad[0]=0x30+PI_LastDotPressed;
		SendMsgPayLoad[1]=0x31;
		SendMsgPayLoad[2]=0x31;
    	SendPacket('A',&SendMsgPayLoad,3);
		//SendPacket('E',&SendMsgPayLoad,3);
	}
}


void CaptureCellValue(void)
{
	int i=0;
	/*
	This section process the stored dot values and
	generate a 6-bit number to be send over serial interface
	*/
	ProcessedCellValue=0x00;
	for(i=0;i<6;i++)
	{
		if(DotsPressed[i])
		{
			ProcessedCellValue |= _BV(i);
		}
	}
	SendMsgPayLoad[0]=ProcessedCellValue;	//6-bit number representing the pressed dots
	SendMsgPayLoad[1]=1;					//Cell number is always 1 for the primary interface
	SendPacket('B',&SendMsgPayLoad,2);		//Send the two bytes with messege type of 'B'
	ResetCellState();						//Reset the cell state so that new letter can start
}
