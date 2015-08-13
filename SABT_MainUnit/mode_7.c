/**
* @file mode_7.c
* @brief Code for mode 7 - Hindi Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "learn_letter_mode.h"
#include "mp3s.h"

#include "script_hindi.h"


void mode_7_reset(void) {
    learn_letter_reset(&script_hindi, NULL, NULL);
}

void mode_7_main(void) {
    learn_letter_main(&script_hindi, NULL, NULL);
}