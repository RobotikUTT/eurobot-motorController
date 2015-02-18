"use strict";

var _interopRequire = function (obj) { return obj && obj.__esModule ? obj["default"] : obj; };

var _prototypeProperties = function (child, staticProps, instanceProps) { if (staticProps) Object.defineProperties(child, staticProps); if (instanceProps) Object.defineProperties(child.prototype, instanceProps); };

var _get = function get(object, property, receiver) { var desc = Object.getOwnPropertyDescriptor(object, property); if (desc === undefined) { var parent = Object.getPrototypeOf(object); if (parent === null) { return undefined; } else { return get(parent, property, receiver); } } else if ("value" in desc && desc.writable) { return desc.value; } else { var getter = desc.get; if (getter === undefined) { return undefined; } return getter.call(receiver); } };

var _inherits = function (subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) subClass.__proto__ = superClass; };

var _classCallCheck = function (instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } };

var Packet = _interopRequire(require("./Packet"));




var PACKET_NUMBER = 0;
var PACKET_LENGTH = 1;


/**
 * @brief Packet used for testing communication between 
 * this software and an other module
 */

var TestPacket = (function (Packet) {
    /**
     * @brief Constructor
     * 
     * @param number Number that will be incremented in return by the other module
     */

    function TestPacket() {
        var number = arguments[0] === undefined ? 0 : arguments[0];
        _classCallCheck(this, TestPacket);

        _get(Object.getPrototypeOf(TestPacket.prototype), "constructor", this).call(this);
        this.number = number;
    }

    _inherits(TestPacket, Packet);

    _prototypeProperties(TestPacket, {
        getPacketNumber: {
            value: function getPacketNumber() {
                return PACKET_NUMBER;
            },
            writable: true,
            configurable: true
        },
        getPacketLength: {
            value: function getPacketLength() {
                return PACKET_LENGTH;
            },
            writable: true,
            configurable: true
        }
    }, {
        getNumber: {


            /**
             * @brief number getter
             */

            value: function getNumber() {
                return this.number;
            },
            writable: true,
            configurable: true
        },
        setNumber: {


            /**
             * @brief number setter
             */

            value: function setNumber(number) {
                this.number = number;
            },
            writable: true,
            configurable: true
        }
    });

    return TestPacket;
})(Packet);

module.exports = TestPacket;
//# sourceMappingURL=../../communication/packets/TestPacket.js.map