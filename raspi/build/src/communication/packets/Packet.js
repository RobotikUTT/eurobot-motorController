"use strict";

var _prototypeProperties = function (child, staticProps, instanceProps) { if (staticProps) Object.defineProperties(child, staticProps); if (instanceProps) Object.defineProperties(child.prototype, instanceProps); };

var _classCallCheck = function (instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } };

/**
 * @brief Communication packet used to talk with other modules
 * (such as the motor controller module)
 * @details Protocol used:
 *  TODO
 * @return [description]
 */

var Packet = (function () {
  /**
   * @brief Constructor
   */

  function Packet() {
    _classCallCheck(this, Packet);

    this.length = 0;
  }

  _prototypeProperties(Packet, {
    getPacketNumber: {


      /**
       * @brief Get packet unique identifier
       */

      value: function getPacketNumber() {
        throw new Error("Virtual method");
      },
      writable: true,
      configurable: true
    },
    getPacketLength: {


      /**
       * @brief Return sum of argument length in bytes
       */

      value: function getPacketLength() {
        throw new Error("Virtual method");
      },
      writable: true,
      configurable: true
    }
  });

  return Packet;
})();

module.exports = Packet;
//# sourceMappingURL=../../communication/packets/Packet.js.map