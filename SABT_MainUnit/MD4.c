/**
 * @file MD4.c
 * @brief Code for mode 4 - One player Hangman
 * @author Kory Stiger (kstiger) Susan Zuo(tzuo)
 */

#include "common.h"
#include "globals.h"
#include "audio.h"
#include "script_common.h"
#include "letter_globals.h"
#include "script_english.h"
#include "dictionary.h"
#include "mp3s.h"


// Used to set global fileset variables
// State variables
static bool md_input_ready = false;
static bool md_input_valid = false;

//Globals
static char input_word[MAX_WORD_LEN];
static char *chosen_word;
static char mistake_pool[26] = "";

static char *dictionary[11] = {"rock", "tree", "sky", "apple", "car", "dog",
    "cat", "water", "floor", "leaf", "road"};
    
static int index_arr[11] = {0,1,2,3,4,5,6,7,8,9,10};
static dictionary_t dict = {
    dictionary,
    11,
    0,
    index_arr
};

/*
 * @brief Try to place global entered_letter into global input_word.
 * @return bool - True on success (finds a match and places letter),
 *                false failure (doesn't find a match)
 */
bool place_letter() {
    for (int i = 0; i < strlen(chosen_word); i++) {
        if (entered_letter == chosen_word[i]) {
            input_word[i] = entered_letter;
            return true;
        }
    }
    log_msg("No letter match.");
    return false;
}

void choose_next_word() {
    int num_words = dict.num_words;
    if (dict.index >= num_words - 1) {
        shuffle(num_words, dict.index_array);
        dict.index = 0;
    }
    chosen_word = dict.words[dict.index_array[dict.index]];
    dict.index++;
}

/**
 *  Placing hints by adding correct letters into the input letter(working as if
 *  already entered the correct letters). Shuffle the index to decide where to put
 *  the hints.
 *  @param num_hint number of hints given
 */
void place_hint(int num_hint){
    int chosen_word_len = strlen(chosen_word);
    while (num_hint == chosen_word_len) { //make sure answer is not given right away
        choose_next_word();
        chosen_word_len = strlen(chosen_word);
    }
    int index_arr[chosen_word_len];
    int hint_i;
    init_index_arr(index_arr, chosen_word_len);
    shuffle(chosen_word_len, index_arr);
    for (int i = 0; i < num_hint; i++){
        hint_i = index_arr[i];
        input_word[hint_i] = chosen_word[hint_i];
    }
}

/**
 * @brief Check if the input dot combination is a valid letter and register the 
 *        valid letter
 * @param button_bits - char, holds bits 0-5 that correspond to which buttons have
 *        have been pressed
 * @return bool - true if a valid letter, false if not
 */
bool is_letter_valid(char button_bits)
{
    char letter_from_bits = get_letter_from_bits(button_bits);
    if ((letter_from_bits >= 'a') && (letter_from_bits <= 'z')) {
        entered_letter = letter_from_bits;
        return true;
    }
    return false;
}

/**
 *  Check if the user made the same mistake before
 *
 *  @param entered_letter used enterd letter
 *
 *  @return whether it's past mistake
 */
bool is_past_mistake(char entered_letter) {
    for (int i = 0; mistake_pool[i] != '\0'; i++){
        if (mistake_pool[i] == entered_letter)
            return true;
    }
    return false;
}

void md4_play_mistake() {
    play_feedback(MP3_YOU_HAVE);
    play_number(max_mistakes - mistakes);
    if (max_mistakes - mistakes == 1)
        play_feedback(MP3_MISTAKE_REMAINING);
    else
        play_feedback(MP3_MISTAKES_REMAINING);
}



void md4_reset(void) {
    log_msg("*** MD4 - one player hangman ***");
    set_mode_globals(&script_english, NULL, NULL);
    reset_globals();
    reset_stats();
    md_input_ready = false;
    md_input_valid = false;
    max_mistakes = 8;

}

void md4_main(void) {   
    switch (next_state) {
        case STATE_INITIAL:
            shuffle(dict.num_words, dict.index_array);
            dict.index = 0;
            play_welcome();
            next_state = STATE_GENERATE_QUESTION;
            break;
            
        case STATE_GENERATE_QUESTION:
            choose_next_word();
            log_msg("Next word: %s", chosen_word);
            mistakes = 0;
            init_char_arr(mistake_pool, max_mistakes);
            init_char_arr(input_word, MAX_WORD_LEN);
            play_direction(MP3_CHOOSE_NUM_OF_HINTS);
            play_mode_audio(MP3_SUBMODE);

            next_state = STATE_CHOOSE_LEVEL;
            break;
            
        case STATE_CHOOSE_LEVEL:
            last_dot = create_dialog(NULL,
                                        DOT_1 | DOT_2 | DOT_3 | ENTER_CANCEL);
            switch (last_dot) {
                    case '1':
                        place_hint(1);
                        next_state = STATE_PROMPT;
                        break;
                    case '2':
                        place_hint(2);
                        next_state = STATE_PROMPT;
                        break;
                    case '3':
                        place_hint(3);
                        next_state = STATE_PROMPT;
                        break;
                    case CANCEL:
                        log_msg("Quitting to main menu.");                    
                        quit_mode();
                        break;
                    case ENTER:
                        next_state = STATE_PROMPT;
                        break;
                    default:
                        break;
            }
            
            break;
        
        case STATE_PROMPT:
            play_feedback(MP3_SPELLING_SO_FAR);
            play_string(input_word, strlen(chosen_word));
            play_direction(MP3_GUESS_A_LETTER);
            next_state = STATE_GET_INPUT;
            break;
            
        case STATE_GET_INPUT:
            if (io_user_abort == true) {
                log_msg("[MD4] User aborted input");
                next_state = STATE_REPROMPT;
                io_init();
                break;
            }
            if (get_character(&md_input_valid, &entered_letter)) {
                if (md_input_valid) {
                    log_msg("[MD4] User answer: %c", entered_letter);
                    play_string(&entered_letter, 1);
                    next_state = STATE_CHECK_ANSWER;
                }
                else
                    log_msg("[MD4] IO error");
            }
            break;
            
        case STATE_CHECK_ANSWER:
            if (place_letter() ||
                (!strncmp(input_word, chosen_word, strlen(chosen_word)))) {
                play_feedback(MP3_YES);
                
                 if (!strncmp(input_word, chosen_word, strlen(chosen_word))) {
                    play_feedback(MP3_YOU_HAVE_GUESSED_THE_WORD);
                    play_string(chosen_word, strlen(chosen_word));
                    play_tada();
                    next_state = STATE_GENERATE_QUESTION;
                 }
                 else next_state = STATE_PROMPT;
            }
            else {
                play_feedback(MP3_NO);
                if (mistakes == max_mistakes) {
                    play_feedback(MP3_7_MISTAKES_YOU_MISSED);
                    play_string(chosen_word, strlen(chosen_word));
                    play_direction(MP3_NEW_WORD);
                    next_state = STATE_GENERATE_QUESTION;
                }
                else {
                    if (is_past_mistake(entered_letter)) 
                        play_feedback(MP3_YOU_HAVE_MADE_THE_SAME_MISTAKE);
                    else {
                        mistake_pool[mistakes] = entered_letter;
                        mistakes++;
                    }
                    md4_play_mistake();
                    next_state = STATE_PROMPT;
                }
            } 
            break;
            
        case STATE_REPROMPT:
            last_dot = create_dialog(MP3_WORD_COMMANDS,
                                        ENTER_CANCEL | LEFT_RIGHT);
            switch (last_dot) {
                case RIGHT:     // play answer
                    play_feedback(MP3_THE_ANSWER_IS);
                    play_string(chosen_word, strlen(chosen_word));
                    next_state = STATE_GENERATE_QUESTION;
                    io_init();
                    break;
                    
                case ENTER:     // skip question
                    next_state = STATE_GENERATE_QUESTION;
                    break;
                    
                    // Try again
                case LEFT:
                case CANCEL:    // try again
                    next_state = STATE_PROMPT;
                    io_init();
                    break;
                
                case NO_DOTS:
                default:
                    break;
            }
            break;
            
        default:
            log_msg("[MD9] Error: next_state: %d", next_state);
            quit_mode();
            break;
            
            
    }
}