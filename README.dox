# Stand Alone Braille Tutor

## Overview
The Stand Alone Braille Tutor (hereafter SABT) is a device that helps blind
users learn to write braille through audio feedback and accessible controls.

## Current Progress
Currently, the primary board is the only board with significant software. The
MCU and Primary UI boards can communicate between themselves and the PC in order
to play some very basic games.
### Current Games (Primary UI)
- Dot practice
 - The SABT prompts the user to press a numbered dot (1-6). If the button
   pressed is correct, the SABT replies 'Good' and prompts the user to find
   another dot. If incorrect, the board replies 'No' and the user must find the
   correct dot.

## Components
There are two different boards that compose the SABT - the Main Control Unit and
one of three User Interface boards.
### Main Control Unit (MCU)
The MCU handles the following:
- Provides connection (via USB cable) to a PC for debugging purposes / mode
  selection
- Provides battery power / power from USB
- Has an SD card slot for sound files
- Plays sound files
- Handles different Modes

### Primary User Interface Board (Primary UI)
- Consists of six large braille dots, essentially one large cell
- Has volume control buttons, mode control buttons, enter buttons
- Transmits signals to MCU when a button is pressed for processing by the MCU

### Intermediate User Interface Board (Intermediate UI)
- Consists of three large braille cells and two rows of actual sized braille
  cells (16 cells per row)

### Advanced User Interface Board (Advanced UI)
- Consists of six rows of braille cells (16 cells per row)

## Menu System
Because the SABT is designed for blind / visually impaired users, the menu
system provides a design challenge 

## Doxygen Documentation  
Documentation at http://cmu-15-239.github.com/SABTSoftware/
