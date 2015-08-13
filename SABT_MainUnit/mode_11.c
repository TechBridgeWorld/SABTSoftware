/**
 * @file mode_11.c
 * @brief Mode 11 code - Everyday Noises Game
 * @author Poornima Kaniarasu (pkaniara)
 */

#include "globals.h"
#include "Modes.h"
#include "audio.h"
#include "datastructures.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h"
#include "sound_game_mode.h"
 #include "mp3s.h"

#define SCRIPT_ADDRESS &script_english


const char *noise_list[MAX_INDEX + 1] = {"aeroplane", "rain", "bell", "doorbell", "horn", "auto",
    "truck", "train", "siren", "phone", "clock", NULL};

const char* noise_sounds[MAX_INDEX + 1] = {"aer", "rai", "bel", "doo", "hor", "aut",
    "tru", "tra", "sir", "pho", "clo", NULL};

void mode_11_reset(void) {
    sound_game_reset(SCRIPT_ADDRESS, NULL, NULL, noise_list, noise_sounds);
}

void mode_11_main(void) {
    sound_game_main(SCRIPT_ADDRESS, NULL, NULL);
}