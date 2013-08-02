# Modes

The SABT offers modularity of functionality in the form of modes, that can be toggled on or off without recompiling the code. All modes developed for the SABT get programmed on the flash memory, but only select modes enabled via MODES.DAT on the SD card are made available to the end user.

## Developing Modes

2 files are required of every mode - a header file that makes the core logic and IO handler functions available to the UI event registrars, and a source file that implements those functions. These files are numbered sequentially. Each mode must have a unique mode number. The mode number can be 2 digits long at most.The maximum number of modes on the SABT is #defined as MAX_NUMBER_OF_MODES in globals.h. As of now, the SABT has 11 stable modes:
MD1 - Dot practice
MD2 - Letter practice
MD3 - Animal game
MD4 - Hangman
MD5 - Two-player hangman
Two further modes are currently under development and testing:
MD6 - Free play
MD7 - Hindi Braille practice
MD8 - Number practice
MD9 - Maths practice
MD11 - Everyday noises game
MD12 - Kannada Braille practice

The header file itself must declare the core logic and UI functions for the mode. They all follow the same pattern as follows, where # represents the mode number. Note: these are optional - you can write your mode using just main() and reset() and the new IO library:

void md#_main(void); - Implements the core state logic for the mode. This is the function that main() indirectly calls in the absence of any IO interrupts.
void md#_reset(void); - A function that resets all state variables.
void md#_call_mode_yes_answer(void); - An IO handler that handles ENTER events
void md#_call_mode_no_answer(void); - An IO handler that handles CANCEL events
void md#_input_dot(char this_dot); - An IO handler that handles single dot entry. char this_dot is in the form of characters ‘1’ through ‘6’.
void md#_input_cell(char this_cell); - An IO handler that handles single cell entry.
void md#_call_mode_left(void); - IO handler for LEFT button
void md#_call_mode_right(void); - IO handler for RIGHT button

### NOTE

md#_input_cell functions are unimplemented in all modes as of now.
There are currently no handlers for the scroll buttons. This feature will probably be desired for future modes.
The purpose of the md#_input_mode_no_answer() function is unclear. It seems as if the parent UI handler automatically exits from running modes.

## Registering Modes

### Registering MD#.h

For instance, if files corresponding to MD1 are MD1.c and MD1.h. The header file is registered under Modes.h as a #include “MDx.h” directive.
Registering functions in MD#.c
The functions declared in MD#.h and defined in MD#.c must then be registered in UI_handle.c. This file defines a number of the device level IO interrupt handlers. The following are a list of these handlers:
void ui_play_intro_current_mode(void);
void ui_call_mode_yes_answer(void);
void ui_call_mode_no_answer(void);
void ui_input_dot_to_current_mode(char this_dot);
void ui_input_cell_to_current_mode(char this_cell);
void ui_run_main_of_current_mode(void);
void ui_reset_the_current_mode(void);

All of these functions are in the form of large switch-case statements that call the appropriate mode handler when a button interrupt is received. Functions for new modes can be registered by creating new cases for each of these functions that case on the unique mode number of the mode being developed.

### Enabling Modes

The SD card has a file called MODES.DAT. This file contains a specially formatted string that informs the SABT about which modes to make available to the user. The intention of this feature is to enable some control over the features of the device without needing development level hardware and software tools.

The string is of the form 1,2,3,4;, where each # signifies a mode number to enable and the ; signifies the end of the string. Enabling modes that do not exist in the code will lead to unpredictable behaviour. The mode IDs can be up to 2 digits long. The mode file size should not exceed 128 characters in total (including EOF and other characters).

### NOTE: Currently dynamic mode configuration is disabled because of a bug in reading in MODES.DAT. So the modes have to be statically set when flashing the MCU by changing a string in ui_handle.c's check_modes() function.

### Adding Menu Item MP3

All modes must have a MP3 file that can be played back when the user is scrolling through the menu. The file must be named “MD#.mp3” and placed in the root directory of the SD card.