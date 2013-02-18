/**
 * @file MD2.h
 * @brief definitions for mode 2
 * @author Nick LaGrow
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD2_H_
#define _MD2_H_

void MD2_Main(void);
void MD2_Reset(void);
void MD2_CallModeYesAnswer(void);
void MD2_CallModeNoAnswer(void);

void MD2_InputDot(char thisDot);
void MD2_InputCell(char thisCell);

#endif /* _MD2_H_ */
