#include "script_kannada.h"

// Kannada script array
glyph_t glyphs_kannada[SCRIPT_KANNADA_LENGTH] = {
    {KAN_A, "A", NULL, NULL},       // 0
    {KAN_AA, "AA", NULL, NULL},     // 1
    {KAN_I, "I", NULL, NULL},       // 2
    {KAN_II, "II", NULL, NULL},     // 3
    {KAN_U, "U", NULL, NULL},       // 4
    {KAN_UU, "UU", NULL, NULL},     // 5
    {KAN_RU_1, "RU", NULL, &glyphs_kannada[7]},   // 6
    {KAN_RU_2, "RU_2", &glyphs_kannada[6] ,NULL}, // 7

    {KAN_E, "E", NULL, NULL},       // 8
    {KAN_EE, "EE", NULL, NULL},     // 9
    {KAN_AI, "AI", NULL, NULL},     // 10
    {KAN_O, "O", NULL, NULL},       // 11
    {KAN_OO, "OO", NULL, NULL},     // 12
    {KAN_AU, "AU", NULL, NULL},     // 13
    {KAN_AM, "AM", NULL, NULL},     // 14
    {KAN_AHA, "AHA", NULL, NULL},   // 15

    {KAN_KA, "KA", NULL, NULL},     // 16
    {KAN_KHA, "KHA", NULL, NULL},   // 17
    {KAN_GA, "GA", NULL, NULL},     // 18
    {KAN_GHA, "GHA", NULL, NULL},   // 19
    {KAN_NNA, "NNA", NULL, NULL},   // 20
    
    {KAN_CHA, "CHA", NULL, NULL},   // 21
    {KAN_CHHA, "CHHA", NULL, NULL}, // 22
    {KAN_JA, "JA", NULL, NULL},     // 23
    {KAN_JHA, "JHA", NULL, NULL},   // 24
    {KAN_NYAA, "NYAA", NULL, NULL}, // 25
    
    {KAN_TTA, "TTA", NULL, NULL},   // 26
    {KAN_TTHA, "TTHA", NULL, NULL}, // 27
    {KAN_DDA, "DDA", NULL, NULL},   // 28
    {KAN_DDHA, "DDHA", NULL, NULL}, // 29
    {KAN_GNA, "GNA", NULL, NULL},   // 30

    {KAN_TA, "TA", NULL, NULL},     // 31
    {KAN_THA, "THA", NULL, NULL},   // 32
    {KAN_DA, "DA", NULL, NULL},     // 33
    {KAN_DHA, "DHA", NULL, NULL},   // 34
    {KAN_NA, "NA", NULL, NULL},     // 35

    {KAN_PA, "PA", NULL, NULL},     // 36
    {KAN_PHA, "PHA", NULL, NULL},   // 37
    {KAN_BA, "BA", NULL, NULL},     // 38
    {KAN_BHA, "BHA", NULL, NULL},   // 39
    {KAN_MA, "MA", NULL, NULL},     // 40

    {KAN_YA, "YA", NULL, NULL},     // 41
    {KAN_RA, "RA", NULL, NULL},     // 42
    {KAN_LA, "LA", NULL, NULL},     // 43
    {KAN_VA, "VA", NULL, NULL},     // 44

    {KAN_SHA, "SHA", NULL, NULL},   // 45
    {KAN_SHHA, "SHHA", NULL, NULL}, // 46
    {KAN_SA, "SA", NULL, NULL},     // 47
    {KAN_HA, "HA", NULL, NULL},     // 48

    {KAN_DLA, "DLA", NULL, NULL},   // 49
};

int letters_kannada[SCRIPT_KANNADA_NUM_LETTERS] = {
    0,  1,  2,  3,  4,  5,  6,       8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49 };

script_t script_kannada = {
    SCRIPT_KANNADA_LENGTH,
    SCRIPT_KANNADA_NUM_LETTERS,
    0,
    SCRIPT_KANNADA_FILESET,
    glyphs_kannada,
    letters_kannada
};