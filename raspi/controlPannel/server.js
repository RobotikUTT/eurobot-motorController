var path    = require('path');
var express = require('express');
var winston = require('winston');
var app     = express().use(express.static(path.join(__dirname, './')));
var server  = require('http').Server(app);
var io      = require('socket.io')(server);
var log     = require('../libs').logger.getLogger(module);
var config  = require('../libs').config;
var util    = require('util');


/*
    Setup socket.io
 */

io.on('connection', function(socket) {
    log.info('[IO] Connected');

    socket.on('disconnect', function() {
        log.info('[IO] Disconnected');
    });
});


app.get('/', function(req, res) {
    res.sendFile(__dirname + '/public/index.html');
});

app.get('/style.css', function(req, res) {
    res.sendFile(__dirname + '/public/style.css');
});

app.get('/script.js', function(req, res) {
    res.sendFile(__dirname + '/public/script.js');
});

var port = config.get('pannel').port;
server.listen(port);

log.info('[WEB] Server listening on *:' + port);


module.exports = io;