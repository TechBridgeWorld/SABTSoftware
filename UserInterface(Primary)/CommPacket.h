#ifndef _COMM_PACKET_H_
#define _COMM_PACKET_H_

volatile char SendMsgPayLoad[5];
volatile unsigned char MCU_Packet[20];
volatile int iPktSize;

uint16_t Calculate_CRC(unsigned char* pstrMsg);
bool MCU_PKT_CompilePacket(char cmd, char* pl, int plLen);
void SendPacket(char cmd, char* payLoad, int plLen);

#endif
