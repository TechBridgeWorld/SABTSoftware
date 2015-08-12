/**
 * @file MD6.c
 * @brief  Mode 6 code - Free Play
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "globals.h"
#include "audio.h"
#include "common.h"
#include "script_common.h"
#include "MD6.h"
#include "mp3s.h"

/* Change these for new script */
#include "script_english.h"

static script_t* this_script = &script_english;
static glyph_t *this_glyph = NULL;

void md6_reset(void) {
    set_mode_globals(this_script, NULL, NULL);
    reset_globals();
    play_welcome();
    current_state = GET_INPUT;
    log_msg("[MD6] Mode reset");
}

void md6_main(void) {
    switch (current_state) {

        case GET_INPUT:
            cell = get_cell();
            if (cell == NO_DOTS)
                break;

            cell_pattern = GET_CELL_PATTERN(cell);
            cell_control = GET_CELL_CONTROL(cell);

            switch (cell_control) {
                case WITH_ENTER:    // checks validity of letter when enter is pressed
                    this_glyph = search_script(this_script, cell_pattern);
                    current_state = CHECK_ANSWER;
                    break;

                case WITH_LEFT:
                case WITH_RIGHT:
                case WITH_CANCEL:
                    break;
            }
            break;


        case CHECK_ANSWER:
            if (this_glyph == NULL)
                play_feedback(MP3_INVALID_PATTERN);
            else
                play_glyph(this_glyph);
            current_state = GET_INPUT;
            break;

        default:
            log_msg("Invalid state_t %d", current_state);
            quit_mode();
            break;
    }
}