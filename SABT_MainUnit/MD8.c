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

#include "script_digits.h"

void md8_reset(void) {
    learn_letter_reset(&script_digits, NULL, NULL);
}

void md8_main(void) {
    learn_letter_main(&script_digits, NULL, NULL);
}