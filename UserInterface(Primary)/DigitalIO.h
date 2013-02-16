/**
 * @file DigitalIO.h
 * @brief Definitions for Input/Output
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _DIGITAL_IO_H_
#define _DIGITAL_IO_H

//Braill inputs for primary interface (Digital)
#define UI_BR1 4
#define UI_BR2 1
#define UI_BR3 5
#define UI_BR4 2
#define UI_BR5 3
#define UI_BR6 0

//User inputs on the PORTB
#define UI_MODE1 1
#define UI_MODE2 0
#define UI_ENTER1 7
#define UI_ENTER2 6

//User inputs in the PORTD
#define UI_VOLUP 3
#define UI_VOLDOWN 2

//User outputs for PORTD
#define UI_STAT1 6
#define UI_STAT2 7

/* Digital IO button states:
 * 0 - Idle
 * 1 - Button pressed
 * 2 - Button command used
 */

volatile char Enter1State;
volatile char Enter2State;
volatile char Mode1State;
volatile char Mode2State;
volatile char VolUpState;
volatile char VolDownState;

void Initialize_Digital_IO(void);
void CheckCommandButtons(void);
void RunCommandTasks(void);

void SetStatLED1(bool bState);
void SetStatLED2(bool bState);
void Enter1Task(void);
void Enter2Task(void);
void Mode1Task(void);
void Mode2Task(void);
void VolUpTask(void);
void VolDownTask(void);

#endif /* _DIGITAL_IO_H_ */
