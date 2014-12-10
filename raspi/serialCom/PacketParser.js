'use strict';

var log = require('../libs').logger.getLogger(module);
var packets = require('./packets');


var StateEnum = {
    SIGN: 0,
    COMMAND: 1,
    ARGUMENTS: 2,
    XOR: 3
}


/**
 * @brief Constructor
 */

var PacketParser = function() {
    this.state = StateEnum.SIGN;
    this.buffer = new Buffer(0);
    this.xorSum  = 0;
    this.packet = null;
};


/**
 * @brief Reset the state, delete current packet
 */

PacketParser.prototype.dropPacket = function() {
    //Reset state -> wait for a new packet
    this.state = StateEnum.SIGN;
    this.flushBuffer();
    this.packet = null;
};


/**
 * @brief flush buffer
 */

PacketParser.prototype.flushBuffer = function() {
    this.buffer = new Buffer(0);
}


/**
 * @brief Parse a buffer of data
 * 
 * @param emitter EventEmitter object. Will emit error or data
 * @param buffer  databuffer
 */

PacketParser.prototype.parse = function(emitter, buffer) {
    switch(this.state) {

        case StateEnum.SIGN:
            //We are waiting for the three-byte signature : 3*0xFF
            
            //Not enough bytes. We stock them into this.buffer
            if (this.buffer.length + buffer.length < 3) {
                this.buffer = Buffer.concat([buffer, this.buffer]);
                return;
            }

            //Already something in buffer, concat them
            if (this.buffer.length != 0) {
                buffer = Buffer.concat([this.buffer, buffer]);
                this.flushBuffer();
            }

            for (var i = 0; i < buffer.length; i++) {
                if (buffer[i] === buffer[i+1] && buffer[i+1] === buffer[i+2] 
                    && buffer[i] === 0xFF) {

                    //Change state
                    this.state = StateEnum.COMMAND;

                    //If there's more data, slice the signature and parse the remaining
                    if (buffer.length-1 > i+2) {
                        buffer = buffer.slice(i+3);
                        this.parse(emitter, buffer);
                    }

                    return;
                }
            }

            break;
        

        case StateEnum.COMMAND:
            //Analyze command byte
            var packetNumber = buffer[0];

            try {
                this.packet = packets.createPacket(packetNumber);
            }
            catch (e) {
                log.error('Unknow packet number: ' + packetNumber + '. Drop packet');
                this.dropPacket();
                return;    
            } 

            //Add it to the xor sum (first term)
            this.xorSum = packetNumber;

            //Change state
            this.state = StateEnum.ARGUMENTS;

            //If there's more data, slice the packetNumber and parse the remaining
            if (buffer.length > 1){
                this.parse(emitter, buffer.slice(1));
            }

            break;


        case StateEnum.ARGUMENTS:
            //We search the packet arguments
            if (this.buffer.length != 0) {
                //Already something in buffer, concat them
                buffer = Buffer.concat([this.buffer, buffer], 2);
                this.flush();
                return;
            }

            var argLength = this.packet.getArgLength();

            if (buffer.length < argLength)
            {
                //Not enough bytes. Store the buffer
                this.buffer = buffer;
                return;
            }
            
            //Enough bytes, XorSum
            for (var i = 0; i < argLength; i ++) {
                this.xorSum ^= buffer[i];
            }

            //Set arguments
            this.packet.setArguments(buffer.slice(0, argLength));

            //Change state
            this.state = StateEnum.XOR;

            //If there's more data, parse the remaining
            if (buffer.length > argLength)
            {
                this.parse(emitter, buffer.slice(argLength));
            }
            
            break;


        case StateEnum.XOR:
            //Analizing xor sum

            if (this.buffer.length != 0) {
                //Already something in buffer, concat them
                buffer = Buffer.concat([this.buffer, buffer], 2);
                this.flush();
            }


            if (this.xorSum == buffer[0]) {
                emitter.emit('data', this.packet)
            }
            else {
                log.error('Packet with wrong xorSum dropped', this.packet);
            }

            //Reset packet
            this.dropPacket();

            //If there's more data, slice the signature and parse the remaining
            if (this.buffer.length > 1) {
                this.parse(emitter, buffer.slice(1));
            }

            break;
    }
};


module.exports = PacketParser;