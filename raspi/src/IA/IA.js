'use strict';

var Scheduler       = require('node-robot').Scheduler;
var packetNumbers   = require('../serialCom/packets/constants').packetNumber;
var log             = require('../libs').logger.getLogger(module);

var packetNames = {};

for (var prop in packetNumbers) {
    if(packetNumbers.hasOwnProperty(prop)) {
        packetNames[packetNumbers[prop]] = parseInt(prop);
    }
}


function handlePacket(packet) {
    switch (packet.packetNumber) {

        case packetNames.TestPacket:
            log.debug('[RCV] TestPacket. Number: ' + packet.number);

            this.motorController.testCom(packet.number+1, function onFinished() {
                log.debug('[SEND] Test packet. Number: ', packet.number+1);
            });
            
            break;
    }
}

function IA(motorController) {
    this.motorController = motorController;
    this.stream = motorController.serialPort;
    this.scheduler = new Scheduler();
}


IA.prototype.start = function() {
    var self = this;
    this.stream.on('data', function(packet) {
        handlePacket.call(self, packet);
    });
};


IA.prototype.stop = function() {
    this.stream.removeListener(handlePacket);
};


module.exports = IA;