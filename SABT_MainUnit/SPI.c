/**
 * @file SPI.C
 * @brief Sets up the Seriel Peripheral Interface
 * @ref AtATmega1284P(Preferred).pdf
 *  This says that The Serial Peripheral Interface (SPI) allows high
 *  speedsynchronous data transfer between the ATmega1284P and 
 *  peripheral devices or between several AVR devices.
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

/**
 * @brief Initilizes variables for SPI
 * @return Void
 */
void SPI_Initialize(void){
  DDRB |= _BV(SD_SELECT)|_BV(MP3_DATA)|_BV(MP3_CMD)|_BV(MP3_RESET);
  DDRB &= ~_BV(MP3_DREQ);
  DDRB=0xB7;
  SPCR=0x50; //SPE=1, MSTR=1, SPR1=0, SPR0=0 (Fosc/4)
  SPI_2X();
}

void SPI_DeselectAll(void){
  SPI_PORT |= _BV(SD_SELECT)|_BV(MP3_DATA)|_BV(MP3_CMD);
}

void SPI_Select_SD(void){
  SPI_PORT |= _BV(SD_SELECT)|_BV(MP3_DATA)|_BV(MP3_CMD);
  SPI_PORT &= ~_BV(SD_SELECT);
}

void SPI_Select_MP3_Data(void){
  SPI_PORT |= _BV(SD_SELECT)|_BV(MP3_DATA)|_BV(MP3_CMD);
  SPI_PORT &= ~_BV(MP3_DATA);
}

void SPI_Select_MP3_Cmd(void){
  SPI_PORT |= _BV(SD_SELECT)|_BV(MP3_DATA)|_BV(MP3_CMD);
  SPI_PORT &= ~_BV(MP3_CMD);
}

void SPI_2X(void){ //4MHz
  SPCR &= ~_BV(SPR1);
  SPCR &= ~_BV(SPR0);
  SPSR |= _BV(SPI2X);
}

void SPI_1X(void){  //62.5 kHz
  SPCR |= _BV(SPR1)|_BV(SPR0);
  SPSR &= ~_BV(SPI2X);
}

unsigned char SPI_transmit(unsigned char data){
  // Start transmission
  SPDR = data;
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
  data = SPDR;
  return(data);
}

unsigned char SPI_receive(void){
  unsigned char data;
  // Wait for reception complete

  SPDR = 0xff;
  while(!(SPSR & (1<<SPIF)));
  data = SPDR;

  // Return data register
  return data;
}
