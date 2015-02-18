import packets from '../communication/packets/index'
//import Parser from '../communcation/PacketParser'

let i2c = require('i2c-bus');


/**
 * @brief Motor controller interface
 * @details Control two motors by I2C
 */

class MotorController {

    /**
     * @brief Constructor
     * 
     * @param  address I2C address
     */
    
    constructor(address) {
        this.address = address;
        //this.parser = new PacketParser();
    } 


    /**
     * @brief Send a packet to the module
     * 
     * @param  packet Packet object
     */
    
    sendPacket(packet) {

    }


    /**
     * @brief Ping the module to test communication
     * @details Use a TestPacket. If the module responds correctly,
     * the response TestPacket will have a (number+1) number.
     */
    
    ping(number) {
        this.sendPacket(new packets.TestPacket(number));
    }
}

export default MotorController;