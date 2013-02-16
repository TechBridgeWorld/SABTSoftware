/**
 * @file AnalogInputs.h
 * @brief Analog input definitions
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _ANALOG_INPUTS_H_
#define _ANALOG_INPUTS_H_

volatile bool ADC_COMPLETE;
volatile uint16_t ADC_LastVal[6];

void InitializeAnalogPorts();

uint16_t MeasureInput(int whichADC);

void DetectTheDot(void);

#endif /* _ANALOG_INPUTS_H_ */
