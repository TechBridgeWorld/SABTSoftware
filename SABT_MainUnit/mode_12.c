/**
* @file mode_12.c
* @brief Code for mode 12 - Kannada Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
* @todo Record "Welcome to Kannada letter practice" as m12_welc.mp3.
*/


// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "mode_letter_game.h"
#include "mp3s.h"

#include "script_kannada.h"

void mode_12_reset(void) {
    mode_letter_game_reset();
}

void mode_12_main(void) {
    mode_letter_game_main();
}