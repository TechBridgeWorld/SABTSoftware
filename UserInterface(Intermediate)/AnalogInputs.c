#include "GlobalsUI.h"


volatile uint16_t valADC;;
uint8_t iADC;
uint8_t valADCH,valADCL;
bool LED_ONOFF;

void InitializeAnalogPorts()
{

	ADCSRA |= 0xEF;
	ADCSRB=0x00;
	ADMUX = 0x67;
	DIDR0=0x3F;
	int i=0;
	for(i=0;i<6;i++)
	{
		ADC_LastVal[i]=0;
	}
	LED_ONOFF=false;
}

/*
----------------------------------------------------------
THESE ARE NOT BEING USED
*/
void StartMeasurement(void)
{
	ADCSRA =0xCE;
}

void StopMeasurement(void)
{

}

uint16_t MeasureInput(int whichADC)
{
	//ADMUX = 0x40 + whichADC;
	_delay_ms(5);
	uint16_t iVal=0;
	int i=0;
	for(i=0;i<10;i++)
	{
		ADC_COMPLETE=false;
		//StartMeasurement();
		_delay_ms(5);
		valADC=((ADCH<<8) | ADCL);
		//while(!ADC_COMPLETE);
		iVal+=valADC;
		_delay_ms(5);
	}
	iVal=iVal/10;
	return iVal;
}
/*
----------------------------------------------------------
*/

uint8_t analog10(int which) 
{
	int i=0;
	uint8_t tADCH,tADCL;
	uint16_t tADC0=0;
	uint16_t tADC[10];
	uint16_t eADC;
	uint16_t iVal=0;
	//bool ADC_STABLE=false;
	ADMUX = 0x40 + which;
	_delay_ms(2); // need at least 130 us between conversions
	for(i=0;i<10;i++)
	{
		tADCH=ADCH;
		tADCL=ADCL;	
		_delay_ms(1);
		tADC[i]=((tADCH<<8) | tADCL);
		iVal+=tADC[i];
	}
	valADC=iVal/10;
	return ADCH;
}


uint8_t analog_diff(int which) 
{
	int i=0,j=0;
	uint8_t cntStableCheck=0;
	uint8_t tADCH,tADCL;
	uint16_t tADC0=0;
	uint16_t tADC[4];
	int dADC[3];
	uint16_t eADC;
	uint16_t iVal1=0;
	uint16_t iVal2=0;
	bool ADC_STABLE=false;
	ADMUX = 0x40 + which;
	_delay_ms(2); // need at least 130 us between conversions
	while(!ADC_STABLE)
	{
		tADCH=ADCH;
		tADCL=ADCL;	
		_delay_ms(3);
		tADC[j]=((tADCH<<8) | tADCL);
		if(j==0)
			dADC[j]=tADC[j]-tADC[j+3];
		else
			dADC[j]=tADC[j]-tADC[j-1];
		cntStableCheck=0;
		for(int tempj=0;tempj<4;tempj++)
		{
			if(dADC[tempj]<=1 && dADC[tempj]>=-1) cntStableCheck++;
			if(cntStableCheck==4) ADC_STABLE=true;
		}
		if(j++>3) j=0;
	}
	valADC=tADC[4];
	return ADCH;
}

ISR(ADC_vect){
	//valADC=((ADCH<<8) | ADCL);
	//ADC_COMPLETE=true;
}

void DetectTheDot(void)
{	
	for(iADC=0;iADC<6;iADC++)
	{
		//analog10(iADC);
		analog_diff(iADC);
		_delay_ms(20);
		if(valADC!=ADC_LastVal[iADC])//If the value has changed
		{
			/*
			---------------------------------------------------------
			This section is used only for debugging the UI.
			Once a change is detected, the analog value is send directly to the 
			main unit with the message type 'E'. This will be send to the terminal window
			by the main unit in the format [ADC channel,MSB,LSB].			
			*/
			//SendMsgPayLoad[0]=iADC;
			//SendMsgPayLoad[2]=(char)(valADC & 0x00ff);
			//SendMsgPayLoad[1]=(char)((valADC >> 8) & 0x00ff);
			//SendPacket('E',&SendMsgPayLoad,3);
			/*
			---------------------------------------------------------
			*/
			ADC_LastVal[iADC]=valADC;
			if(LED_ONOFF)
			{
				SetStatLED1(false);
				LED_ONOFF=false;
			}else
			{
				SetStatLED1(true);
				LED_ONOFF=true;
			}
		}
	}
}
