/**
 * @file SD_routines.c
 * @brief code to interact with SD card
 * @ref sdcard_appnote_foust
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

//******************************************************************
//Function  : to initialize the SD/SDHC card in SPI mode
//Arguments  : none
//return  : unsigned char; will be 0 if no error,
//         otherwise the response byte will be sent
//******************************************************************

/**
 * @brief initializes the SD/SDHC card in SPI mode
 * @return unsigned char - return 0 if no error and response byte otherwise
 */
unsigned char sd_init(void)
{
  unsigned char i, response, sd_version;
  unsigned int retry = 0;

  for(i = 0; i < 10; i++)
    spi_transmit(0xff);   //80 clock pulses spent before sending the first command
  SD_CS_ASSERT;
  do
  {
    response = sd_send_command(GO_IDLE_STATE, 0); //send 'reset & go idle' command
    retry++;
    if(retry>0x20)
      return 1;   //time out, card not detected
  } while(response != 0x01);

  SD_CS_DEASSERT;
  spi_transmit (0xff);
  spi_transmit (0xff);

  retry = 0;

  sd_version = 2; //default set to SD compliance with ver2.x; 
  //this may change after checking the next command
  do
  {
    // Check power supply status, mendatory for SDHC card
    response = sd_send_command(SEND_IF_COND, 0x000001AA); 
    retry++;
    if(retry > 0xfe) 
    {
      sd_version = 1;
      card_type = 1;
      break;
    }
  }
  while(response != 0x01);

  retry = 0;

  do
  {
    // CMD55, must be sent before sending any ACMD command
    response = sd_send_command(APP_CMD,0); 
    response = sd_send_command(SD_SEND_OP_COND,0x40000000); // ACMD41

    retry++;
    if(retry > 0xfe)
    {
      return 2;  //time out, card initialization failed
    }
  }
  while(response != 0x00);


  retry = 0;
  sdhc_flag = 0;

  if (sd_version == 2)
  {
    do
    {
      response = sd_send_command(READ_OCR,0);
      retry++;
      if(retry > 0xfe)
      {
        card_type = 0;
        break;
      } //time out
    }
    while(response != 0x00);

    if(sdhc_flag == 1) card_type = 2;
    else card_type = 3;
  }

  return 0; //successful return
}

/**
 * @brief sends a command to the SD card
 * @param cmd - unsigned char, command to send to the SD card
 * @param arg - unsigned long, argument of the command sent
 * @return unsigned char - response byte
 */
unsigned char sd_send_command(unsigned char cmd, unsigned long arg)
{
  unsigned char response, retry = 0, status;

  // SD card accepts byte address while SDHC accepts block address in multiples of 512
  // so, if it's SD card we need to convert block address 
  // into corresponding byte address by 
  // multipying it with 512. which is equivalent to shifting it left 9 times
  // following 'if' loop does that

  if(sdhc_flag == 0)    
    if(cmd == READ_SINGLE_BLOCK     ||
        cmd == READ_MULTIPLE_BLOCKS  ||
        cmd == WRITE_SINGLE_BLOCK    ||
        cmd == WRITE_MULTIPLE_BLOCKS ||
        cmd == ERASE_BLOCK_START_ADDR|| 
        cmd == ERASE_BLOCK_END_ADDR ) 
    {
      arg = arg << 9;
    }

  SD_CS_ASSERT;

  spi_transmit(cmd | 0x40); //send command, first two bits always '01'
  spi_transmit(arg >> 24);
  spi_transmit(arg >> 16);
  spi_transmit(arg >> 8);
  spi_transmit(arg);

  // It is compulsory to send correct CRC for CMD8 (CRC=0x87) & CMD0 (CRC=0x95)
  if(cmd == SEND_IF_COND)
    spi_transmit(0x87);    // for remaining commands, CRC is ignored in SPI mode
  else 
    spi_transmit(0x95); 

  while((response = spi_receive()) == 0xff) // wait response
    if(retry++ > 0xfe) break; // time out error

  if(response == 0x00 && cmd == 58)  // checking response of CMD58
  {
    status = spi_receive() & 0x40;     // first byte of the OCR register (bit 31:24)
    if(status == 0x40) sdhc_flag = 1;  // we need it to verify SDHC card
    else sdhc_flag = 0;

    spi_receive(); // remaining 3 bytes of the OCR register are ignored here
    spi_receive(); // one can use these bytes to check power supply limits of SD
    spi_receive(); 
  }

  spi_receive(); // extra 8 CLK
  SD_CS_DEASSERT;

  return response;
}

/**
 * @brief Erases the specified no. of blocks of SD card
 * @return unsigned char - 0 if no error and response byte if error
 */
unsigned char sd_erase (unsigned long start_block, unsigned long total_blocks)
{
  unsigned char response;

  // Send starting block address
  response = sd_send_command(ERASE_BLOCK_START_ADDR, start_block); 

  if(response != 0x00) //check for SD status: 0x00 - OK (No flags set)
    return response;

  // Send end block address
  response = sd_send_command(ERASE_BLOCK_END_ADDR, (start_block + total_blocks - 1));

  if(response != 0x00)
    return response;

  // Erase all selected blocks
  response = sd_send_command(ERASE_SELECTED_BLOCKS, 0); 
  if(response != 0x00)
    return response;

  return 0;
}

/**
 * @brief Reads a single block from SD card
 *        Reads 512 bytes (SPI Block) from SD Card
 * @return unsigned char - 0 if no error and response byte if error
 */
unsigned char sd_read_single_block(unsigned long start_block)
{
  unsigned char response;
  unsigned int i, retry = 0;

  response = sd_send_command(READ_SINGLE_BLOCK, start_block); //read a Block command

  if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

  SD_CS_ASSERT;

  retry = 0;
  while(spi_receive() != 0xfe) //wait for start block token 0xfe (0x11111110)
    if(retry++ > 0xfffe)
    {
      SD_CS_DEASSERT; 
      return 1;
    } //return if time-out

  for(i = 0; i < 512; i++) //read 512 bytes
    buffer[i] = spi_receive();

  spi_receive(); //receive incoming CRC (16-bit), CRC is ignored here
  spi_receive();

  spi_receive(); //extra 8 clock pulses
  SD_CS_DEASSERT;

  return 0;
}


/**
 * @brief Reads a single block from SD card, specifically for the dictionary
 *        functions
 *        Reads 512 bytes (SPI Block) from SD Card
 * @return unsigned char - 0 if no error and response byte if error
 */
unsigned char sd_read_single_dict_block(unsigned long start_block)
{
  unsigned char response;
  unsigned int i, retry = 0;

  response = sd_send_command(READ_SINGLE_BLOCK, start_block); //read a Block command

  if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

  SD_CS_ASSERT;

  retry = 0;
  while(spi_receive() != 0xfe) //wait for start block token 0xfe (0x11111110)
    if(retry++ > 0xfffe)
    {
      SD_CS_DEASSERT; 
      return 1;
    } //return if time-out

  for(i = 0; i < 512; i++) //read 512 bytes
    dict_buffer[i] = spi_receive();

  spi_receive(); //receive incoming CRC (16-bit), CRC is ignored here
  spi_receive();

  spi_receive(); //extra 8 clock pulses
  SD_CS_DEASSERT;

  return 0;
}


/**
 * @brief Writes a single block of SD Card. Data that is written is put into the
 *        buffer variables and writes out the 512 charachters.  
 * @param start_block - unsigned long, describes which block you want to right 
 * @return unsigned char - 0 if no error
 *                         response byte will be sent if an error
 */
unsigned char sd_write_single_block(unsigned long start_block)
{
  unsigned char response;
  unsigned int i, retry = 0;

  response = sd_send_command(WRITE_SINGLE_BLOCK, start_block); //write a Block command

  if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)
  SD_CS_ASSERT;

  spi_transmit(0xfe);     //Send start block token 0xfe (0x11111110)

  for(i = 0; i < 512; i++)    //send 512 bytes data
    spi_transmit(buffer[i]);

  spi_transmit(0xff);     //transmit dummy CRC (16-bit), CRC is ignored here
  spi_transmit(0xff);

  response = spi_receive();

  if((response & 0x1f) != 0x05) //response= 0xXXX0AAA1 ; AAA='010' - data accepted
  {                              //AAA='101'-data rejected due to CRC error
    SD_CS_DEASSERT;              //AAA='110'-data rejected due to write error
    return response;
  }

  while(!spi_receive()) //wait for SD card to complete writing and get idle
    if(retry++ > 0xfffe)
    {
      SD_CS_DEASSERT; 
      return 1;
    }

  SD_CS_DEASSERT;
  spi_transmit(0xff);   //just spend 8 clock cycle delay before reasserting the CS line
  SD_CS_ASSERT;         //re-asserting the CS line to verify if card is still busy

  while(!spi_receive()) //wait for SD card to complete writing and get idle
    if(retry++ > 0xfffe)
    {
      SD_CS_DEASSERT; 
      return 1;
    }

  SD_CS_DEASSERT;

  return 0;
}


#ifndef FAT_TESTING_ONLY

/**
 * @brief Reads multiple blocks from the SD card and send every block to UART
 * @return unsigned char - 0 if no error and response byte if error
 */
unsigned char sd_read_multiple_blocks (unsigned long start_block, 
    unsigned long total_blocks)
{
  unsigned char response;
  unsigned int i, retry = 0;

  retry = 0;

  response = sd_send_command(READ_MULTIPLE_BLOCKS, start_block); //write a Block command

  if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

  SD_CS_ASSERT;

  while(total_blocks)
  {
    retry = 0;
    while(spi_receive() != 0xfe) //wait for start block token 0xfe (0x11111110)
      if(retry++ > 0xfffe)
      {
        SD_CS_DEASSERT; 
        return 1;
      } //return if time-out

    for(i = 0; i < 512; i++) //read 512 bytes
      buffer[i] = spi_receive();

    spi_receive(); //receive incoming CRC (16-bit), CRC is ignored here
    spi_receive();

    spi_receive(); //extra 8 cycles

    for(i = 0; i < 512; i++) //send the block to UART
    {
      if(buffer[i] == '~') break;
      transmit_byte(buffer[i]);
    }

    total_blocks--;
  }

  sd_send_command(STOP_TRANSMISSION, 0); //command to stop transmission
  SD_CS_DEASSERT;
  spi_receive(); //extra 8 clock pulses

  return 0;
}

/**
 * @brief Recieves data from UART and writes to multiple blocks of SD card
 * @return unsigned char - response byte
 */
unsigned char sd_write_multiple_blocks(unsigned long start_block, 
    unsigned long total_blocks)
{
  unsigned char response, data;
  unsigned int i, retry = 0;
  unsigned long block_counter = 0, size;

  response = sd_send_command(WRITE_MULTIPLE_BLOCKS, start_block); //write a Block command

  if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

  SD_CS_ASSERT;

  while( block_counter < total_blocks )
  {
    i = 0;
    do
    {
      data = receive_byte();
      if(data == 0x08)  //'Back Space' key pressed
      { 
        if(i != 0)
        { 
          transmit_byte(data);
          transmit_byte(' '); 
          transmit_byte(data); 
          i--; 
          size--;
        } 
        continue;     
      }
      transmit_byte(data);
      buffer[i++] = data;
      if(data == 0x0d)
      {
        transmit_byte(0x0a);
        buffer[i++] = 0x0a;
      }
      if(i == 512) break;
    }
    while (data != '~');

    spi_transmit(0xfc); //Send start block token 0xfc (0x11111100)

    for(i = 0; i < 512; i++) //send 512 bytes data
      spi_transmit(buffer[i]);

    spi_transmit(0xff); //transmit dummy CRC (16-bit), CRC is ignored here
    spi_transmit(0xff);

    response = spi_receive();
    if((response & 0x1f) != 0x05) //response= 0xXXX0AAA1 ; AAA='010' - data accepted
    {                              //AAA='101'-data rejected due to CRC error
      SD_CS_DEASSERT;             //AAA='110'-data rejected due to write error
      return response;
    }

    while(!spi_receive()) //wait for SD card to complete writing and get idle
      if(retry++ > 0xfffe)
      {
        SD_CS_DEASSERT; 
        return 1;
      }

    spi_receive(); //extra 8 bits
    block_counter++;
  }

  spi_transmit(0xfd); //send 'stop transmission token'
  retry = 0;

  while(!spi_receive()) //wait for SD card to complete writing and get idle
    if(retry++ > 0xfffe)
    {
      SD_CS_DEASSERT; 
      return 1;
    }

  SD_CS_DEASSERT;
  spi_transmit(0xff); //just spend 8 clock cycle delay before reasserting the CS signal

  // re assertion of the CS signal is required to verify if card is still busy
  SD_CS_ASSERT; 

  while(!spi_receive()) //wait for SD card to complete writing and get idle
    if(retry++ > 0xfffe)
    {
      SD_CS_DEASSERT; 
      return 1;
    }

  SD_CS_DEASSERT;

  return 0;
}

#endif

//******** END ****** www.dharmanitech.com *****
