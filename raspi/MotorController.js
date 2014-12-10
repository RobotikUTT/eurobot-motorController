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


    //Attributes
    this.address = address;
    this.baudrate = baudrate;
    var parser = new PacketParser();

    this.serialPort = new SerialPort(this.address, {
        baudrate: this.baudrate,
        parser: function(emitter, buffer) {
            parser.parse(emitter, buffer);
        }
    });

    var that = this;

    this.onOpen = function(error) {
        if (error) {
            log.error('[ERR] ' + error);
        }

        log.info('[OK] Serial connection is open on ' + this.address);

        this.serialPort.on('data', function(data){
            log.debug('[RECV] ', data);
        });

        this.serialPort.on('error', function(error) {
            log.error('[ERR] ', error);
        });

        this.serialPort.on('close', function() {
            log.info('[INFO] Serial connection is now closed');
        });

        if (callback) callback();
    }

    //Event listeners
    this.serialPort.on('open', function(error) {
        that.onOpen(error);

    });
};


/**
 * @brief Send a packet to the controller
 * 
 * @param packet Packet object
 * @param callback callback function
 */

MotorController.prototype.sendPacket = function(packet, callback) {
    this.serialPort.write(packet.getBuffer(), callback);
};


MotorController.prototype.testCom = function(arg, callback) {
    log.debug('[SEND] testPacket, arg: ' + arg);

    var testPacket = new packets.TestPacket(arg);
    this.sendPacket(testPacket, callback);
};

module.exports = MotorController;