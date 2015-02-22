/**
 * @file audio.c
 * @brief Audio library
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "VS1053.h"
#include "glyph.h"
#include "Globals.h"
#include "common.h"
#include "io.h"
#include "script_common.h"
#include "FAT32.h"

/* Maximum number of MP3s that can be queued at a given time */
#define MAX_PLAYLIST_SIZE 32

/* System files fileset on SD card */
#define SYSTEM_FILESET "SYS_"

/** Number name parsers, macro refers to number it's position away from decimal
	point */
#define PLACE_ONES 0x01
#define PLACE_TENS 0x02
#define PLACE_HUNDREDS 0x03
#define PLACE_THOUSANDS 0x04
#define PLACE_TEN_THOUSANDS 0x05
// ints on this platform are 2 bytes long - largest signed int is 32,768

bool playlist_empty = true;

/** Playlist supports queuing some files at a time with filenames of up to 13
	characters */
static char playlist[MAX_FILENAME_SIZE][MAX_PLAYLIST_SIZE];
static short playlist_size = 0;
static short playlist_index = 0;

/** Set via set_mode_globals() in each mode so that audio library
	does not have to be constantly passed filesets */
char* lang_fileset = NULL;
char* mode_fileset = NULL;

/**
 *	@brief Tries to queue the requested MP3 file to the playlist
 * 	@param char* fileset - (optional) Pointer to fileset
 * 	@param char* mp3 - Pointer to MP3 filename (4 characters)
 * 	@return bool - True if file was added, false if queue is full or error
 */

bool play_mp3(char* fileset, char* mp3) {

	if (mp3 == NULL) {
		PRINTF("[Audio] Error: Cannot play NULL MP3\n\r");
		return false;
	}

	//Return false if playlist is full
	if (playlist_size == MAX_PLAYLIST_SIZE) {
		PRINTF("[Audio] Playlist full\n\r");
		return false;
	}
	
	
	playlist_size++;

	//Truncate to 8 chars if the string is longer than that
	if (strlen(mp3)>8){
		mp3[8] = '\0';
	}

	//Otherwise add file to playlist
	if (fileset != NULL)
		sprintf(playlist[playlist_size - 1], "%s%s.mp3", fileset, mp3);
 	else
 		sprintf(playlist[playlist_size - 1], "%s.mp3", mp3);

	playlist_empty = false;
	return true;
}



/**
 * @brief Plays a specified amount of slience
 * @param int milliseconds - Length of slience in milliseconds
 *   currently can be 250, 500, 750 or 1000
 * @return void
 */
void play_silence(int milliseconds) {
	switch (milliseconds) {
		case 250:
			play_mp3(SYSTEM_FILESET,"S025");
			break;
		case 500:
			play_mp3(SYSTEM_FILESET,"S050");
			break;
		case 750:
			play_mp3(SYSTEM_FILESET,"S075");
			break;
		case 1000:
			play_mp3(SYSTEM_FILESET,"S100");
			break;
		default:
			break;
	}
}

/**
* @brief Clears MP3 playlist
* @param void
* @return void
*/
void clear_playlist(void) {
	playlist_empty = true;
	playlist_size = 0;
	playlist_index = 0;
}

/**
 * @brief Plays next queued MP3 file. Only called when queue is not empty.
 *		is called repeatedly till queue is empty
 * @param void
 * @return void
 */
void play_next_mp3(void) {
	
	//Only called when the playlist is not empty
	if (playlist_empty == true) {
		PRINTF("[Audio] Error: Playlist empty\n\r");
		return;
	}

	PRINTF("[Audio] Playing: ");
	PRINTF(playlist[playlist_index]);
	NEWLINE;
	
	play_mp3_file((unsigned char*)playlist[playlist_index]);

	playlist_index++;

	//If playlist is now empty, reset variables 
	if (playlist_index == playlist_size) {
		clear_playlist();
	}
}

/**
 * @brief Queues dot sound file
 * @param char* fileset - Language fileset for dot numbers
 * @param char dot - Dot to play
 * @return void
 */
void play_dot(char dot) {
	char mp3[5];
	switch (dot) {
		case '1': case '2': case '3': case '4': case '5': case '6': break;
		case ENTER: dot = 'E'; break;
		case CANCEL: dot = 'C'; break;
		case LEFT: case RIGHT: case NO_DOTS: return;
		default:
			sprintf(dbgstr, "[Audio] Invalid dot: %c\n\r", dot);
			PRINTF(dbgstr);
			break;
	}
	sprintf(mp3, "DOT%c", dot);
	play_mp3(lang_fileset, mp3);
}

/**
 * @brief Play sound file corresponding to an glyph, checks for NULL arg
 * @param glyph_t* this_glyph - Pointer to glyph to play
 * @return void
 */
void play_glyph(glyph_t *this_glyph) {
	char mp3[5];
	if (this_glyph != NULL) {
		sprintf(mp3, this_glyph->sound);
		play_mp3(lang_fileset, mp3);
	}
}

/**
 * @brief Plays the dot sequence for a given bit pattern
 * @param char pattern - Bit pattern to play
 * @return void
 */
void play_pattern(unsigned char pattern) {
	if (pattern == 0) {
		play_mp3(lang_fileset, MP3_BLANK);
		return;
	}
	char dot[2];
	for (int i = 0; pattern != 0; i++, pattern = pattern >> 1) {
		if (pattern & 0x01) {
			play_dot((itoa((i+1), dot, 10))[0]);
		}
	}
}

/**
 * @brief Play dot sequence corresponding to an glyph, checks for NULL arg
 * @param glyph_t *this_glyph - glyph to play dot sequence for
 * @return void
 */
void play_dot_sequence(glyph_t *this_glyph) {
	char pattern; 
	if (this_glyph != NULL) {
		pattern = this_glyph->pattern;
		play_pattern(pattern);
		if (this_glyph->next != NULL) {
			// Plays all the next glyphs in the linked list
			sprintf(dbgstr, "[Audio] Playing next pattern: %s\n\r",
				this_glyph->next->sound);
			play_mp3(lang_fileset, MP3_NEXT_CELL);
			play_dot_sequence(this_glyph->next);
			play_silence(250);
		}
	} else {
		play_mp3(lang_fileset, MP3_INVALID_PATTERN);
	}
}

/**	
	@brief Plays number
	@param int number - Number to be played. Must be under between -9,999 and
	9,999
	@return void
*/
void play_number(int number) {
	
	int curr_digit = -1;
	int digits = -1;
	char mp3[5] = "";


	// If number is just 0, play #0 and return
	if (number == 0) {
		play_mp3(lang_fileset, "#0");
		return;
	}

	if (number < 0) {
		// Say "Negative" and take absolute value
		play_mp3(lang_fileset, "#NEG");
		number = 0 - number;
	}

	// Count number of digits
	digits = get_num_of_digits(number);

	while (number != 0) {
		// Extract current digit and adjust number
		curr_digit = number / ten_to_the(digits - 1);

		if (curr_digit != 0) {
			sprintf(mp3, "#%d", curr_digit);
			switch (digits) {
				case PLACE_ONES:
					play_mp3(lang_fileset, mp3);
					break;

				case PLACE_TENS:
					if (curr_digit == 1) {
						// If teen, play teen and return immediately
						sprintf(mp3, "#%d", number);
						play_mp3(lang_fileset, mp3);
						return;
					} else {
						// Is a multiple of ten
						sprintf(mp3, "#%d0", curr_digit);
						play_mp3(lang_fileset, mp3);
					}
					break;

				case PLACE_HUNDREDS:
					play_mp3(lang_fileset, mp3);
					play_mp3(lang_fileset, "#HUN");
					break;

				default:
					PRINTF("[Audio] Error: Number greater than 3 digits\n\r");
					quit_mode();
			}
		}
		number -= curr_digit * ten_to_the(digits - 1);	
		digits--;
	}
}

/**
* @brief Plays a NULL-terminated array of glyphs
* @param glyph_t* - Pointer to line
* @return void
*/
void play_line(glyph_t** line) {
	glyph_t* curr_glyph = NULL;
	for (int i = 0; i < MAX_BUF_SIZE; i++) {
		curr_glyph = line[i];
		if (curr_glyph) {
			play_glyph(curr_glyph);
		} else {
			return;
		}
	}
}
