/**
 * @file audio.h
 * @brief Declarations for common MP3 control functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdbool.h>
#include "datastructures.h"
#include "mp3s.h"

extern bool playlist_empty;
extern char* lang_fileset;
extern char* mode_fileset;


bool play_mp3(char* fileset, char* mp3);
void play_next_mp3(void);
void clear_playlist(void);
void play_dot(char dot);
void play_pattern(char pattern);
void play_glyph(glyph_t *this_glyph);
void play_dot_sequence(glyph_t *this_glyph);
void play_silence(int milliseconds);
void play_number(long number);
void play_line(glyph_t** line);
void play_word(word_node_t *this_word);
void play_string(char* word, int word_len);

void play_welcome();
void play_submode_choice();
void play_direction(char* dir);
void play_feedback(char* dir);
void play_tada();
void play_system_audio(char* dir);
void play_mode_audio(char* dir);

#endif /* _AUDIO_H_ */
