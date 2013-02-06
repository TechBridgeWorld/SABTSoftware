/**
 * @file MD3.h
 * @brief definitions for mode 3
 * @author Nick LaGrow (nlagrow)
 */

#ifndef _MD3_H_
#define _MD3_H_

void MD3_Main(void);
void MD3_Reset(void);
void MD3_CallModeYesAnswer(void);
void MD3_CallModeNoAnswer(void);

void MD3_InputDot(char thisDot);
void MD3_InputCell(char thisCell);

#endif /* _MD3_H_ */
