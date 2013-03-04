/**
 * @file MD2.h
 * @brief definitions for mode 2
 * @author Nick LaGrow
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _MD2_H_
#define _MD2_H_

//Enter - just needs to be more then top cell value
// This is arbitrary
#define ENTER 140

//bit correspondance for letters.  Right most bit is 1
#define A_BITS 0b00000001
#define B_BITS 0b00000011
#define C_BITS 0b00001001
#define D_BITS 0b00011001
#define E_BITS 0b00010001
#define F_BITS 0b00001011
#define G_BITS 0b00011011
#define H_BITS 0b00010011
#define I_BITS 0b00001010
#define J_BITS 0b00011010
#define K_BITS 0b00000101
#define L_BITS 0b00000111
#define M_BITS 0b00001101
#define N_BITS 0b00011101
#define O_BITS 0b00010101
#define P_BITS 0b00001111
#define Q_BITS 0b00011111
#define R_BITS 0b00010111
#define S_BITS 0b00001110
#define T_BITS 0b00011110
#define U_BITS 0b00100101
#define V_BITS 0b00100111
#define W_BITS 0b00111010
#define X_BITS 0b00101101
#define Y_BITS 0b00111101
#define Z_BITS 0b00110101



// State definitions
#define STATE_INITIAL         0   // Just started, plays welcome message
#define STATE_REQUEST_INPUT_1 1   // Request for input from user - part 1
#define STATE_REQUEST_INPUT_2 2   // Request for input from user - part 2
#define STATE_WAIT_INPUT      3   // Waiting for user input
#define STATE_PROC_INPUT      4   // Processed user input
#define STATE_ERROR_1         5   // first part of the error sequence
#define STATE_ERROR_2         6   // second part of the error sequence
#define SET_LETTER_VALS	      7   // Set the letter vals for random and current

char button_bits;
char letter_set;
char random_count;
char current_count;
char initial_letter;
char current_letter;
char current_random_letter;
char use_random_letter;

char[] letter_bits_arr;
char[] letter_arr;

void MD2_Main(void);
void MD2_Reset(void);
void MD2_CallModeYesAnswer(void);
void MD2_CallModeNoAnswer(void);

void MD2_InputDot(char thisDot);
void MD2_InputCell(char thisCell);

#endif /* _MD2_H_ */
