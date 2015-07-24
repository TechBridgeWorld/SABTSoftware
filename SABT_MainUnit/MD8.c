/**
* @file MD8.c
* @brief Code for mode 8 - English Number Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "learn_letter_mode.h"
#include "mp3s.h"

// Script and fileset parameters
#include "script_digits.h"
#define SCRIPT_ADDRESS &script_digits
#define SCRIPT_LENGTH SCRIPT_DIGITS_LENGTH
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD8_"

void md8_reset(void) {
	learn_letter_reset(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}

void md8_main(void) {
	learn_letter_main(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}