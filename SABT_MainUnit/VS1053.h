/**
 * @file VS1053.h
 * @brief TODO idk wut this is
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef __VS1053_H__
#define __VS1053_H__

#include <stdbool.h>
#include <string.h> 

volatile bool VS1053_SKIP_PLAY;

//Public access functions
unsigned char VS1053_Initialize(void);  //Init decoder chip
void VS1053_SoftwareReset(void);  //Software reset routine
bool VS1053_IncreaseVol(void);
bool VS1053_DecreaseVol(void);


void VS1053_WriteData(unsigned char data);  //Write MP3 data
//Write to an internal register
void VS1053_WriteCmd(unsigned char addr, unsigned int cmd);  
unsigned int VS1053_ReadCmd(unsigned char addr);  //Read an internal register

void RequestToPlayMP3file(const char* thisFile);

#endif /* __VS1053_H__ */
