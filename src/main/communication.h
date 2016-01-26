#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

/**
 * UART Packet description:

    +--------------+----------+---------------------+----------+
    | Data    Size | Command  | Payload             | Checksum |
    | (1-2 bytes)  | (1 byte) | (0 to payload size) |  1 byte  |
    +--------------+----------+---------------------+----------+

    * Data size
      - By data size we understand size of payload plus size of command
        field. For a packet with no payload at all, data size is 1.

        Data size field can be 1 byte or two bytes long. For data size
        less than 128 it is depicted as one-byte. If the most significant
        bit is set, then another byte will follow. Data size will then
        be DATASIZE & 0x7FFF, you'll have to clear the MSbit.

        Examples:

          payload size 3, data size is one byte, 0x04
          payload size 512, data size is two bytes, 0x82 0x01

    * Command
      - Command or reply. 1 byte long.

    * Payload
      - Variable size payload for this packet. Size is depicted by Payload
        Size. If Payload Size is zero, this field is not present in packet.

    * Checksum
      - Packet checksum, using XOR with 0 as start value. To compute checksum
        for an outgoing packet, XOR all values of the packet except for the
        checksum itself. To validate checksum of a packet, XOR all bytes
        received including checksum. If result is zero, then packet has no
        errors.

    Packets received by arduino have a size limit, due to memory size constraints.
    Any packet received with size greater than this limit is ignored.

    Reset procedure

    Before sending out commands, you should reset the state machines by issuing
    at few number of zeroes. The minimum number of zeroes to send is
    arduino packet size limit + 1. This will ensure proper reset.

    Thanks alvaro: http://forum.arduino.cc/index.php?topic=8091.0
 */

/**
 * Commands IDs
 */

#define MAX_PACKET_SIZE 12

#define CMD_PING        0x00
#define CMD_PONG        0x01
#define CMD_SET_TUNINGS 0x02

typedef enum ReadState {
	SIZE,
	SIZE2,
	CMD,
	DATA,
	CRC
} ReadState_t;

/**
 * Init UART and CAN bus
 */
void initCommunication();

/**
 * Handle incoming data
 */
void processCommunication();

/**
 * Handle incoming UART data
 */
void processUART();

/**
 * Handle incoming CAN data
 */
void processCAN();

/**
 * Process a complete packet
 * @param cmd  Command identifier
 * @param buf  Data buffer
 * @param size Size of `buf`
 */
void processPacket(uint8_t cmd, uint8_t* buf, uint16_t size);

#endif
