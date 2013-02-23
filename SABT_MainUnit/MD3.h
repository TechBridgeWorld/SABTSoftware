/**
 * @file MD3.h
 * @brief definitions for mode 3
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD3_H_
#define _MD3_H_

// State definitions
#define STATE_INITIAL        0   // Just started, plays welcome message
#define STATE_REQUEST_INPUT1 1   // Request for input from user - part 1
#define STATE_REQUEST_INPUT2 2   // Request for input from user - part 2
#define STATE_WAIT_INPUT     3   // Waiting for user input
#define STATE_PROC_INPUT     4   // Processed user input

#define PRIME                53

int current_state;               // current state of the mode
char expected_dot;               // The expected response from user 
char last_dot;                   // char representing last big dot pressed
int animals_used_list[11];       // Int array representing the animals that 
								 // have been used already.
int animals_used;                // Keeps track of number of animals used in current rotation
char* animal;                    // Name of current animal
char current_letter;             // Current letter being checked for 

void MD3_Main(void);
void MD3_Reset(void);
void MD3_CallModeYesAnswer(void);
void MD3_CallModeNoAnswer(void);

void MD3_InputDot(char thisDot);
void MD3_InputCell(char thisCell);

#endif /* _MD3_H_ */
