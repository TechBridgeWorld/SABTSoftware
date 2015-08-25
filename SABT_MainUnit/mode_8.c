/**
* @file mode_8.c
* @brief Code for mode 8 - English Number Braille Practice
* @author Vivek Nair (viveknair@cmu.edu)
*/

// Standard libraries
#include <stdbool.h>
#include <stdio.h>
#include "mode_letter_game.h"
#include "mp3s.h"

#include "script_digits.h"

void mode_8_reset(void) {
    mode_letter_game_reset();
}

void mode_8_main(void) {
    mode_letter_game_main();
}