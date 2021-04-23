//
// Created by alban on 02/04/2021.
//

#include <HiveEntropyAPI/full.h>
#include "endpoint.h"

Endpoint::Endpoint(HiveEntropyNode *nodeInterface) : nodeInterface(
        nodeInterface) {}

Endpoint::~Endpoint() {}

void Endpoint::addMessageCallback(std::string url, HttpMethod httpMethod, coap_method_handler_t callbackFunction) {
    (*nodeInterface).registerMessageHandler(url, httpMethod, callbackFunction);
}
