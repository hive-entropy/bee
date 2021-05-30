//
// Created by alban on 02/04/2021.
//

#ifndef PDS_BEE_ENDPOINT_H
#define PDS_BEE_ENDPOINT_H

#include <HiveEntropyAPI/full.h>

/**
 * An endpoint to communicate with other workers.
 * The listen method is passed a function that will serve as a callback. When a message is received, it is passed as an argument to the callback function.
 * Messages can be sent by the endpoint to answer to a request.
 */
class Endpoint {
private:
    HiveEntropyNode *node;

public:
    /**
     * Endpoint constructor.
     * @param nodeInterface
     */
    Endpoint(HiveEntropyNode *node);

    /**
     * Endpoint destructor.
     */
    virtual ~Endpoint();

    template<Message(*F)(Message)>
    void addMessageCallback(std::string url, HttpMethod httpMethod);

    template<Message(*F)(Message)>
    void addAsynchronousMessageCallback(std::string url, HttpMethod httpMethod);
};

template<Message(*F)(Message)>
void Endpoint::addMessageCallback(std::string url, HttpMethod httpMethod) {
    node->registerMessageHandler<F>(url, httpMethod);
}

template<Message(*F)(Message)>
void Endpoint::addAsynchronousMessageCallback(std::string url, HttpMethod httpMethod) {
    node->registerAsynchronousHandler<F>(url, httpMethod);
}


#endif //PDS_BEE_ENDPOINT_H
