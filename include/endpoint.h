//
// Created by alban on 02/04/2021.
//

#ifndef PDS_BEE_ENDPOINT_H
#define PDS_BEE_ENDPOINT_H

#include "HiveEntropyAPI/HiveEntropyNodeInterface.h"

class Message;
class ResponseBuilder;
class HiveEntropyNodeInterface;

/**
 * An endpoint to communicate with other workers.
 * The listen method is passed a function that will serve as a callback. When a message is received, it is passed as an argument to the callback function.
 * Messages can be sent by the endpoint to answer to a request.
 */
class Endpoint {
private:
    HiveEntropyNodeInterface *nodeInterface;
    ResponseBuilder *responseBuilder;

public:
    /**
     * Endpoint constructor.
     * @param nodeInterface
     * @param responseBuilder
     */
    Endpoint(HiveEntropyNodeInterface *nodeInterface, ResponseBuilder *responseBuilder);

    /**
     * Endpoint destructor.
     */
    virtual ~Endpoint();

    /**
     * Register the given function as a message handler.
     * @param callbackFunction Function that will be called when a message is received.
     */
    void addMessageCallback(std::string url, HttpMethod httpMethod, coap_method_handler_t callbackFunction);
};


#endif //PDS_BEE_ENDPOINT_H
