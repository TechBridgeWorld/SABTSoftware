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

int english_letters[SCRIPT_ENGLISH_NUM_LETTERS] = {
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

cell_t a = {DOTS1};
cell_t b = {DOTS12};
cell_t c = {DOTS14};
cell_t d = {DOTS145};
cell_t e = {DOTS15};
cell_t f = {DOTS124};
cell_t g = {DOTS1245};
cell_t h = {DOTS125};
cell_t i = {DOTS24};
cell_t j = {DOTS245};
cell_t k = {DOTS13};
cell_t l = {DOTS123};
cell_t m = {DOTS134};
cell_t n = {DOTS1345};
cell_t o = {DOTS135};
cell_t p = {DOTS1234};
cell_t q = {DOTS12345};
cell_t r = {DOTS1235};
cell_t s = {DOTS234};
cell_t t = {DOTS2345};
cell_t u = {DOTS136};
cell_t v = {DOTS1236};
cell_t w = {DOTS2456};
cell_t x = {DOTS1346};
cell_t y = {DOTS13456};
cell_t z = {DOTS1356};

#define ENG_LETTER_A {"a", "ENG", &a, 1, "A"}
#define ENG_LETTER_B {"b", "ENG", &b, 1, "B"}
#define ENG_LETTER_C {"c", "ENG", &c, 1, "C"}
#define ENG_LETTER_D {"d", "ENG", &d, 1, "D"}
#define ENG_LETTER_E {"e", "ENG", &e, 1, "E"}
#define ENG_LETTER_F {"f", "ENG", &f, 1, "F"}
#define ENG_LETTER_G {"g", "ENG", &g, 1, "G"}
#define ENG_LETTER_H {"h", "ENG", &h, 1, "H"}
#define ENG_LETTER_I {"i", "ENG", &i, 1, "I"}
#define ENG_LETTER_J {"j", "ENG", &j, 1, "J"}
#define ENG_LETTER_K {"k", "ENG", &k, 1, "K"}
#define ENG_LETTER_L {"l", "ENG", &l, 1, "L"}
#define ENG_LETTER_M {"m", "ENG", &m, 1, "M"}
#define ENG_LETTER_N {"n", "ENG", &n, 1, "N"}
#define ENG_LETTER_O {"o", "ENG", &o, 1, "O"}
#define ENG_LETTER_P {"p", "ENG", &p, 1, "P"}
#define ENG_LETTER_Q {"q", "ENG", &q, 1, "Q"}
#define ENG_LETTER_R {"r", "ENG", &r, 1, "R"}
#define ENG_LETTER_S {"s", "ENG", &s, 1, "S"}
#define ENG_LETTER_T {"t", "ENG", &t, 1, "T"}
#define ENG_LETTER_U {"u", "ENG", &u, 1, "U"}
#define ENG_LETTER_V {"v", "ENG", &v, 1, "V"}
#define ENG_LETTER_W {"w", "ENG", &w, 1, "W"}
#define ENG_LETTER_X {"x", "ENG", &x, 1, "X"}
#define ENG_LETTER_Y {"y", "ENG", &y, 1, "Y"}
#define ENG_LETTER_Z {"z", "ENG", &z, 1, "Z"}

cell_t cap_a[2] = {{DOTS6}, {DOTS1}}; // just testing 2-cell concept
#define ENG_LETTER_CAPA {"A", "ENG", cap_a, 2, "A"}

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

letter_t all_letters[26] = {ENG_LETTER_A, ENG_LETTER_B,
	ENG_LETTER_C, ENG_LETTER_D, ENG_LETTER_E, ENG_LETTER_F,
	ENG_LETTER_G, ENG_LETTER_H, ENG_LETTER_I, ENG_LETTER_J,
	ENG_LETTER_K, ENG_LETTER_L, ENG_LETTER_M, ENG_LETTER_N,
	ENG_LETTER_O, ENG_LETTER_P, ENG_LETTER_Q, ENG_LETTER_R,
	ENG_LETTER_S, ENG_LETTER_T, ENG_LETTER_U, ENG_LETTER_V,
	ENG_LETTER_W, ENG_LETTER_X, ENG_LETTER_Y, ENG_LETTER_Z };

alphabet_t english_alphabet = {all_letters, 26};