/**
 * @file audio.c
 * @brief Code for common MP3 functions
 * @author Vivek Nair
 */

#include "VS1053.h"
#include <stdio.h>
#include "alphabet.h"
#include <stdlib.h>

/**
 * @brief Plays a generic MP3 file at an optionally specified path
 * @param char* path, char* filename
 * @return void
 */
void play_mp3(char* path, char* filename) {
	char file[255];
	if (path) {
		sprintf((char*)file, "%s/%s", path, filename);
	} else {
		sprintf((char*)file, "%s", filename);
	}
	request_to_play_mp3_file(file);
}

/**
 * @brief Play dot sound file
 * @param char dot
 * @return void
 */
void play_dot(char* path, char dot) {
	char file[10];
	sprintf((char*)file, "dot_%c.mp3", dot);
	play_mp3(path, file);
}

/**
 * @brief Play sound file corresponding to an alphabet, checks for NULL arg
 * @param alphabet_t* alpha
 * @return void
 */
void play_alphabet(char* path, alphabet_t *alpha) {
	char file[10];
	if (alpha != NULL)
		sprintf((char*)file, "%s.mp3", alpha->sound);
	else
		sprintf((char*)file, "INVPAT.mp3");
	play_mp3(path, file);
}

/**
 * @brief Plays the dot sequence for a given bit pattern
 * @param char* - localisation path, char - bit pattern
 * @return void
 */
void play_bit_pattern(char* path, unsigned char bit_pattern) {
	char dot[2] = "0";
	for (int i = 0; bit_pattern != 0; i++, bit_pattern >> 1) {
		if (bit_pattern & 0x01) {
			play_dot(path, (itoa(i, dot, 10))[0]);
		}
	}
}

/**
 * @brief Play dot sequence corresponding to an alphabet, checks for NULL arg
 * @param alphabet_t *alpha
 * @return void
 */
void play_dot_sequence(char* path, alphabet_t *alpha) {
	char bit_pattern;
	if (alpha != NULL) {
		bit_pattern = alpha->bit_pattern;
		play_bit_pattern(path, bit_pattern);
	} else {
		 play_mp3(path, "INVPAT.mp3");
	}
}

/**
 * @brief Informs user about erroroneous input
 * @param char* path to localisation, char - bit pattern
 * @return void
 */
void play_input_error(char* path, char bit_pattern) {
	play_mp3(path, "INVPAT.mp3");
	play_mp3(path, "YOU_PRESSED.mp3");
	play_bit_pattern(path, bit_pattern);
}
