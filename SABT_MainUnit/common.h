/**
 * @file common.h
 * @brief Declarations for some common functions and constants
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include "datastructures.h"

// Handy common macros

#define ARRAYLEN(x)  (sizeof(x) / sizeof((x)[0]))
#define CHARTOINT(c)     ((c) - '0')

//Used for cyclic traversal of lists
#define PLUS_ONE_MOD(X, BOUND) ((X + 1 > BOUND - 1) ? (0) : (X + 1))	
#define MINUS_ONE_MOD(X, BOUND) ((X - 1 < 0) ? (BOUND - 1) : (X - 1))


int   get_current_mode();
lang_type set_language();
char* get_lang_prefix();
char* get_mode_prefix();
void  log_msg(const char* format, ...);
void  set_mode_globals(script_t* new_script, char* new_lang_fileset, char* new_mode_fileset);
void  reset_globals();
void  reset_stats();
int   timer_rand(void);
char  add_dot(char bits, char dot);
void  quit_mode(void);
long  ten_to_the(int);
int   get_num_of_digits(long);
void  play_requested_dot(char);
void  init_index_arr(int *index_arr, int arr_len);
void  init_char_arr(char* arr, int len);

#endif /* _COMMON_H_ */
