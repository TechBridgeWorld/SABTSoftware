/**
* @file mode_12.c
* @brief Code for mode 12 - Kannada Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
* @todo Record "Welcome to Kannada letter practice" as m12_welc.mp3.
*/


// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "learn_letter_mode.h"
#include "mp3s.h"

#include "script_kannada.h"

void mode_12_reset(void) {
    learn_letter_reset(&script_kannada, NULL, NULL);
}

void mode_12_main(void) {
    learn_letter_main(&script_kannada, NULL, NULL);
}