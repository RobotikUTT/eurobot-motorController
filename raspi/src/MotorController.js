class MotorController {
    constructor(address, baudrate) {
        this.address = address;
        this.baudrate = baudrate;
        this.parser = null;
    } 
    sendPacket(packet) {
        console.log(packet);
    }

    ping() {

    }
}

export default MotorController;