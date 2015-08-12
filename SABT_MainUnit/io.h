/**
 * @file io.c
 * @brief Declarations for common IO functions
 * @author Vivek Nair (viveknair@cmu.edu)
 * @author Marjorie Carlson (marjorie@cmu.edu)
 */

#ifndef _IO_H_
#define _IO_H_


#ifndef _DATASTRUCTURES_H_
	typedef struct glyph glyph_t;
#endif

// Basic IO functions
void io_init(void);
char get_dot(void);
char get_cell(void);
bool get_line(void);

// Intermediate IO functions
bool get_number(bool* valid, int* res);
bool get_character(bool* valid, char* character);
bool get_first_glyph(glyph_t** res);
bool parse_letter(void);
bool parse_digit(void);
bool parse_symbol(void);
bool parse_word(void);
bool parse_string(void);

// Advanced IO functions
char create_dialog(char* prompt, char control_mask);
void log_msg(const char* format, ...);
void log_msg_no_newline(const char* format, ...);

#endif /* _IO_H_ */
