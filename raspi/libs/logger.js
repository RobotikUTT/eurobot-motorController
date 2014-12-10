'use strict';

var winston = require('winston');
var utils   = require('./utils');
var config  = require('./config');
var util = require('util');


var io = null;


var WebLogger = winston.transports.WebLogger = function(options) {
    this.name = 'webLogger';
    this.level = options.level || 'info';
};

util.inherits(WebLogger, winston.Transport);

WebLogger.prototype.log = function (level, msg, meta, callback) {
    if (io)
    {
        io.emit('log', { level: level, msg: msg, meta: meta });
        callback(null, true);
    }
};


/**
 * Get a logger including the module path
 * @param  {Object} module module name
 * @return {Object} Logger object
 */

function getLogger(module) {
    //using filename in log statements
    var path = utils.getModuleName(module);
    var options = config.get('log');
    options.path = path;

    var transports = [ new winston.transports.Console(options),
        new WebLogger(options)
    ];

    return new winston.Logger({
        transports: transports
    });
}


function initIO(controlPannel) {
    io = controlPannel;
}


module.exports = {
    getLogger: getLogger,
    initIO: initIO
}