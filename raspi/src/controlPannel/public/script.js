'use strict';

var socket = io();


socket.on('log', function(data) {
    var div = document.getElementById('debug');


    var toAdd = '';
    toAdd += '<p> <span class="' + data.level + '">' + data.level + '</span>: ';
    toAdd += (data.msg) ? data.msg : '';
    toAdd += (data.meta) ? JSON.stringify(data.meta) : '';
    toAdd += '</p>';

    div.innerHTML += toAdd;
});