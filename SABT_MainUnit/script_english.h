/**
 *  @file script_english.h
 *  @brief Structures for the English language
 *  @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_ENGLISH_H_
#define _SCRIPT_ENGLISH_H_

#include "datastructures.h"
#include "script_common.h"

#define SCRIPT_ENGLISH_FILESET "ENG_"
#define SCRIPT_ENGLISH_LENGTH 26 // 26 glyphs
#define SCRIPT_ENGLISH_NUM_LETTERS 26 // 26 letters

// English Alphabets - bit pattern

#define ENG_A 0b000001
#define ENG_B 0b000011
#define ENG_C 0b001001
#define ENG_D 0b011001
#define ENG_E 0b010001
#define ENG_F 0b001011
#define ENG_G 0b011011
#define ENG_H 0b010011
#define ENG_I 0b001010
#define ENG_J 0b011010
#define ENG_K 0b000101
#define ENG_L 0b000111
#define ENG_M 0b001101
#define ENG_N 0b011101
#define ENG_O 0b010101
#define ENG_P 0b001111
#define ENG_Q 0b011111
#define ENG_R 0b010111
#define ENG_S 0b001110
#define ENG_T 0b011110
#define ENG_U 0b100101
#define ENG_V 0b100111
#define ENG_W 0b111010
#define ENG_X 0b101101
#define ENG_Y 0b111101
#define ENG_Z 0b110101

extern script_t script_english;

#endif /* _SCRIPT_ENGLISH_H_ */

