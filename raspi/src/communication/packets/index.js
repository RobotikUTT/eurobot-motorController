let fs     = require('fs');
let path   = require('path');;


/**
 * Fetch all the Packets classes
 */

export let packets = {};

fs
    .readdirSync(__dirname)
    .filter(function(fileName) {
        return ((fileName.indexOf('.') !== 0) && (fileName.indexOf('.map') === -1) 
            && (fileName !== 'Packet.js') && (fileName !== 'index.js'));
    })
    .forEach(function(fileName) {
        fileName = fileName.split('.js')[0];
        packets[fileName] = require(path.join(__dirname, fileName));
    });


/**
 * Packets name and number
 */

export let numberEnum = {
    "TEST": packets.TestPacket.getPacketNumber()
};