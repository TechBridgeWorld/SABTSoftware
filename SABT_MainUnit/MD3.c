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
#include "datastructures.h"
#include "common.h"
#include "letter_globals.h"
#include "script_common.h"
#include "script_english.h" 
#include "MD3.h"
#include "sound_game_mode.h"
 #include "mp3s.h"

const char *animal_list[MAX_INDEX + 1] = {"bee", "camel", "cat", "cow", "dog", "horse",
    "hyena", "pig", "rooster", "sheep", "zebra", NULL};
	
const char* animal_sounds[MAX_INDEX + 1] = {"BEE","CAM","CAT","COW","DOG","HOR",
    "HYE","PIG","ROO","SHE","ZEB", NULL};

void md3_reset(void) {
	sound_game_reset(&script_english, NULL, NULL, animal_list, animal_sounds);
}

void md3_main(void) {
	sound_game_main(&script_english, NULL, NULL);
}