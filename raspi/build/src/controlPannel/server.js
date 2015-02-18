"use strict";

var _interopRequireWildcard = function (obj) { return obj && obj.__esModule ? obj : { "default": obj }; };




/**
 * @brief Start the webServer
 */

exports.start = start;
var logger = _interopRequireWildcard(require("../libs/logger"));

var path = require("path");
var express = require("express");
var app = express().use(express["static"](path.join(__dirname, "./")));
var server = require("http").Server(app);
var io = require("socket.io")(server);
var util = require("util");


/**
 * Socket io is set up, we can pass the object
 * to the logger module
 */

logger.initIO(io);
var log = logger.getLogger(module);

var port = 8080;

io.on("connection", function (socket) {
    log.info("[IO] Connected");

    socket.on("disconnect", function () {
        log.info("[IO] Disconnected");
    });
});


app.get("/", function (req, res) {
    res.sendFile(__dirname + "/public/index.html");
});

app.get("/style.css", function (req, res) {
    res.sendFile(__dirname + "/public/style.css");
});

app.get("/script.js", function (req, res) {
    res.sendFile(__dirname + "/public/script.js");
});function start() {
    server.listen(port);
    log.info("[WEB] Server listening on *:" + port);
}


exports["default"] = io;
Object.defineProperty(exports, "__esModule", {
    value: true
});
//# sourceMappingURL=../controlPannel/server.js.map