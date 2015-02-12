#include "SerialComCmd.h"

byte SerialComCmd::getCommandLength(byte commandID)
{
    switch(commandID)
    {
        case CMD_TEST:
            return 1;
        default:
            return 0;
    }
}

void SerialComCmd::executeCommand(byte commandID, byte* buffer)
{
    switch(commandID)
    {
        case CMD_TEST: //Ping reception
        {
            //Send pong
            SerialCom serialCom = SerialCom();
            serialCom.setSendCommand(SerialComCmd::CMD_TEST);
            serialCom.writeUInt8(SERIALCOM_EXTRACT_UINT8(0, buffer) + 1);
            serialCom.send();
            break;
        }
    }
}