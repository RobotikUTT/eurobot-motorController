'use strict';

var SerialPort   = require('serialport').SerialPort;
var log          = require('./libs').logger.getLogger(module);
var PacketParser = require('./serialCom/PacketParser');
var packets      = require('./serialCom/packets');


/**
 * @brief constructor
 * @details object interfacing the motor controller with serial.
 * Provide events of packet fully-recieved and packet-forging function.
 * 
 * @param address serial address
 * @param baudrate connection speed
 * @param callback callback called once the connection is opened
 */

var MotorController = function(address, baudrate, callback) {
    //Type checking
    if (!typeof(address) === 'string') throw new TypeError('address must be a string');
    if (!typeof(baudrate) === 'number') throw new TypeError('address must be a string');
    if (!typeof(baudrate) === 'function') throw new TypeError('callback must be a function');


    //Default arguments
    if (typeof(address) === 'undefined' || address === null) address = '/dev/ttyACM0';
    if (typeof(baudrate) === 'undefined' || baudrate === null) baudrate = '9600';


    var parser = new PacketParser();

    this.serialPort = new SerialPort(address, {
        baudrate: baudrate,
        parser: function(emitter, buffer) {
            parser.parse(emitter, buffer);
        }
    });

    var onOpen = function(error) {
        if (error) {
            return log.error(JSON.stringify(error));
        }

        log.info('[OK] Serial connection is open on ' + address);
        
        this.serialPort.on('error', function(error) {
            log.error(JSON.stringify(error));
        });

        this.serialPort.on('close', function() {
            log.info('Serial connection is now closed');
        });

        if (callback) 
            callback();
    }

    //Event listeners
    var self = this;
    this.serialPort.on('open', function(error) {
        onOpen.call(self, error);
    });
};


/**
 * @brief Send a packet to the controller
 * 
 * @param packet Packet object
 * @return A promise
 */
MotorController.prototype.sendPacket = function(packet) {
    return this.serialPort.writeAsync(packet.getBuffer());
};


/**
 * @brief Test the communication with the controller
 * 
 * @param arg number to send (Byte)
 * @return A promise
 */
MotorController.prototype.testCom = function(arg) {
    var testPacket = new packets.TestPacket;
    testPacket.number = arg;

    return this.sendPacket(testPacket);
};


module.exports = MotorController;