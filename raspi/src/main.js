import MotorController from './modules/MotorController';
import * as controlPannel from './controlPannel/server';


controlPannel.start();
var motorController = new MotorController();