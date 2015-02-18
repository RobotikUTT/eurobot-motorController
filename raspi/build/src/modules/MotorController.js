"use strict";

var _interopRequire = function (obj) { return obj && obj.__esModule ? obj["default"] : obj; };

var _prototypeProperties = function (child, staticProps, instanceProps) { if (staticProps) Object.defineProperties(child, staticProps); if (instanceProps) Object.defineProperties(child.prototype, instanceProps); };

var _classCallCheck = function (instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } };

var packets = _interopRequire(require("../communication/packets/index"));

//import Parser from '../communcation/PacketParser'

var i2c = require("i2c-bus");


/**
 * @brief Motor controller interface
 * @details Control two motors by I2C
 */

var MotorController = (function () {
  /**
   * @brief Constructor
   * 
   * @param  address I2C address
   */

  function MotorController(address) {
    _classCallCheck(this, MotorController);

    this.address = address;
    //this.parser = new PacketParser();
  }

  _prototypeProperties(MotorController, null, {
    sendPacket: {


      /**
       * @brief Send a packet to the module
       * 
       * @param  packet Packet object
       */

      value: function sendPacket(packet) {},
      writable: true,
      configurable: true
    },
    ping: {


      /**
       * @brief Ping the module to test communication
       * @details Use a TestPacket. If the module responds correctly,
       * the response TestPacket will have a (number+1) number.
       */

      value: function ping(number) {
        this.sendPacket(new packets.TestPacket(number));
      },
      writable: true,
      configurable: true
    }
  });

  return MotorController;
})();

module.exports = MotorController;
//# sourceMappingURL=../modules/MotorController.js.map