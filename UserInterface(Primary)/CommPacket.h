/**
 * @file CommPacket.h
 * @brief Definitions for building packets
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#ifndef _COMM_PACKET_H_
#define _COMM_PACKET_H_

#define MAX_MSG_PAYLOAD_LENGTH  5   // Maximum total length of packet payload
#define MAX_PACKET_LENGTH       20  // Maximum total length of a packet
#define PACKET_HEADER_LENGTH    5   // Header of a message length
#define PACKET_WRAP_LENGTH      7   // Number of bytes not in payload

#define DEFAULT_MSG_NUMBER      2   // Currently a dummy value for msg number

// The payload for the current message being built
volatile char mcu_message_payload[MAX_MSG_PAYLOAD_LENGTH];

// The completed packet to be sent to the MCU
volatile unsigned char mcu_packet[MAX_PACKET_LENGTH];

// The size of mcu_packet
volatile int mcu_packet_size;

uint16_t calculate_crc(unsigned char* message);
bool mcu_compile_packet(char cmd, char* payload, int payload_length);
void send_packet(char cmd, char* payload, int payload_length);

#endif /* _COMM_PACKET_H_ */
