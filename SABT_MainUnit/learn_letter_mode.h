/**
* @file learn_letter_mode.h
* @brief Function declarations for the letter-learning modes
* @author Vivek Nair (viveknair@cmu.edu)
*/ 


#ifndef LEARN_LETTER_MODE_H_
#define LEARN_LETTER_MODE_H_

// Standard libraries
#include <stdbool.h>
#include <stdio.h>

// Custom libraries
#include "audio.h"
#include "common.h"
#include "debug.h"
#include "script_common.h"

void learn_letter_reset (script_t* new_script, char* new_lang_fileset, char* new_mode_fileset);
void learn_letter_main  (script_t* SCRIPT_ADDRESS, char* LANG_FILESET, char* MODE_FILESET);

#endif 