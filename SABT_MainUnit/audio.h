/**
 * @file audio.h
 * @brief Declarations for common MP3 control functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdbool.h>

#include "alphabet.h"

extern bool playlist_empty;

bool play_mp3(char* fileset, char* mp3);
void play_next_mp3();
void play_dot(char* fileset, char dot);
void play_bit_pattern(char* fileset, char bit_pattern);
void play_alphabet(char* fileset, alphabet_t *alpha);
void play_dot_sequence(char* fileset, alphabet_t *alpha);
void play_input_error(char* fileset, char bit_pattern);
void play_silence(int milliseconds);

#endif /* _AUDIO_H_ */
