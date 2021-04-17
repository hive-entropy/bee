//
// Created by alban on 02/04/2021.
//

#ifndef PDS_BEE_BEE_H
#define PDS_BEE_BEE_H

class Message;
class Endpoint;
class Processor;

/**
 * Main class of the worker. This class coordinates the different actions the worker must do.
 * A worker must listen for incoming messages, process the corresponding requests - either it is a computational or an informational request - and answer to the original sender.
 *
 * Act as a router. Take messages as input and, depending on the request received, call the appropriate methods.
 * If the request is of computation type: extracts data and sends it to the processor. Once the computation is done, the result is sent back to the endpoint.
 * If the request is of informational type:
 */
class Bee {
private:
    Endpoint *endpoint;
    Processor *processor;
public:
    /**
     * Class constructor.
     * @param endpoint
     * @param processor
     */
    Bee(Endpoint *endpoint, Processor *processor);

    /**
     * Class destructor.
     */
    virtual ~Bee();

    /**
     * Start the worker.
     * Call the listener to add a message handler.
     */
    void run();

    /**
     * Callback function passed to the endpoint. Take decision based on request type.
     * This function archetype must match the callback defined in the API: registerMessageHandler(string key, Message  (*func)(Message message))
     * @param message The message that have been received.
     */
    void processRequest(Message message);
};


#endif //PDS_BEE_BEE_H
