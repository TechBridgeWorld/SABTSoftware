#include "script_english.h"

glyph_t english_glyphs[SCRIPT_ENGLISH_LENGTH] = {
    {ENG_A, "A", NULL, NULL},
    {ENG_B, "B", NULL, NULL},
    {ENG_C, "C", NULL, NULL},
    {ENG_D, "D", NULL, NULL},
    {ENG_E, "E", NULL, NULL},
    {ENG_F, "F", NULL, NULL},
    {ENG_G, "G", NULL, NULL},
    {ENG_H, "H", NULL, NULL},
    {ENG_I, "I", NULL, NULL},
    {ENG_J, "J", NULL, NULL},
    {ENG_K, "K", NULL, NULL},
    {ENG_L, "L", NULL, NULL},
    {ENG_M, "M", NULL, NULL},
    {ENG_N, "N", NULL, NULL},
    {ENG_O, "O", NULL, NULL},
    {ENG_P, "P", NULL, NULL},
    {ENG_Q, "Q", NULL, NULL},
    {ENG_R, "R", NULL, NULL},
    {ENG_S, "S", NULL, NULL},
    {ENG_T, "T", NULL, NULL},
    {ENG_U, "U", NULL, NULL},
    {ENG_V, "V", NULL, NULL},
    {ENG_W, "W", NULL, NULL},
    {ENG_X, "X", NULL, NULL},
    {ENG_Y, "Y", NULL, NULL},
    {ENG_Z, "Z", NULL, NULL}
};

int english_letters[SCRIPT_ENGLISH_LENGTH] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25
};


script_t script_english = {
    SCRIPT_ENGLISH_LENGTH,
    SCRIPT_ENGLISH_LENGTH,
    0,
    SCRIPT_ENGLISH_FILESET,
    english_glyphs,
    english_letters
};

// NEW STUFF

cell_t a_cell = DOT_1;
cell_t b_cell = DOT_1 | DOT_2;
cell_t c_cell = DOT_1 | DOT_4;
cell_t d_cell = DOT_1 | DOT_4 | DOT_5;
cell_t e_cell = DOT_1 | DOT_5;
cell_t f_cell = DOT_1 | DOT_2 | DOT_4;
cell_t g_cell = DOT_1 | DOT_2 | DOT_4 | DOT_5;
cell_t h_cell = DOT_1 | DOT_2 | DOT_5;
cell_t i_cell = DOT_2 | DOT_4;
cell_t j_cell = DOT_2 | DOT_4 | DOT_5;
cell_t k_cell = DOT_1 | DOT_3;
cell_t l_cell = DOT_1 | DOT_2 | DOT_3;
cell_t m_cell = DOT_1 | DOT_3 | DOT_4;
cell_t n_cell = DOT_1 | DOT_3 | DOT_4 | DOT_5;
cell_t o_cell = DOT_1 | DOT_3 | DOT_5;
cell_t p_cell = DOT_1 | DOT_2 | DOT_3 | DOT_4;
cell_t q_cell = DOT_1 | DOT_2 | DOT_3 | DOT_4 | DOT_5;
cell_t r_cell = DOT_1 | DOT_2 | DOT_3 | DOT_5;
cell_t s_cell = DOT_2 | DOT_3 | DOT_4;
cell_t t_cell = DOT_2 | DOT_3 | DOT_4 | DOT_5;
cell_t u_cell = DOT_1 | DOT_3 | DOT_6;
cell_t v_cell = DOT_1 | DOT_2 | DOT_3 | DOT_6;
cell_t w_cell = DOT_2 | DOT_4 | DOT_5 | DOT_6;
cell_t x_cell = DOT_1 | DOT_3 | DOT_4 | DOT_6;
cell_t y_cell = DOT_1 | DOT_3 | DOT_4 | DOT_5 | DOT_6;
cell_t z_cell = DOT_1 | DOT_3 | DOT_5 | DOT_6;
cell_t cap_cell = DOT_6;

#define ENGL_A {&a_cell, 1, "a", ENGLISH}
#define ENGL_B {&b_cell, 1, "b", ENGLISH}
#define ENGL_C {&c_cell, 1, "c", ENGLISH}
#define ENGL_D {&d_cell, 1, "d", ENGLISH}
#define ENGL_E {&e_cell, 1, "e", ENGLISH}
#define ENGL_F {&f_cell, 1, "f", ENGLISH}
#define ENGL_G {&g_cell, 1, "g", ENGLISH}
#define ENGL_H {&h_cell, 1, "h", ENGLISH}
#define ENGL_I {&i_cell, 1, "i", ENGLISH}
#define ENGL_J {&j_cell, 1, "j", ENGLISH}
#define ENGL_K {&k_cell, 1, "k", ENGLISH}
#define ENGL_L {&l_cell, 1, "l", ENGLISH}
#define ENGL_M {&m_cell, 1, "m", ENGLISH}
#define ENGL_N {&n_cell, 1, "n", ENGLISH}
#define ENGL_O {&o_cell, 1, "o", ENGLISH}
#define ENGL_P {&p_cell, 1, "p", ENGLISH}
#define ENGL_Q {&q_cell, 1, "q", ENGLISH}
#define ENGL_R {&r_cell, 1, "r", ENGLISH}
#define ENGL_S {&s_cell, 1, "s", ENGLISH}
#define ENGL_T {&t_cell, 1, "t", ENGLISH}
#define ENGL_U {&u_cell, 1, "u", ENGLISH}
#define ENGL_V {&v_cell, 1, "v", ENGLISH}
#define ENGL_W {&w_cell, 1, "w", ENGLISH}
#define ENGL_X {&x_cell, 1, "x", ENGLISH}
#define ENGL_Y {&y_cell, 1, "y", ENGLISH}
#define ENGL_Z {&z_cell, 1, "z", ENGLISH}
#define ENGL_CAP {&cap_cell, 1, "capital", ENGLISH}

letter_t eng_cap = ENGL_CAP;

letter_t all_english_letters[27] = { ENGL_A, ENGL_B,
    ENGL_C, ENGL_D, ENGL_E, ENGL_F, ENGL_G, ENGL_H,
    ENGL_I, ENGL_J, ENGL_K, ENGL_L, ENGL_M, ENGL_N,
    ENGL_O, ENGL_P, ENGL_Q, ENGL_R, ENGL_S, ENGL_T,
    ENGL_U, ENGL_V, ENGL_W, ENGL_X, ENGL_Y, ENGL_Z,
    ENGL_CAP };

alphabet_t english_alphabet = {all_english_letters, 26, ENGLISH};
alphabet_t english_plus_cap = {all_english_letters, 27, ENGLISH};