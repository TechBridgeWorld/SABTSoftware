************
* Overview *
************

Wrapper Functions	// Abstracts sound playback for convenience
	|				// 	play_dot(char dot)
	|				//	play_pattern(char pattern)
	|				// 	play_glyph(glyph_t* this_glyph)
	|				//	play_dot_sequence(glyph_t* this_glyph)
	|				//	play_silence(int milliseconds)
	|				//	play_number(int number)
	|				//	play_line(glyph_t** line)
	|				// Check audio.c comments for more detailed info
	|
	v
play_mp3()			// Adds MP3 to 32-length file queue
	|
	v
play_next_mp3()		// Selects next MP3 from queue
	|
	v
play_mp3_file()		// Handles actual MP3 file playback from SD to VS1053