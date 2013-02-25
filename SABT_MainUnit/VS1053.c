/**
 * @file VS1053.c
 * @brief code for the SABT audio codec
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

volatile unsigned int temp1=0;
volatile unsigned int temp2=0;
volatile unsigned int temp3=0;
volatile unsigned char temp4;
volatile unsigned char temp5;
volatile unsigned char tempAddr;

volatile uint16_t VS1053_Vol;

/**
 * @brief initialize the audio codec
 * @return unsigned char - return 0 on success
 *                         return 1 if not written properly to 0x03
 *                         return 2 if not written properly to 0x0B
 *                         return 3 if not written properly to 0x0b
 *                         return 4 if not written properly to 0x00
 */
unsigned char VS1053_Initialize(void)
{
  unsigned char retry;
  SPI_DeselectAll();
  SPI_PORT &= ~_BV(MP3_RESET);  //De assert RESET line (Chip in the reset state)
  _delay_ms(20);          //Hold for 20ms
  SPI_PORT |= _BV(MP3_RESET);    //Assert RESET line (Chip is powered now)
  SPI_1X();            //Low speed mode
  _delay_ms(20);          //Hold for 20ms
  retry=0;

  while(VS1053_ReadCmd(0x03) != 0x9800)  //REDO if not written properly  
  {
    VS1053_WriteCmd(0x03,0x9800);    //set PLL register to 3.5 (preferred for normal operation)
    if(retry++ >10 )return 1;      //try this for 10 times
  }

  _delay_ms(20);
  
  //VS1053_WriteCmd(0x05,0x000A);      //Set low sample rate
  
  retry=0;
  while(VS1053_ReadCmd(0x0B) != 0xFEFE)  //REDO if not written properly
  {
    VS1053_WriteCmd(0x0B,0xfefe);    //Total silence
    if(retry++ >10 )return 2;
  }

  VS1053_WriteCmd(0x05,0xac45);      //Activate sample rate as 44.1kHz stereo

  retry=0;
  while(VS1053_ReadCmd(0x0b) != 0x2828)  //REDO if not written properly
  {
    VS1053_WriteCmd(0x0b,0x2828);    //Set volume to a midrange value
    if(retry++ >10 )return 3;      //try this for 10 times
  }
  VS1053_Vol=0x2828;
  retry=0;
  while(VS1053_ReadCmd(0x00) != 0x0800)      //REDO if not written properly
  {
    VS1053_WriteCmd(0x00,0x0800);        //Set mode register to SDINEW mode and MICIN
    if(retry++ >10 )return 4;          //try this for 10 times
  }
  _delay_ms(20);
  VS1053_SoftwareReset();
  //_delay_ms(20);
  SPI_2X();
  return 0;
}

void VS1053_SoftwareReset(void)
{
  VS1053_WriteCmd(0x00,0x0804);    //Software reset via mode register
  _delay_ms(20);
}

bool VS1053_IncreaseVol(void)
{
  int retry=0;
  VS1053_Vol=VS1053_Vol+((uint16_t)(1<<8)+1);
  if(VS1053_Vol>=0xFEFE)
  {
    VS1053_Vol=0xFEFE; 
  }
  else
  {
    while(VS1053_ReadCmd(0x0B) != VS1053_Vol)  //REDO if not written properly
    {
      VS1053_WriteCmd(0x0B,VS1053_Vol);    //Set the requested volume
      if(retry++ >10 )return false;
    }
  }
  return true;
}

/**
 * @brief Decreases the volume on the volume on the sound chip
 * @return bool - whether or not it could complete the required change
 */
bool VS1053_DecreaseVol(void)
{
  int retry=0;
  VS1053_Vol=VS1053_Vol-((uint16_t)(1<<8)+1);
  if(VS1053_Vol<=0x0101)
  {
    VS1053_Vol=0x0101;
  }
  else
  {
    while(VS1053_ReadCmd(0x0B) != VS1053_Vol)  //REDO if not written properly
    {
      VS1053_WriteCmd(0x0B,VS1053_Vol);    //Set the requested volume
      if(retry++ >10 )return false;
    }
  }
  return true;
}          

/**
 * @brief ?
 * @param data - unsigned char, transmits data on SPI interface
 * @return Void
 */
void VS1053_WriteData(unsigned char data)
{
  SPI_Select_MP3_Data();
  SPI_transmit(data);
  SPI_DeselectAll();
}

/**
 * @brief ?
 * @param addr - unsigned char, address to write into
 * @param cmd - unsigned int, commmand to process
 * @return Void
 */
void VS1053_WriteCmd(unsigned char addr, unsigned int cmd)
{
    temp4 = (cmd & 0xFF00) >> 8;
    temp5 = (cmd & 0x00FF);
  tempAddr=addr;
  SPI_Select_MP3_Cmd();
  SPI_transmit(0x02);
  SPI_transmit(addr);
  SPI_transmit(temp4);
  SPI_transmit(temp5);
  SPI_DeselectAll();
}

/**
 * @brief ?
 * @param addr - unsigned char, address to read from
 * @return unsigned int - return combination of SPI_recieves
 */
unsigned int VS1053_ReadCmd(unsigned char addr)
{
  tempAddr=addr;
  SPI_Select_MP3_Cmd();
  SPI_transmit(0x03);
  SPI_transmit(addr);
  temp1 = SPI_receive();
  temp2 = SPI_receive();
  temp3 = temp1*256 + temp2;
  SPI_DeselectAll();
  return temp3;
}

/**
 * @brief queues an MP3 file to be played
 *        Only one file can be queued to be played at a time (picked up by the main
 *        loop)
 * @param thisFile  the name of the MP3 file to be played
 * @return Void
 */
void RequestToPlayMP3file(const char* thisFile)
{
  int i = 0;
  
  while(i < strlen((char*)fileName))
  {
    fileName[i++] = '\0';
  }

  DPRINTF("fileName = %s\n\r", fileName);
  i = 0;
  
    // copy the file name to the global variable
  while(*thisFile != '.')
  {
    fileName[i++] = *(thisFile++);
  }
  fileName[i++] = *(thisFile++);
  fileName[i++] = *(thisFile++);
  fileName[i++] = *(thisFile++);
  fileName[i++] = *(thisFile++);
  DPRINTF("end fileName = %s\n\r", fileName);

  // Mark that a file is queued to be played by the user interface
  UI_MP3_file_Pending = true;
}
