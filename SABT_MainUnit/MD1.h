/**
 * @file MD1.h
 * @brief definitions for mode 1
 * @author Nick LaGrow (nlagrow)
 */

#ifndef _MD1_H_
#define _MD1_H_

// State definitions
#define STATE_INITIAL        0   // Just started, plays welcome message
#define STATE_REQUEST_INPUT1 1   // Request for input from user - part 1
#define STATE_REQUEST_INPUT2 2   // Request for input from user - part 2
#define STATE_WAIT_INPUT     3   // Waiting for user input
#define STATE_PROC_INPUT     4   // Processed user input

#define PRIME			     53

char expected_dot;               // The expected response from user 

void MD1_Main(void);
void MD1_Reset(void);
void MD1_CallModeYesAnswer(void);
void MD1_CallModeNoAnswer(void);

void MD1_InputDot(char thisDot);
void MD1_InputCell(char thisCell );

#endif /* _MD1_H_ */
