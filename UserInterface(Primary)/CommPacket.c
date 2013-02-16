/**
 * @file CommPacket.c
 * @brief handle communication packets
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "GlobalsUI.h"

/**
 * @brief calculate cyclic redundancy checks (CRC)
 * @param pStrMsg the message being sent
 * @return checksum
 */
uint16_t Calculate_CRC(unsigned char* pstrMsg)
{
  unsigned char msglen=*(pstrMsg+2)-5;//Not including the checksum bytes
  uint16_t chksum=0;
  pstrMsg+=3;
  while(msglen > 1)
  {
    chksum+=(*(pstrMsg)<<8) | *(pstrMsg+1);
    chksum = chksum & 0xffff;
    msglen-=2;
    pstrMsg+=2;
  }
  if(msglen>0) //If the packet size is odd numbered
  {
    chksum = chksum^ (int)*(pstrMsg++);
  }
  return(chksum);
}

/**
 * @brief compile packet to send to MCU (?)
 *
 * [U][I][msglen][msg_number][msgtype][payload][CRC1][CRC2]
 *
 * @param cmd command type (A-E)
 * @param pl the packet message
 * @param plLen length of the packet message
 * @return true if the packet was created successfuly, false otw 
 */
bool MCU_PKT_CompilePacket(char cmd, char* pl, int plLen)
{
  uint16_t chksum;
  int i = 0;

  //Header always contain the 0xFA and 0xFB
  MCU_Packet[0]='U';
  MCU_Packet[1]='I';
  iPktSize=plLen+7;
  
  if ( plLen+7 > 20 ) //SABT can handle packets upto 20 bytes 
  {
    return false;
  }

  MCU_Packet[2] = plLen+7;
  MCU_Packet[3] = 2;  
  MCU_Packet[4] = cmd;  
  
  for(i=0;i<plLen;i++)
  {
    MCU_Packet[5+i]=*(pl++);
  
  }
  
  //memcpy( &MCU_Packet[5], pl, plLen);
  chksum = Calculate_CRC((unsigned char*)&MCU_Packet);
  MCU_Packet[plLen+5] = chksum >> 8;
  MCU_Packet[plLen+6] = chksum & 0xFF;  
  
  return true;
}

/**
 * @brief send the prepared packet
 * @param cmd command (A-E)
 * @param payload the message
 * @param plLen length of the message
 * @return Void
 * TODO: make sure sent packages can't race / be garbled
 */
void SendPacket(char cmd, char* payLoad, int plLen)
{
  int i = 0;

  if(MCU_PKT_CompilePacket(cmd,payLoad,plLen))
  {
    for(i = 0; i < iPktSize; i++)
    {
      USART_transmitByteToMCU(MCU_Packet[i]);
    }
  }  
}
