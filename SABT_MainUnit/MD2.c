/**
* @file MD2.c
* @brief Code for mode 2 - English Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "learn_letter_mode.h"


// Script and fileset parameters
#include "script_english.h"
#define SCRIPT_ADDRESS &script_english

void md2_reset(void) {
    learn_letter_reset(&script_english, NULL, NULL);
}

void md2_main(void) {
    learn_letter_main(&script_english, NULL, NULL);
}