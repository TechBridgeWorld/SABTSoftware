/**
* @file MD12.c
* @brief Code for mode 12 - Kannada Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "learn_letter_mode.h"

// Script and fileset parameters
#include "script_kannada.h"
#define SCRIPT_ADDRESS &script_kannada
#define SCRIPT_LENGTH SCRIPT_KANNADA_LENGTH
#define LANG_FILESET "KAN_"
#define MODE_FILESET "MD12"

void md12_reset(void) {
	learn_letter_reset(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}

void md12_main(void) {
	learn_letter_main(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}