"use strict";

var _prototypeProperties = function (child, staticProps, instanceProps) { if (staticProps) Object.defineProperties(child, staticProps); if (instanceProps) Object.defineProperties(child.prototype, instanceProps); };

var _classCallCheck = function (instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } };

var Scheduler = require("node-robot").Scheduler;


var IA = (function () {
  /**
   * @brief Constructor
   * 
   * @param motorController Motor interface
   */

  function IA(motorController) {
    _classCallCheck(this, IA);

    this.motorController = motorController;
    this.scheduler = new Scheduler();
  }

  _prototypeProperties(IA, null, {
    start: {


      /**
       * @brief Start IA
       */

      value: function start() {},
      writable: true,
      configurable: true
    },
    stop: {

      /**
       * @brief Stop IA
       */
      value: function stop() {},
      writable: true,
      configurable: true
    }
  });

  return IA;
})();

module.exports = IA;
//# sourceMappingURL=../IA/IA.js.map