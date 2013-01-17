#ifndef _ANALOGINPUTS_H_
#define _ANALOGINPUTS_H_

volatile bool ADC_COMPLETE;
volatile uint16_t ADC_LastVal[6];

void InitializeAnalogPorts();

uint16_t MeasureInput(int whichADC);

void DetectTheDot(void);

#endif
