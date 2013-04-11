/**
 * @file VS1053.c
 * @brief code for the SABT audio codec
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

// TODO find more descriptive variable names
volatile unsigned int temp1 = 0;
volatile unsigned int temp2 = 0;
volatile unsigned int temp3 = 0;
volatile unsigned char temp4;
volatile unsigned char temp5;
volatile unsigned char temp_address;

volatile uint16_t vs1053_volume;

/**
 * @brief initialize the audio codec
 * @return unsigned char - return 0 on success
 *                         return 1 if not written properly to 0x03
 *                         return 2 if not written properly to 0x0B
 *                         return 3 if not written properly to 0x0b
 *                         return 4 if not written properly to 0x00
 */
unsigned char vs1053_initialize(void)
{
  unsigned char retry;
  
  spi_deselect_all();
  SPI_PORT &= ~_BV(MP3_RESET);    // De assert RESET line (Chip in the reset state)
  _delay_ms(20);                  // Hold for 20ms
  SPI_PORT |= _BV(MP3_RESET);     // Assert RESET line (Chip is powered now)
  spi_1x();                       // Low speed mode
  _delay_ms(20);                  // Hold for 20ms
  retry = 0;

  while(vs1053_read_command(0x03) != 0x9800)  //REDO if not written properly  
  {
    // Set PLL register to 3.5 (preferred for normal operation)
    vs1053_write_command(0x03,0x9800);
    
    if(retry++ > 10 ) return 1;     // try this 10 times
  }

  _delay_ms(20);
  
  //vs1053_write_command(0x05,0x000A);        // Set low sample rate
  
  retry = 0;
  while(vs1053_read_command(0x0B) != 0xFEFE)  // REDO if not written properly
  {
    vs1053_write_command(0x0B, 0xfefe);       // Total silence
    if(retry++ > 10 ) return 2;
  }

  vs1053_write_command(0x05, 0xac45);         // Activate sample rate as 44.1kHz stereo

  retry = 0;
  while(vs1053_read_command(0x0b) != 0x2828)  // REDO if not written properly
  {
    vs1053_write_command(0x0b, 0x2828);       // Set volume to a midrange value
    if(retry++ > 10 ) return 3;               // try this for 10 times
  }

  vs1053_volume = 0x2828;
  retry = 0;
  while(vs1053_read_command(0x00) != 0x0800)  // REDO if not written properly
  {
    vs1053_write_command(0x00, 0x0800);       // Set mode reg to SDINEW mode and MICIN
    if(retry++ > 10 ) return 4;               // try this for 10 times
  }

  _delay_ms(20);
  vs1053_software_reset();
  //_delay_ms(20);
  
  spi_2x();
  return 0;
}

void vs1053_software_reset(void)
{
  vs1053_write_command(0x00, 0x0804);         // Software reset via mode register
  _delay_ms(20);
}

bool vs1053_increase_vol(void)
{
  int retry = 0;

  // Increase the global volume setting
  vs1053_volume = vs1053_volume + ((uint16_t)(1 << 8) + 1);
  
  // Check for maximum volume setting
  if(vs1053_volume >= 0xFEFE)
  {
    vs1053_volume = 0xFEFE; 
  }
  else
  {
    // Actually increase the volume
    while(vs1053_read_command(0x0B) != vs1053_volume)   // REDO if not written properly
    {
      vs1053_write_command(0x0B, vs1053_volume);        // Set the requested volume
      if(retry++ > 10) return false;
    }
  }
  return true;
}

/**
 * @brief Decreases the volume on the volume on the sound chip
 * @return bool - whether or not it could complete the required change
 */
bool vs1053_decrease_vol(void)
{
  int retry = 0;

  // Decrease the global volume setting
  vs1053_volume = vs1053_volume - ((uint16_t)(1 << 8) + 1);
  
  // Check for minimum volume
  if(vs1053_volume <= 0x0101)
  {
    vs1053_volume = 0x0101;
  }
  else
  {
    // Actually decrease the volume
    while(vs1053_read_command(0x0B) != vs1053_volume)   // REDO if not written properly
    {
      vs1053_write_command(0x0B, vs1053_volume);        // Set the requested volume
      if(retry++ > 10 ) return false;
    }
  }
  return true;
}

/**
 * @brief ?
 * @param data - unsigned char, transmits data on SPI interface
 * @return Void
 */
void vs1053_write_data(unsigned char data)
{
  spi_select_mp3_data();
  spi_transmit(data);
  spi_deselect_all();
}

/**
 * @brief ?
 * @param addr - unsigned char, address to write into
 * @param cmd - unsigned int, commmand to process
 * @return Void
 */
void vs1053_write_command(unsigned char addr, unsigned int cmd)
{
  temp4 = (cmd & 0xFF00) >> 8;
  temp5 = (cmd & 0x00FF);
  temp_address = addr;
  spi_select_mp3_cmd();
  spi_transmit(0x02);
  spi_transmit(addr);
  spi_transmit(temp4);
  spi_transmit(temp5);
  spi_deselect_all();
}

/**
 * @brief ?
 * @param addr - unsigned char, address to read from
 * @return unsigned int - return combination of SPI_recieves
 */
unsigned int vs1053_read_command(unsigned char addr)
{
  temp_address = addr;
  spi_select_mp3_cmd();
  spi_transmit(0x03);
  spi_transmit(addr);
  temp1 = spi_receive();
  temp2 = spi_receive();
  temp3 = temp1 * 256 + temp2;
  spi_deselect_all();
  return temp3;
}

/**
 * @brief queues an MP3 file to be played
 *        Only one file can be queued to be played at a time (picked up by the main
 *        loop)
 * @param this_file  the name of the MP3 file to be played
 * @return Void
 */
void request_to_play_mp3_file(const char* this_file)
{
  int i = 0, j;
  
  while(i < strlen((char*)g_file_name))
  {
    g_file_name[i++] = '\0';
  }

  i = 0;
  
  // copy the file name to the global variable
  while(this_file[i] != '.')
  {
    g_file_name[i] = this_file[i];
	i ++;
  }

  for(j = 0; j < 4; j ++)
  {
    g_file_name[i] = this_file[i];
    i ++;
  }

  //clear out anything that is left
  for(; i < FILE_NAME_LEN; i ++)
    g_file_name[i] = 0;

  // Mark that a file is queued to be played by the user interface
  ui_mp3_file_pending = true;
}
