/**
 * @file MD2.h
 * @brief definitions for mode 2
 * @author Nick LaGrow
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD2_H_
#define _MD2_H_

// State definitions
#define STATE_INITIAL         0   // Just started, plays welcome message
#define STATE_REQUEST_INPUT_1 1   // Request for input from user - part 1
#define STATE_REQUEST_INPUT_2 2   // Request for input from user - part 2
#define STATE_WAIT_INPUT      3   // Waiting for user input
#define STATE_PROC_INPUT      4   // Processed user input
#define STATE_ERROR_1         5   // first part of the error sequence
#define STATE_ERROR_2         6   // second part of the error sequence

char button_bits;
char letter_set;
char random_count;
char current_count;
char initial_letter;
char current_letter;
char current_random_letter;
char use_random_letter;

void MD2_Main(void);
void MD2_Reset(void);
void MD2_CallModeYesAnswer(void);
void MD2_CallModeNoAnswer(void);

void MD2_InputDot(char thisDot);
void MD2_InputCell(char thisCell);

#endif /* _MD2_H_ */
