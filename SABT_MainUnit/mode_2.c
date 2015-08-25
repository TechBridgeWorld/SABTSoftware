/**
* @file mode_2.c
* @brief Code for mode 2 - English Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "mode_letter_game.h"


// Script and fileset parameters
#include "script_english.h"

void mode_2_reset(void) {
    mode_letter_game_reset();
}

void mode_2_main(void) {
    mode_letter_game_main();
}