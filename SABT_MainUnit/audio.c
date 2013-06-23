/**
 * @file audio.c
 * @brief Code for common MP3 functions
 * @author Vivek Nair
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "VS1053.h"
#include "alphabet.h"
#include "Globals.h"

#define MAX_PLAYLIST_SIZE 32

// Playlist supports quening some files at a time with filenames of up to 13
//	characters
bool playlist_empty = true;
static char playlist[MAX_FILENAME_SIZE][MAX_PLAYLIST_SIZE];
static short playlist_size = 0;
static short playlist_index = 0;

/**
 * @brief Tries to queue the requested MP3 file to the playlist
 * @param char* - Pointer to filename
 * @return bool - True if file was added, false if queue is full
 */
bool play_mp3(char* fileset, char* mp3) {

	if (mp3 == NULL) {
		PRINTF("Error: Cannot play NULL MP3");
		NEWLINE;
	}

	//Return false if playlist is full
	if (playlist_size == MAX_PLAYLIST_SIZE) {
		PRINTF("Playlist full");
		NEWLINE;
		return false;
	}
	
	//Otherwise add file to playlist
	playlist_size++;
	if (fileset != NULL)
		sprintf(playlist[playlist_size - 1], "%s%s.mp3", fileset, mp3);
 	else
 		sprintf(playlist[playlist_size - 1], "%s.mp3", mp3);

 	PRINTF("Queuing: ");
 	PRINTF(playlist[playlist_size - 1]);
 	NEWLINE;

	playlist_empty = false;
	return true;
}


/**
 * @brief Plays a specified amount of slience
 * @param int - Length of slience in milliseconds
 * @return void
 */
void play_silence(int milliseconds) {
	switch (milliseconds) {
		case 250:
			play_mp3("SYS_","S025");
			break;
		case 500:
			play_mp3("SYS_","S050");
			break;
		case 750:
			play_mp3("SYS_","S075");
			break;
		case 1000:
			play_mp3("SYS_","S100");
			break;
		default:
			break;
	}
}

/**
 * @brief Plays next queued MP3 file. Only called when queue is not empty.
 *		is called repeatedly till queue is empty
 * @param void
 * @return void
 */
void play_next_mp3() {
	//Only called when the playlist is not empty
	PRINTF("Playing: ");
	PRINTF(playlist[playlist_index]);
	NEWLINE;
	request_to_play_mp3_file(playlist[playlist_index]);
	playlist_index++;

	//If playlist is now empty, reset variables 
	if (playlist_index == playlist_size) {
		playlist_empty = true;
		playlist_size = 0;
		playlist_index = 0;
	}
}

/**
 * @brief Queues dot sound file
 * @param char - Represents dot to play
 * @return void
 */
void play_dot(char* fileset, char dot) {
	char mp3[5];
	sprintf(mp3, "%c", dot);
	play_mp3(fileset, mp3);
}

/**
 * @brief Play sound file corresponding to an alphabet, checks for NULL arg
 * @param alphabet_t* alpha
 * @return void
 */
void play_alphabet(char* fileset, alphabet_t *alpha) {
	char mp3[5];
	if (alpha != NULL)
		sprintf(mp3, "%s", alpha->sound);
	else
		sprintf(mp3, "INVP");
	play_mp3(fileset, mp3);
}

/**
 * @brief Plays the dot sequence for a given bit pattern
 * @param char* - localisation path, char - bit pattern
 * @return void
 */
void play_bit_pattern(char* fileset, unsigned char bit_pattern) {
	char dot[2];
	for (int i = 0; bit_pattern != 0; i++, bit_pattern = bit_pattern >> 1) {
		if (bit_pattern & 0x01) {
			play_dot(fileset, (itoa((i+1), dot, 10))[0]);
		}
	}
}

/**
 * @brief Play dot sequence corresponding to an alphabet, checks for NULL arg
 * @param alphabet_t *alpha
 * @return void
 */
void play_dot_sequence(char* fileset, alphabet_t *alpha) {
	char bit_pattern;
	if (alpha != NULL) {
		bit_pattern = alpha->bit_pattern;
		play_bit_pattern(fileset, bit_pattern);
	} else {
		play_mp3(fileset, "INVP");
	}
}

/**
 * @brief Informs user about erroroneous input
 * @param char* path to localisation, char - bit pattern
 * @return void
 */
void play_input_error(char* fileset, char bit_pattern) {
	play_mp3(fileset, "INVP");
	play_mp3(fileset, "UPRS");
	play_bit_pattern(fileset, bit_pattern);
	play_mp3(fileset, "TAGA");
	play_silence(500);
}
