/**
 * @file VS1053.h
 * @brief volume controls
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef __VS1053_H__
#define __VS1053_H__

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define MIN_VOL     19280    //0x2828 + 9000(base 10)
#define VOL_INCR    1000     //Min_vol is assigned based off of testing.  IF you go down by more then 9, vol_INCR, when set at
                             //1000. Sound will go up for 4 down presses before becoming completely quiet.  
                             //make sure not to let this happen

volatile bool vs1053_skip_play;

//Public access functions
unsigned char vs1053_initialize(void);                // Init decoder chip
void vs1053_software_reset(void);                     // Software reset routine
bool vs1053_increase_vol(void);
bool vs1053_decrease_vol(void);

//controls sound level
volatile uint16_t vs1053_volume;

void vs1053_write_data(unsigned char data);           // Write MP3 data
// Write to an internal register
void vs1053_write_command(unsigned char addr, unsigned int cmd);  
unsigned int vs1053_read_command(unsigned char address);  //  Read an internal register


void request_to_play_mp3_file(const char* this_file);

#endif /* __VS1053_H__ */
