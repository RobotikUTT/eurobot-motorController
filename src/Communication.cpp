#include "Communication.h"

void Communication::execute(byte command, byte length, byte* params)
{
    Serial.print("Warn : Command 0x");
    Serial.print(command, HEX);
    Serial.println(" doen't exist (There is no functions defined YET)");
}