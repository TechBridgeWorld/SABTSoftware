/*
 * @file sound_game_mode.h
 *
 * @brief Function declarations for "sound game modes" (MD 3,11)
 * @author: Edward Cai
 */ 

#ifndef SOUND_GAME_MODE_H_
#define SOUND_GAME_MODE_H_

// State machine constants
#define STATE_NULL     0x00
#define STATE_MENU     0x01
#define STATE_GENQUES  0x02
#define STATE_PROMPT   0x03
#define STATE_INPUT    0x04
#define STATE_CHECK    0x05
#define STATE_REPROMPT 0x06

#define PRIME          	53

// Submode constants
#define SUBMODE_NULL 0x00
#define SUBMODE_PLAY 0x01
#define SUBMODE_LEARN 0x02

#define MAX_INCORRECT_TRIES_1 3
#define MAX_INCORRECT_TRIES_2 6

#define MAX_INDEX 11



void sound_game_main(script_t* new_script, char* new_lang_fileset, char* new_mode_fileset);
void sound_game_reset(script_t* new_script, char* new_lang_fileset, char* new_mode_fileset, char const *SOUND_LIST[], char const *SOUND_SOURCE_LIST[]);

#endif 

