/**
 * @file GlobalsUI.h
 * @brief contains global includes and definitions
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _GLOBALS_UI_H_
#define _GLOBALS_UI_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

#include "DigitalIO.h"
#include "USART_MCU.h"
#include "Timer.h"
#include "CommPacket.h"
#include "ProcessInput.h"

void delay();

#endif /* _GLOBALS_H_ */
