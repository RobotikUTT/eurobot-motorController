let Scheduler = require('node-robot').Scheduler;


class IA {
    /**
     * @brief Constructor
     * 
     * @param motorController Motor interface
     */
    
    constructor(motorController) {
        this.motorController = motorController;
        this.scheduler = new Scheduler();
    }


    /**
     * @brief Start IA
     */

    start() {

    }

    /**
     * @brief Stop IA
     */
    stop() {

    }
}


export default IA;