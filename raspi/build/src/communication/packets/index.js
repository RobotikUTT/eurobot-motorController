"use strict";

var fs = require("fs");
var path = require("path");;


/**
 * Fetch all the Packets classes
 */

var packets = exports.packets = {};

fs.readdirSync(__dirname).filter(function (fileName) {
    return fileName.indexOf(".") !== 0 && fileName.indexOf(".map") === -1 && fileName !== "Packet.js" && fileName !== "index.js";
}).forEach(function (fileName) {
    fileName = fileName.split(".js")[0];
    packets[fileName] = require(path.join(__dirname, fileName));
});


/**
 * Packets name and number
 */

var numberEnum = exports.numberEnum = {
    TEST: packets.TestPacket.getPacketNumber()
};
Object.defineProperty(exports, "__esModule", {
    value: true
});
//# sourceMappingURL=../../communication/packets/index.js.map