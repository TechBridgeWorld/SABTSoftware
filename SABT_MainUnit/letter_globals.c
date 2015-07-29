/**
 *  @file letter_globals.c
 *  @brief Pound defines used for determining letters and buttons
 *         DEPRECATED
 *  @author Kory Stiger (kstiger)
 *  @author Poornima Kanirasu (pkaniara)
 */

#include "letter_globals.h"

 #define GLYPH_LEN 26

// Contains bit representations of each of the letters
char letter_bits_arr[GLYPH_LEN] = {
    A_BITS, B_BITS, C_BITS, D_BITS, E_BITS, F_BITS, G_BITS, 
    H_BITS, I_BITS, J_BITS, K_BITS, L_BITS, M_BITS, N_BITS, 
    O_BITS, P_BITS, Q_BITS, R_BITS, S_BITS, T_BITS, U_BITS, 
    V_BITS, W_BITS, X_BITS, Y_BITS, Z_BITS
};

char letter_arr[GLYPH_LEN] =  {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
    'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
    'w', 'x', 'y', 'z'
};

/** 
 * @brief Check if the input dot combination is a valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool valid_letter(char button_bits) {
    char letter_from_bits = get_letter_from_bits(button_bits);

    if ((letter_from_bits >= 'a') && (letter_from_bits <= 'z')) {
        entered_letter = letter_from_bits;
        return true;
    }

    return false;
}


/**
 * @brief Changes letter bits into an actual char letter
 * @param bits - char, bits that correspond to the buttons pressed
 * @return char - letter that corresponds to buttons pressed 
 *                on error - not found bits, return -1
 */
char get_letter_from_bits(char bits) {
    for (int i = 0; i < GLYPH_LEN; i++) {
        if (letter_bits_arr[i] == bits)
            return letter_arr[i];
    }
    // Return error on failure (letter not present in array)
    return -1;
}

/**
 * @brief Changes letter into an its Braille bit representation
 * @param bits - char, bits that correspond to letter character
 * @return char - letter that corresponds to Braille pattern
 *                on error - not found bits, return -1
 */
char get_bits_from_letter(char letter) {
    for(int i = 0; i < GLYPH_LEN; i++) {
        if (letter_arr[i] == letter)
            return letter_bits_arr[i];
    }
    // Return error on failure (letter not present in array)
    return -1;
}

