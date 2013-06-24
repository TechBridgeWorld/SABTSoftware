/**
 * @file alphabet.h
 * @brief Structures for single alphabets and scripts
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _ALPHABET_H_
#define _ALPHABET_H_

//Stores information about single alphabet; used to build scripts
typedef struct alphabet {
	char bit_pattern;		// 0b00xxxxxx 6-bit pattern Braille representation
	char sound[10];			// BBBB in AAA_BBBB.mp3, fileset is set by the file
} alphabet_t;

//Structure representing a script
typedef struct script {
	short length;						// Number of alphabets
	char fileset[5];				// Fileset on SD card; 4 characters long
	alphabet_t* alphabets;  // Pointer to array of alphabets
} script_t;

#endif /* _ALPHABET_H_ */
