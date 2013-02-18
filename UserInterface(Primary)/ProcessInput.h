/**
 * @file ProcessInput.h
 * @brief Handle dot presses
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _PROCESS_INPUT_H_
#define _PROCESS_INPUT_H_

volatile bool DotsPressed[6];
volatile char InterfaceType; // 1=Reading, 2=Writing

void ResetCellState(void);
void ProcessTheDot(void);
void CaptureCellValue(void);

#endif /* _PROCESS_INPUT_H_ */
