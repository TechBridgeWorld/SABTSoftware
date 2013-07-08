/**
 * @file script_common.h
 * @brief Definitions for common script wrapper functions
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _SCRIPT_COMMON_H_
#define _SCRIPT_COMMON_H_ 

#include "glyph.h"
#include <stdbool.h>
#include <stddef.h>

#define SCRIPT_COMMON_LENGTH 12
#define SYS_FILESET "SYS_"

// Common MP3 prompts
#define MP3_CORRECT "CORR"
#define MP3_WRONG "WRNG"
#define MP3_TRY_AGAIN "TAGA"
#define MP3_TADA "TADA"

extern script_t* lang_script;
extern script_t script_common;

glyph_t* get_glyph_by_pattern(char pattern);
bool is_number(glyph_t* curr_glyph);
int get_digit(glyph_t* curr_glyph);
bool is_blank(glyph_t* curr_glyph);

#endif /* _SCRIPT_COMMON_H_ */
