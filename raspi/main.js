'use strict';

var MotorController = require('./MotorController');
var controlPannel   = require('./controlPannel/server.js');
var serialPort      = require('serialport');
var readline        = require('readline');
var logger          = require('./libs').logger;
var config          = require('./libs').config;

logger.initIO(controlPannel);
var log = logger.getLogger(module);


var motorController = null;

if (config.get('port')) {
    motorController = new MotorController(config.get('port'));
}
else {
    serialPort.list(function (err, ports) {
        if (err) {
            log.error(err);
            return;
        }

        if (ports.length === 0) {
            log.error('No open ports');
        }
        else if (ports.length === 1) {
            motorController = new MotorController(ports[0].comName);
        }
        else {
            for (var i = 0; i < ports.length; i++) {
                log.info(i + ': ' + ports[i].comName);
            }

            var rl = readline.createInterface({
                input: process.stdin,
                output: process.stdout
            });

            log.info('Choose a com port: ');

            rl.question('', function(answer) {
                motorController = new MotorController(ports[parseInt(answer)].comName);
            });
        }
    });    
}

