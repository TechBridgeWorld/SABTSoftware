/**
* @file MD2.c
* @brief Code for mode 2 - English Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>


// Script and fileset parameters
#include "script_english.h"
#define SCRIPT_ADDRESS &script_english
#define SCRIPT_LENGTH SCRIPT_ENGLISH_LENGTH
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD2_"

void md2_reset(void) {
	learn_letter_reset(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}

void md2_main(void) {
	learn_letter_main(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}