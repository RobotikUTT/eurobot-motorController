'use strict';

var log = require('../../libs').logger(module);


/**
 * @brief constructor
 */

var TestPacket = function() {
    this.packetNumber = 0;
    this.number = null;
}


/**
 * @brief Packet arguments length accessor
 * @return length in Bytes of the packet arguments
 */

TestPacket.prototype.getArgLength = function() {
    return 1;
};


/**
 * @brief get Packet arguments from buffer
 * 
 * @param  argList Buffer containing the arguments
 */

TestPacket.prototype.setArguments = function(argList) {
    this.number = argList[0];
};


module.exports = TestPacket;