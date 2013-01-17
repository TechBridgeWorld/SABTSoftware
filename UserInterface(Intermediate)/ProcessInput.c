#include "GlobalsUI.h"

uint8_t TempDot=0;
char ProcessedCellValue;
uint8_t PI_LastDotPressed;
uint8_t PI_LastCell_raw;
uint8_t PI_LastCell_col;
uint16_t DotVals_Buttons[18]={0x0033,0x0048,0x005D,0x00BE,0x00B2,0x00C8,
							  0x0187,0x019C,0x0191,0x0206,0x021B,0x0211,
							  0x02E7,0x02FD,0x02F2,0x0352,0x0373,0x037E};

void ResetCellState(void)
{
	int i=0;
	for(i=0;i<6;i++)
	{
		DotsPressed[i]=false;
	}
	PI_LastDotPressed=0;
}

void CheckButtonInput(void)
{
/*
The analog input of the buttons based Braille cells are captured via two ADC channels (ADC0 and ADC2)
Top two raws: ADC0
Bottom row : ADC2
*/
	bool NewDotDetected=false;
	uint8_t CurrentDot=0;
	uint8_t CurrentCol=0;
	//first check ADC0
	for(int i=0;i<18;i++)
	{
		if(i==2 || i==5 || i==8 || i==11 || i==14 || i==17) //use ADC2
		{
			if((ADC_LastVal[2]>=DotVals_Buttons[i]-2) && (ADC_LastVal[2]<=DotVals_Buttons[i]+2))
			{				
				NewDotDetected=true;
				TempDot=i;
			}
		}else			//use ADC0
		{
			if((ADC_LastVal[0]>=DotVals_Buttons[i]-2) && (ADC_LastVal[0]<=DotVals_Buttons[i]+2))
			{
				NewDotDetected=true;
				TempDot=i;
			}			
		}
		if(NewDotDetected)
		{
			if(TempDot<6)
			{
				//DotsPressed[TempDot]=true;
				CurrentDot=TempDot+1;
				CurrentCol=1;
			}
			else if(TempDot<12)
			{
				//DotsPressed[TempDot-6]=true;
				CurrentDot=TempDot-5;
				CurrentCol=2;
			}
			else
			{
				//DotsPressed[TempDot-12]=true;
				CurrentDot=TempDot-11;
				CurrentCol=3;
			}
			PI_LastCell_raw=1;
			
		}
	}
	/*
	If a new dot is detected send it to the main unit with the message type 'A'
	*/
	if(NewDotDetected)
	{
		if(InterfaceType==1)
		{
			//OK this is the default			
		}else if(InterfaceType==2)
		{
			//OK Flip the pattern
			if(CurrentDot>3) CurrentDot-=3;
			else CurrentDot+=3;
		}
		if(PI_LastDotPressed==CurrentDot && PI_LastCell_col==CurrentCol) return; // This is the same button, ignore it
		PI_LastDotPressed=CurrentDot;
		PI_LastCell_col=CurrentCol;
		DotsPressed[CurrentDot-1]=true;
		SendMsgPayLoad[0]=0x30+PI_LastDotPressed;
		SendMsgPayLoad[1]=0x30+PI_LastCell_raw;
		SendMsgPayLoad[2]=0x30+PI_LastCell_col;
    	SendPacket('A',&SendMsgPayLoad,3);
		SendPacket('E',&SendMsgPayLoad,3);
	}
}


void ProcessTheDot(void)
{
	CheckButtonInput();
}

/*
This function is not used as the letter values are 
determined at the main unit
*/
char GetTheCellLetter(void)
{
	switch(ProcessedCellValue)
	{
		case 1:
			return 'a';
			break;
		case 3:
			return 'b';
			break;
		case 9:
			return 'c';
			break;
		case 25:
			return 'd';
			break;
		case 17:
			return 'e';
			break;
		case 11:
			return 'f';
			break;
		case 27:
			return 'g';
			break;
		case 19:
			return 'h';
			break;
		case 10:
			return 'i';
			break;
		case 26:
			return 'j';
			break;
		case 5:
			return 'k';
			break;
		case 7:
			return 'l';
			break;
		case 13:
			return 'm';
			break;
		case 29:
			return 'n';
			break;
		case 21:
			return 'o';
			break;
		case 15:
			return 'p';
			break;
		case 31:
			return 'q';
			break;
		case 23:
			return 'r';
			break;
		case 14:
			return 's';
			break;
		case 30:
			return 't';
			break;
		case 37:
			return 'u';
			break;
		case 39:
			return 'v';
			break;
		case 58:
			return 'w';
			break;
		case 45:
			return 'x';
			break;
		case 61:
			return 'y';
			break;
		case 53:
			return 'z';
			break;
	}
	return 0;
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
