/**
 * @file audio.h
 * @brief Declarations for common MP3 control functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdbool.h>

#include "glyph.h"

extern bool playlist_empty;
extern char* lang_fileset;
extern char* mode_fileset;

bool play_mp3(char* fileset, char* mp3);
void play_next_mp3();
void play_dot(char dot);
void play_cell_pattern(char cell_pattern);
void play_glyph(glyph_t *this_glyph);
void play_glyph_by_pattern(char pattern);
void play_dot_sequence(glyph_t *this_glyph);
void play_silence(int milliseconds);
void play_number(int number);

#endif /* _AUDIO_H_ */
