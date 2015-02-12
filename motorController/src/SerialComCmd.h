#ifndef SERIALCOMCMD_H
#define SERIALCOMCMD_H

#include <Arduino.h>
#include "SerialCom.h"

class SerialCom;

class SerialComCmd
{
	public:

		/**
		* @brief This is the command code for a command that takes one byte parameter and will increment it and send the new value with the same command
		*/
		static const byte CMD_TEST = 0;


		/**
		* @brief Get length of the params of a command
		*
		* @param commandID - The command id
		*/
		static byte getCommandLength(byte commandID);

		/**
		* @brief give the number of bytes that parameters needs for a command
		*
		* @param commandID - The command id
		* @return the length in bytes of parameters or ERROR_NOTCMD if the command doesn't exist
		*/
		static void executeCommand(byte commandID, byte* buffer);
	
};
#endif
