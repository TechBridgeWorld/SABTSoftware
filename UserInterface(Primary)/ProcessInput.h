/**
 * @file ProcessInput.h
 * @brief Handle dot presses
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _PROCESS_INPUT_H_
#define _PROCESS_INPUT_H_

#define NUM_DOTS          6   // Number of braille dots in a cell

#define INTERFACE_READING 1   // reading vs writing formation for dots
#define INTERFACE_WRITING 2   // writing mode

volatile bool dots_pressed[NUM_DOTS];
int recently_pressed[NUM_DOTS];
volatile char interface_type;

char processed_cell_value;
uint8_t pi_last_dot_pressed;

void reset_cell_state(void);
void process_the_dot(void);
void capture_cell_value(void);

#endif /* _PROCESS_INPUT_H_ */
