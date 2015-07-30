/**
* @file MD12.c
* @brief Code for mode 12 - Kannada Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "learn_letter_mode.h"
#include "mp3s.h"

#include "script_kannada.h"

void md12_reset(void) {
    learn_letter_reset(&script_kannada, NULL, NULL);
}

void md12_main(void) {
    learn_letter_main(&script_kannada, NULL, NULL);
}