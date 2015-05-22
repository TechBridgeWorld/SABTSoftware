/**
* @file MD7.c
* @brief Code for mode 7 - Hindi Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "learn_letter_mode.h"

// Script and fileset parameters
#include "script_hindi.h"
#define SCRIPT_ADDRESS &script_hindi
#define SCRIPT_LENGTH SCRIPT_HINDI_LENGTH
#define LANG_FILESET "HIN_"
#define MODE_FILESET "MD7_"


void md7_reset(void) {
	learn_letter_reset(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}

void md7_main(void) {
	learn_letter_main(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}