import Packet from './Packet';


const PACKET_NUMBER = 0;
const PACKET_LENGTH = 1;


/**
 * @brief Packet used for testing communication between 
 * this software and an other module
 */

class TestPacket extends Packet {

    /**
     * @brief Constructor
     * 
     * @param number Number that will be incremented in return by the other module
     */
    
    constructor(number = 0) {
        super();
        this.number = number;
    }

    static getPacketNumber() {
        return PACKET_NUMBER;
    }


    static getPacketLength() {
        return PACKET_LENGTH;
    }


    /**
     * @brief number getter
     */

    getNumber() {
        return this.number;
    }


    /**
     * @brief number setter
     */

    setNumber(number) {
        this.number = number;
    }
}


export default TestPacket;