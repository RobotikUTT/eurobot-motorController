'use strict';


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


/**
 * @brief Create a buffer representing the packet data
 * @return buffer representing the packet data
 */

TestPacket.prototype.getBuffer = function() {
    var buffer = new Buffer(this.getArgLength() + 5);
   
    buffer.writeUInt8(0xFF, 0);
    buffer.writeUInt8(0xFF, 1);
    buffer.writeUInt8(0xFF, 2);
    buffer.writeUInt8(this.packetNumber, 3)
    buffer.writeUInt8(this.number, 4);

    var xorSum = this.packetNumber;
    xorSum ^= this.number;

    buffer.writeUInt8(xorSum, 5);

    return buffer;
};


module.exports = TestPacket;