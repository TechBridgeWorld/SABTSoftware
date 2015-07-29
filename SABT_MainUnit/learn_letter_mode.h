/**
* @file MD7.h
* @brief Function declarations for mode 7 - Hindi practice
* @author Vivek Nair (viveknair@cmu.edu)
*/ 


#ifndef LEARN_LETTER_MODE_H_
#define LEARN_LETTER_MODE_H_

// Standard libraries
#include <stdbool.h>
#include <stdio.h>

// Custom libraries
#include "audio.h"
#include "io.h"
#include "common.h"
#include "debug.h"
#include "script_common.h"

// State machine constants
#define STATE_NULL 0x00
#define STATE_MENU 0x01
#define STATE_GENQUES 0x02
#define STATE_PROMPT 0x03
#define STATE_INPUT 0x04
#define STATE_CHECK 0x05
#define STATE_REPROMPT 0x06

// Submode constants
#define SUBMODE_NULL 0x00
#define SUBMODE_LEARN 0x01
#define SUBMODE_PLAY 0x02

#define MAX_INCORRECT_TRIES 3

void learn_letter_reset (script_t* new_script, char* new_lang_fileset, char* new_mode_fileset);
void learn_letter_main  (script_t* SCRIPT_ADDRESS, char* LANG_FILESET, char* MODE_FILESET);

#endif 