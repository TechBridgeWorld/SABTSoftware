/**
 * @file MD3.c
 * @brief Mode 3 code - Animal Game
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
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
#include "MD3.h"
#include "sound_game_mode.h"

#define SCRIPT_ADDRESS &script_english
#define SCRIPT_LENGTH SCRIPT_ENGLISH_LENGTH
#define LANG_FILESET "ENG_"
#define MODE_FILESET "MD3_"


const char *animal_list[MAX_INDEX + 1] = {"bee", "camel", "cat", "cow", "dog", "horse",
    "hyena", "pig", "rooster", "sheep", "zebra", NULL};
	
const char* animal_sounds[MAX_INDEX + 1] = {"BEE","CAM","CAT","COW","DOG","HOR",
    "HYE","PIG","ROO","SHE","ZEB", NULL};

void md3_reset(void) {
	sound_game_reset(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET, animal_list, animal_sounds);
}

void md3_main(void) {
	sound_game_main(SCRIPT_ADDRESS, LANG_FILESET, MODE_FILESET);
}