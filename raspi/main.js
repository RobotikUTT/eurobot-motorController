'use strict';

var MotorController = require('./MotorController');
var controlPannel   = require('./controlPannel/server.js');
var serialPort      = require('serialport');
var readline        = require('readline');
var Promise         = require('bluebird');
var logger          = require('./libs').logger;
var config          = require('./libs').config;
var IA              = require('./IA/IA');

Promise.promisifyAll(readline);
Promise.promisifyAll(serialPort);

logger.initIO(controlPannel);
var log = logger.getLogger(module);


serialPort.listAsync()
    .then(function(ports) {
        return new Promise(function(resolve, reject) {
            if (ports.length === 0) {
                return reject('No open ports');
            }
            
            if (ports.length === 1) {
                var motorController = new MotorController(ports[0].comName);
                return resolve(motorController);
            }

            //Display open ports
            for (var i = 0; i < ports.length; i++) {
                log.info(i + ': ' + ports[i].comName);
            }

            var rl = readline.createInterface({
                input: process.stdin,
                output: process.stdout
            });

            log.info('Choose a com port: ');

            rl.questionAsync()
                .then(function(answer) {
                    var motorController = new MotorController(ports[parseInt(answer)].comName);
                    resolve(motorController)
                })
                .catch(reject);
        })

    })
    .then(function(motorController) {
        var IA_ = new IA(motorController);
        IA_.start();
    })
    .catch(function(err) {
        log.error(err);
    });