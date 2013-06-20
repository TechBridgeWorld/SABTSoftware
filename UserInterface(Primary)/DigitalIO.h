/**
 * @file DigitalIO.h
 * @brief Definitions for Input/Output
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _DIGITAL_IO_H_
#define _DIGITAL_IO_H

// Braill inputs for primary interface (Digital)
#define UI_BR1 4
#define UI_BR2 1
#define UI_BR3 5
#define UI_BR4 2
#define UI_BR5 3
#define UI_BR6 0

// User inputs on the PORTB
#define UI_MODE1    1
#define UI_MODE2    0
#define UI_ENTER1   7
#define UI_ENTER2   6

// User inputs in the PORTD
#define UI_VOLUP    3
#define UI_VOLDOWN  2

// User outputs for PORTD
#define UI_STAT1    6
#define UI_STAT2    7

// Button states - pressed or not
#define BUTTON_OFF        0   // button not pressed (idle)
#define BUTTON_ON         1   // button pressed, not yet processed
#define BUTTON_PROCESSED  2   // button has been processed

// Payload values for different buttons
#define ENTER1_PAYLOAD      1
#define ENTER2_PAYLOAD      2
#define MODE1_PAYLOAD       3
#define MODE2_PAYLOAD       4
#define VOL_UP_PAYLOAD      6
#define VOL_DOWN_PAYLOAD    5

// State of buttons (BUTTON_ON or BUTTON_OFF)
volatile char enter1_state;
volatile char enter2_state;
volatile char mode1_state;
volatile char mode2_state;
volatile char vol_up_state;
volatile char vol_down_state;
volatile char length_press;

void initialize_digital_io(void);
void check_command_buttons(void);
void run_command_tasks(void);

// Tasks to be run when a button is pressed
void set_stat_led1(bool bState);
void set_stat_led2(bool bState);
void enter1_task(void);
void enter2_task(void);
void mode1_task(void);
void mode2_task(void);
void vol_up_task(void);
void vol_down_task(void);

#endif /* _DIGITAL_IO_H_ */
