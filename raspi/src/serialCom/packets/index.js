'use strict';

var fs     = require('fs');
var path   = require('path');;
var constants = require('./constants');


var packets = {};

fs
    .readdirSync(__dirname)
    .filter(function(fileName) {
        return (fileName.indexOf('.') !== 0) && (fileName !== 'index.js') 
        && (fileName !== 'constants.js');
    })
    .forEach(function(fileName) {
        fileName = fileName.split('.js')[0];
        packets[fileName] = require(path.join(__dirname, fileName));
    });


module.exports = packets;