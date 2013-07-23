#ifndef _LETTER_H_
#define _LETTER_H_
#include <stdbool.h>

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

char letter_bits_arr[26];
char letter_arr[26];

char entered_letter; // Current letter being entered

bool valid_letter(char);
char get_letter_from_bits(char);
char get_bits_from_letter(char);

#endif
