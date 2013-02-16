/**
 * @file SPI.h
 * @brief TODO not sure what SPI is
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _SPI_H_
#define _SPI_H_

//For prototype device
#define SPI_PORT PORTB
#define SD_SELECT 0
#define MP3_DATA 2
#define MP3_CMD 1
#define MP3_DREQ 3
#define MP3_RESET 4

//For breadboard device
/*
#define SPI_PORT PORTB
#define SD_SELECT 0
#define MP3_DATA 4
#define MP3_CMD 1
#define MP3_DREQ 2
#define MP3_RESET 3
*/

void SPI_Initialize(void);
void SPI_DeselectAll(void);
void SPI_Select_SD(void);
void SPI_Select_MP3_Data(void);
void SPI_Select_MP3_Cmd(void);
void SPI_2X(void);  //Double speed operation
void SPI_1X(void);  //Single speed operation (only needed for 
unsigned char SPI_transmit(unsigned char);
unsigned char SPI_receive(void);

#endif /* _SPI_H_ */
