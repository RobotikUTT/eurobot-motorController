'use strict';

var fs     = require('fs');
var path   = require('path');;


var packets = {};

fs
    .readdirSync(__dirname)
    .filter(function(fileName) {
        return (fileName.indexOf('.') !== 0) && (fileName !== 'index.js');
    })
    .forEach(function(fileName) {
        fileName = fileName.split('.js')[0];
        packets[fileName] = require(path.join(__dirname, fileName));
    });


function createPacket(packetNumber) {
    switch (packetNumber) {
        case 0:
            //testPacket
            console.log('chaucheffoune');
            return new packets.TestPacket();
            break;

        default:
            console.log('ici');
            throw new Error();
            break;
    }
}
module.exports = {
    createPacket: createPacket
};