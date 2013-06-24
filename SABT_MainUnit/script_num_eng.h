/**
 *  @file script_num_eng.h
 *  @brief Bit patterns representing numbers
 *  @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_NUM_ENG_H_
#define _SCRIPT_NUM_ENG_H_

#include "alphabet.h"
#include "script_common.h"

#define SCRIPT_NUM_ENG_FILESET "ENG_"
#define SCRIPT_NUM_ENG_LENGTH 10

// Numbers script bit patterns
#define NUM1 				0b00000001
#define NUM2				0b00000011
#define NUM3				0b00001001
#define NUM4				0b00011001
#define NUM5				0b00010001
#define NUM6				0b00001011
#define NUM7				0b00011011
#define NUM8				0b00010011
#define NUM9				0b00001010
#define NUM0				0b00011010

// Numbers script array
alphabet_t alphabets_num_eng[SCRIPT_NUM_ENG_LENGTH] = {
	{NUM1, "1"},
	{NUM2, "2"},
	{NUM3, "3"},
	{NUM4, "4"},
	{NUM5, "5"},
	{NUM6, "6"},
	{NUM7, "7"},
	{NUM8, "8"},
	{NUM9, "9"},
	{NUM0, "0"},
};

script_t script_num_eng = {
	SCRIPT_NUM_ENG_LENGTH,
	SCRIPT_NUM_ENG_FILESET,
	alphabets_num_eng
}; 

#endif /* _SCRIPT_NUM_ENG_H_ */
