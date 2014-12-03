'use strict';

var winston = require('winston');
var utils   = require('./utils');
var config  = require('./config');


/**
 * Get a logger including the module path
 * @param  {Object} module module name
 * @return {Object} Logger object
 */

function getLogger(module) {
    //using filename in log statements
    var path = utils.getModuleName(module); 
    
    return new winston.Logger({
        transports : [
            new winston.transports.Console({
                colorize: true,
                level: config.get('log').level,
                label: path
            })
        ]
    });
}


module.exports = getLogger;