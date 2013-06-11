/**
 * @file alphabet.h
 * @brief Structures for single alphabets
 * @author Vivek Nair (viveknai)
 */

#ifndef _ALPHABET_H_
#define _ALPHABET_H_

//Stores information about single alphabet; used to build scripts
typedef struct alphabet {
	char bit_pattern;
	char sound[10];
} alphabet_t;

//Structure representing a script
typedef struct script {
	short length;
	alphabet_t* alphabets;
} script_t;

#endif /* _ALPHABET_H_ */
