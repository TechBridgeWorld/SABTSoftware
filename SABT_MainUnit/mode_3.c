/**
 * @file mode_3.c
 * @brief Mode 3 code - Animal Game
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
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
#include "mode_3.h"
#include "sound_game_mode.h"
 #include "mp3s.h"

const char *animal_list[MAX_INDEX + 1] = {"bee", "camel", "cat", "cow", "dog", "horse",
    "hyena", "pig", "rooster", "sheep", "zebra", NULL};
    
const char* animal_sounds[MAX_INDEX + 1] = {"bee","cam","cat","cow","dog","hor",
    "hye","pig","roo","she","zeb", NULL};

void mode_3_reset(void) {
    sound_game_reset(animal_list, animal_sounds);
}

void mode_3_main(void) {
    sound_game_main();
}