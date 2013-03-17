/**
 * @file sd_routines.h
 * @brief definitions for SD card stuff
 * @author CC Dharmani, Chennai (India)
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 *
 * **************************************************************
 *  ****** FUNCTIONS FOR SD RAW DATA TRANSFER *******
 * **************************************************************
 * Controller: ATmega32 (Clock: 8 Mhz-internal)
 * Compiler  : AVR-GCC (winAVR with AVRStudio)
 * Version   : 2.3
 * Author  : CC Dharmani, Chennai (India)
 *         www.dharmanitech.com
 * Date    : 08 May 2010
 * **************************************************************
 */

#ifndef _SD_ROUTINES_H_
#define _SD_ROUTINES_H_

//Use following macro if you don't want to activate the multiple block access functions
//those functions are not required for FAT32

#define FAT_TESTING_ONLY         

//use following macros if PB1 pin is used for Chip Select of SD
#define SD_CS_ASSERT     PORTB &= ~0x01
#define SD_CS_DEASSERT   PORTB |= 0x01

//use following macros if SS (PB4) pin is used for Chip Select of SD
//#define SD_CS_ASSERT     PORTB &= ~0x10
//#define SD_CS_DEASSERT   PORTB |= 0x10

//SD commands, many of these are not used here
#define GO_IDLE_STATE             0
#define SEND_OP_COND              1
#define SEND_IF_COND              8
#define SEND_CSD                  9
#define STOP_TRANSMISSION         12
#define SEND_STATUS               13
#define SET_BLOCK_LEN             16
#define READ_SINGLE_BLOCK         17
#define READ_MULTIPLE_BLOCKS      18
#define WRITE_SINGLE_BLOCK        24
#define WRITE_MULTIPLE_BLOCKS     25
#define ERASE_BLOCK_START_ADDR    32
#define ERASE_BLOCK_END_ADDR      33
#define ERASE_SELECTED_BLOCKS     38
#define SD_SEND_OP_COND           41   // ACMD
#define APP_CMD                   55
#define READ_OCR                  58
#define CRC_ON_OFF                59

#define ON     1
#define OFF    0

volatile unsigned long start_block, total_blocks; 
volatile unsigned char sdhc_flag, card_type, buffer[512];

unsigned char sd_init(void);
unsigned char sd_send_command(unsigned char cmd, unsigned long arg);
unsigned char sd_read_single_block(unsigned long start_block);
unsigned char sd_write_single_block(unsigned long start_block);
unsigned char sd_read_multiple_blocks(unsigned long start_block, 
                                      unsigned long total_blocks);
unsigned char sd_write_multiple_blocks(unsigned long start_block, 
                                       unsigned long total_blocks);
unsigned char sd_erase(unsigned long start_block, unsigned long total_blocks);

#endif /* _SD_ROUTINES_H_ */
