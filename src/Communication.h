#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Wire.h>
#include <Arduino.h>
#include "I2cSlaveProtocol.h"
#include "Odometry.h"
#include "Enslavement.h"
#include "Motor.h"
#include "Encoder.h"


/**
 * @class Communication
 * @brief Communication class implemant the i2c slave protocol defined in class I2cSlaveProtocol. I2cSlaveProtocol doesnt' change between arduino boards but Communication will change as it contain command code.
 */
class Communication : public I2cSlaveProtocol
{
    public:

        /**
         * Constructor with a data available line
         * @param address - i2c address
         * @param dataAvailablePin - Pin that will be put up when the slave has data available
         */
        static void open(byte address, byte dataAvailablePin);

        /**
         * Constructor without a data available line
         * @param address - i2c address
         */
        static void open(byte address);

        /**
         * Called when a command is received
         * @param command - The command number
         * @param paramLength - The length of the param buffer
         * @param paramBuf - The list of bytes that constitue params
         */
        static void execute(byte command, byte paramLength, byte *paramBuf);
        /**
         * Called when master request data
         */
        static void send();

        /**
         * Give motors objects for debug
         * @param left - The left motor object
         * @param right - The right motor object
         */
        static void setMotors(Motor* left, Motor* right);

        /**
         * Give encoders objects for debug
         * @param left - The left encoder object
         * @param right - The right encoder object
         */
        static void setEncoders(Encoder* left, Encoder* right);

        static void setEnslavement(Enslavement* enslavement);


        static byte lastCmd;
        static byte lastPingU8;
        static char lastPingS8;
        static unsigned short lastPingU16;
        static short lastPingS16;
        static Odometry* odometry;
        static Enslavement* enslavement;

        static Encoder *leftEncoder;
        static Encoder *rightEncoder;
        static Motor *leftMotor;
        static Motor *rightMotor;


        static const byte cmd_ping = 0x0;
        static const byte cmd_move = 0x1;
        static const byte cmd_odometry = 0x2;
        static const byte cmd_stop = 0x3;
        static const byte cmd_set_motor_pwm = 0x4;
        static const byte cmd_get_encoder_ticks = 0x5;
        static const byte cmd_reset_encoder_ticks = 0x6;
};

#endif