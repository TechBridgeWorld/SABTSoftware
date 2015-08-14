#include "script_english.h"

#define NUM_ENG_LETTERS 26

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

cell_t a = DOT_1;
cell_t b = DOT_1 | DOT_2;
cell_t c = DOT_1 | DOT_4;
cell_t d = DOT_1 | DOT_4 | DOT_5;
cell_t e = DOT_1 | DOT_5;
cell_t f = DOT_1 | DOT_2 | DOT_4;
cell_t g = DOT_1 | DOT_2 | DOT_4 | DOT_5;
cell_t h = DOT_1 | DOT_2 | DOT_5;
cell_t i = DOT_2 | DOT_4;
cell_t j = DOT_2 | DOT_4 | DOT_5;
cell_t k = DOT_1 | DOT_3;
cell_t l = DOT_1 | DOT_2 | DOT_3;
cell_t m = DOT_1 | DOT_3 | DOT_4;
cell_t n = DOT_1 | DOT_3 | DOT_4 | DOT_5;
cell_t o = DOT_1 | DOT_3 | DOT_5;
cell_t p = DOT_1 | DOT_2 | DOT_3 | DOT_4;
cell_t q = DOT_1 | DOT_2 | DOT_3 | DOT_4 | DOT_5;
cell_t r = DOT_1 | DOT_2 | DOT_3 | DOT_5;
cell_t s = DOT_2 | DOT_3 | DOT_4;
cell_t t = DOT_2 | DOT_3 | DOT_4 | DOT_5;
cell_t u = DOT_1 | DOT_3 | DOT_6;
cell_t v = DOT_1 | DOT_2 | DOT_3 | DOT_6;
cell_t w = DOT_2 | DOT_4 | DOT_5 | DOT_6;
cell_t x = DOT_1 | DOT_3 | DOT_4 | DOT_6;
cell_t y = DOT_1 | DOT_3 | DOT_4 | DOT_5 | DOT_6;
cell_t z = DOT_1 | DOT_3 | DOT_5 | DOT_6;
cell_t capital = DOT_6;

#define ENG_LETTER_A {"a", ENGLISH, &a, 1}
#define ENG_LETTER_B {"b", ENGLISH, &b, 1}
#define ENG_LETTER_C {"c", ENGLISH, &c, 1}
#define ENG_LETTER_D {"d", ENGLISH, &d, 1}
#define ENG_LETTER_E {"e", ENGLISH, &e, 1}
#define ENG_LETTER_F {"f", ENGLISH, &f, 1}
#define ENG_LETTER_G {"g", ENGLISH, &g, 1}
#define ENG_LETTER_H {"h", ENGLISH, &h, 1}
#define ENG_LETTER_I {"i", ENGLISH, &i, 1}
#define ENG_LETTER_J {"j", ENGLISH, &j, 1}
#define ENG_LETTER_K {"k", ENGLISH, &k, 1}
#define ENG_LETTER_L {"l", ENGLISH, &l, 1}
#define ENG_LETTER_M {"m", ENGLISH, &m, 1}
#define ENG_LETTER_N {"n", ENGLISH, &n, 1}
#define ENG_LETTER_O {"o", ENGLISH, &o, 1}
#define ENG_LETTER_P {"p", ENGLISH, &p, 1}
#define ENG_LETTER_Q {"q", ENGLISH, &q, 1}
#define ENG_LETTER_R {"r", ENGLISH, &r, 1}
#define ENG_LETTER_S {"s", ENGLISH, &s, 1}
#define ENG_LETTER_T {"t", ENGLISH, &t, 1}
#define ENG_LETTER_U {"u", ENGLISH, &u, 1}
#define ENG_LETTER_V {"v", ENGLISH, &v, 1}
#define ENG_LETTER_W {"w", ENGLISH, &w, 1}
#define ENG_LETTER_X {"x", ENGLISH, &x, 1}
#define ENG_LETTER_Y {"y", ENGLISH, &y, 1}
#define ENG_LETTER_Z {"z", ENGLISH, &z, 1}
#define ENG_CAPITAL  {"cap", ENGLISH, &capital, 1}

letter_t eng_a = ENG_LETTER_A;
letter_t eng_b = ENG_LETTER_B;
letter_t eng_c = ENG_LETTER_C;
letter_t eng_d = ENG_LETTER_D;
letter_t eng_e = ENG_LETTER_E;
letter_t eng_f = ENG_LETTER_F;
letter_t eng_g = ENG_LETTER_G;
letter_t eng_h = ENG_LETTER_H;
letter_t eng_i = ENG_LETTER_I;
letter_t eng_j = ENG_LETTER_J;
letter_t eng_k = ENG_LETTER_K;
letter_t eng_l = ENG_LETTER_L;
letter_t eng_m = ENG_LETTER_M;
letter_t eng_n = ENG_LETTER_N;
letter_t eng_o = ENG_LETTER_O;
letter_t eng_p = ENG_LETTER_P;
letter_t eng_q = ENG_LETTER_Q;
letter_t eng_r = ENG_LETTER_R;
letter_t eng_s = ENG_LETTER_S;
letter_t eng_t = ENG_LETTER_T;
letter_t eng_u = ENG_LETTER_U;
letter_t eng_v = ENG_LETTER_V;
letter_t eng_w = ENG_LETTER_W;
letter_t eng_x = ENG_LETTER_X;
letter_t eng_y = ENG_LETTER_Y;
letter_t eng_z = ENG_LETTER_Z;
letter_t eng_capital = ENG_CAPITAL;

letter_t all_letters[26] = {ENG_LETTER_A, ENG_LETTER_B,
    ENG_LETTER_C, ENG_LETTER_D, ENG_LETTER_E, ENG_LETTER_F,
    ENG_LETTER_G, ENG_LETTER_H, ENG_LETTER_I, ENG_LETTER_J,
    ENG_LETTER_K, ENG_LETTER_L, ENG_LETTER_M, ENG_LETTER_N,
    ENG_LETTER_O, ENG_LETTER_P, ENG_LETTER_Q, ENG_LETTER_R,
    ENG_LETTER_S, ENG_LETTER_T, ENG_LETTER_U, ENG_LETTER_V,
    ENG_LETTER_W, ENG_LETTER_X, ENG_LETTER_Y, ENG_LETTER_Z };

letter_t all_letters_plus_cap[27] = {ENG_LETTER_A, ENG_LETTER_B,
    ENG_LETTER_C, ENG_LETTER_D, ENG_LETTER_E, ENG_LETTER_F,
    ENG_LETTER_G, ENG_LETTER_H, ENG_LETTER_I, ENG_LETTER_J,
    ENG_LETTER_K, ENG_LETTER_L, ENG_LETTER_M, ENG_LETTER_N,
    ENG_LETTER_O, ENG_LETTER_P, ENG_LETTER_Q, ENG_LETTER_R,
    ENG_LETTER_S, ENG_LETTER_T, ENG_LETTER_U, ENG_LETTER_V,
    ENG_LETTER_W, ENG_LETTER_X, ENG_LETTER_Y, ENG_LETTER_Z,
    ENG_CAPITAL };


alphabet_t english_alphabet = {all_letters, 26};
alphabet_t english_plus_cap = {all_letters_plus_cap, 27};