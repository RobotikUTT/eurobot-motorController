'use strict';

var MotorController = require('./src/MotorController');
var controlPannel   = require('./src/controlPannel/server.js');
var serialPort      = require('serialport');
var readline        = require('readline');
var Promise         = require('bluebird');
var logger          = require('./src/libs').logger;
var config          = require('./src/libs').config;
var IA              = require('./src//IA/IA');

Promise.promisifyAll(readline);
Promise.promisifyAll(serialPort);

//Init logger
logger.initIO(controlPannel);
var log = logger.getLogger(module);


var launch = serialPort.listAsync()
    .then(function selectSerialPort(ports) {
        return new Promise(function(resolve, reject) {
            if (ports.length === 0) {
                return reject('No open ports.');
            }
            
            if (ports.length === 1) {
                var motorController = new MotorController(ports[0].comName, 115200, function() {
                    resolve(motorController);
                });

                return;
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
                    var motorController = new MotorController(ports[parseInt(answer)].comName, 115200, function() {
                        resolve(motorController);
                    });
                })
                .catch(reject);
        });
    }) 
    .catch(function(error) {
        log.error(error);
    })
    .then(function(motorController) {
        var IA_ = new IA(motorController);
        IA_.start();
    });
