/**
 * @file audio.h
 * @brief Definitions for common MP3 control functions
 * @author - Vivek Nair (viveknai)
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "alphabet.h"

void play_mp3(char* path, char* filename);
void play_dot(char* path, char dot);
void play_bit_pattern(char* path, char bit_pattern);
void play_alphabet(char* path, alphabet_t *alpha);
void play_dot_sequence(char* path, alphabet_t *alpha);
void play_input_error(char* path, char bit_pattern); 

#endif /* _AUDIO_H_ */
