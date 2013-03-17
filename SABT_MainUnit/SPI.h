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

void spi_initialize(void);
void spi_deselect_all(void);
void spi_select_sd(void);
void spi_select_mp3_data(void);
void spi_select_mp3_cmd(void);
void spi_2x(void);                          // Double speed operation
void spi_1x(void);                          // Single speed operation
unsigned char spi_transmit(unsigned char);
unsigned char spi_receive(void);

#endif /* _SPI_H_ */
