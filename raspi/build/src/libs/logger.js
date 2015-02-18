"use strict";




/**
 * Get a logger including the module path
 * @param  module 
 * @return {Object} Logger object
 */

exports.getLogger = getLogger;



/**
 * @brief Set the io object
 * 
 * @param  io_ socket.io instance 
 */

exports.initIO = initIO;
var winston = require("winston");
var util = require("util");


/**
 * Socket io object
 */

var io = null;


/**
 * @brief Retreive filename given a module name
 * 
 * @param  module module name
 * @return filename
 */

function getModuleName(module) {
    return module.filename.split("/").slice(-2).join("/").split(".js")[0];
};


/**
 * Transports through socket io to the control pannel for web debugging
 */

var WebLogger = winston.transports.WebLogger = function (options) {
    this.name = "webLogger";
    this.level = options.level || "info";
};

util.inherits(WebLogger, winston.Transport);

WebLogger.prototype.log = function (level, msg, meta, callback) {
    if (io) {
        io.emit("log", { level: level, msg: msg, meta: meta });
        callback(null, true);
    }
};function getLogger(module) {
    var path = getModuleName(module);
    var options = {
        colorize: true,
        level: "debug"
    };
    options.path = path;

    var transports = [new winston.transports.Console(options), new WebLogger(options)];

    return new winston.Logger({
        transports: transports
    });
}function initIO(io_) {
    io = io_;
}
Object.defineProperty(exports, "__esModule", {
    value: true
});
//# sourceMappingURL=../libs/logger.js.map