/**
 * @file md11.c
 * @brief Mode 11 code - Everyday Noises Game
 * @author Poornima Kaniarasu (pkaniara)
 */

#include "Globals.h"
#include "Modes.h"
#include "audio.h"
#include "glyph.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h"
#include "sound_game_mode.h"

#define SCRIPT_ADDRESS &script_english
#define SCRIPT_LENGTH SCRIPT_ENGLISH_LENGTH
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD11"


const char *noise_list[MAX_INDEX + 1] = {"aeroplane", "rain", "bell", "doorbell", "horn", "auto",
    "truck", "train", "siren", "phone", "clock", NULL};

const char* noise_sounds[MAX_INDEX + 1] = {"AER", "RAI", "BEL", "DOO", "HOR", "AUT",
    "TRU", "TRA", "SIR", "PHO", "CLO", NULL};

void md11_reset(void) {
	sound_game_reset(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET, noise_list, noise_sounds);
}

void md11_main(void) {
	sound_game_main(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}