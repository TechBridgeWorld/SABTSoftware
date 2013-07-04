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

#define SCRIPT_COMMON_LENGTH 10

extern script_t* lang_script;
extern script_t script_common;

glyph_t* get_glyph_by_pattern(char pattern);

#endif /* _SCRIPT_COMMON_H_ */
