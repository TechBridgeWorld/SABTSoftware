/*
 * @file sound_game_mode.h
 *
 * @brief Function declarations for "sound game modes" (MD 3,11)
 * @author: Edward Cai
 */ 

#ifndef SOUND_GAME_MODE_H_
#define SOUND_GAME_MODE_H_

#define MAX_INDEX 11

void sound_game_main(script_t* new_script, char* new_lang_fileset, char* new_mode_fileset);
void sound_game_reset(script_t* new_script, char* new_lang_fileset, char* new_mode_fileset, char const *SOUND_LIST[], char const *SOUND_SOURCE_LIST[]);

#endif 

