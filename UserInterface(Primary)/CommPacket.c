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
 * @param message the message being sent
 * @return checksum
 */
uint16_t calculate_crc(unsigned char* message)
{
  // Get the length of the message
  // Note that message_length doesn't include the two checksum bytes
  unsigned char message_length = *(message + 2) - 5;
  uint16_t checksum = 0;
  
  message += 3;
  
  // Calculate the checksum
  while(message_length > 1)
  {
    checksum += (*(message) << 8) | *(message + 1);
    checksum = checksum & 0xffff;
    message_length -= 2;
    message += 2;
  }
  
  if(message_length > 0) // If the packet size is odd numbered
  {
    checksum = checksum^((int)*(message++));
  }

  return checksum;
}

/**
 * @brief compile packet to send to MCU (?)
 *
 *        [U][I][message_length][msg_number][msgtype][payload][CRC1][CRC2]
 *
 * @param cmd command type (A-E)
 * @param payload the packet message
 * @param payload_length length of the packet message
 * @return true if the packet was created successfuly, false otw 
 */
bool mcu_compile_packet(char cmd, char* payload, int payload_length)
{
  uint16_t checksum;
  int i = 0;

  // Header from ui board must contain UI prefix
  mcu_packet[0]='U';
  mcu_packet[1]='I';

  // Total packet length = payload + header + crc bytes
  mcu_packet_size = payload_length + PACKET_WRAP_LENGTH;
  
  // SABT can only handle packets up to 20 bytes in length
  if (payload_length + PACKET_WRAP_LENGTH > MAX_PACKET_LENGTH) return false;

  mcu_packet[2] = payload_length + PACKET_WRAP_LENGTH;  // Total message length
  mcu_packet[3] = DEFAULT_MSG_NUMBER;                   // Message number
  mcu_packet[4] = cmd;                                  // Command type: A-E
  
  // Copy over the payload
  for(i = 0; i < payload_length; i++) 
  {
    mcu_packet[PACKET_HEADER_LENGTH + i] = *(payload++);
  }

  // Calculate the checksum - place it as the last two bytes
  checksum = calculate_crc((unsigned char*)&mcu_packet);
  mcu_packet[payload_length + PACKET_HEADER_LENGTH] = (unsigned char)(checksum >> 8);
  mcu_packet[payload_length + PACKET_HEADER_LENGTH + 1] = 
    (unsigned char)(checksum & 0xFF);  
  
  return true;
}

/**
 * @brief send the prepared packet
 * @param cmd command (A-E)
 * @param payload the message
 * @param payload_length length of the message
 * @return Void
 * @TODO: make sure sent packages can't race / be garbled
 */
void send_packet(char cmd, char* payload, int payload_length)
{
  int i = 0;

  if(mcu_compile_packet(cmd, payload, payload_length))
  {
    for(i = 0; i < mcu_packet_size; i++)
    {
      USART_transmitByteToMCU(mcu_packet[i]);
    }
  }  
}
