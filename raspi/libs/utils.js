'use strict';


module.exports.getModuleName = function(module) {
    return module.filename.split('/').slice(-2).join('/').split('.js')[0];
};