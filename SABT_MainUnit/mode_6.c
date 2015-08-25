/**
 * @file mode_6.c
 * @brief  Mode 6 code - Free Play
 * @author Vivek Nair (viveknair@cmu.edu)
 */

#include "globals.h"
#include "audio.h"
#include "common.h"
#include "script_common.h"
#include "mode_6.h"
#include "mp3s.h"
#include "io.h"

/* Change these for new script */
#include "script_english.h"

static glyph_t *this_glyph = NULL;

void mode_6_reset(void) {
}

void mode_6_main(void) {
    switch (current_state) {
        case INITIAL:
            play_welcome();
            current_state = GET_INPUT;
            break;

        case GET_INPUT:
            cell = get_cell();
            if (cell == NO_DOTS)
                break;

            cell_pattern = GET_CELL_PATTERN(cell);
            cell_control = GET_CELL_CONTROL(cell);

            switch (cell_control) {
                case WITH_ENTER:    // checks validity of letter when enter is pressed
                    log_msg("Script address = %p", this_script);
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