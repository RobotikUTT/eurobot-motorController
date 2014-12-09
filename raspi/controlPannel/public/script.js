'use strict';

var socket = io();


socket.on('log', function(data) {
    var div = document.getElementById('debug');
    
    div.innerHTML += '<p class="' + data.level + '">' + data.msg + '</p>';
});