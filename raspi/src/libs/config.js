'use strict';

var nconf = require('nconf');


nconf
    .argv()
    .env()
    .file('./src/config.json');


module.exports = nconf;