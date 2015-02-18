/**
 * @brief Communication packet used to talk with other modules
 * (such as the motor controller module)
 * @details Protocol used:
 *  TODO
 * @return [description]
 */

class Packet {

    /**
     * @brief Constructor
     */

    constructor() {
        this.length = 0;
    }


    /**
     * @brief Get packet unique identifier
     */

    static getPacketNumber() {
        throw new Error("Virtual method");
    }


    /**
     * @brief Return sum of argument length in bytes
     */
    
    static getPacketLength() {
        throw new Error("Virtual method");
    }
}


export default Packet;