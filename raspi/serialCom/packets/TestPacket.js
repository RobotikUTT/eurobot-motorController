'use strict';

var log = require('../../libs').logger(module);


var TestPacket = function() {
    this.packetNumber = 0;
    this.number = null;
}


TestPacket.prototype.getArgLength = function() {
    return 1;
};


TestPacket.prototype.setArguments = function(argList) {
    this.number = argList[0];
};


module.exports = TestPacket;