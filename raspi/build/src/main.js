"use strict";

var _interopRequireWildcard = function (obj) { return obj && obj.__esModule ? obj : { "default": obj }; };

var _interopRequire = function (obj) { return obj && obj.__esModule ? obj["default"] : obj; };

var MotorController = _interopRequire(require("./modules/MotorController"));

var controlPannel = _interopRequireWildcard(require("./controlPannel/server"));




controlPannel.start();
var motorController = new MotorController();
//# sourceMappingURL=main.js.map