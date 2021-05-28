//
// Created by alban on 02/04/2021.
//

#include <HiveEntropyAPI/full.h>
#include "endpoint.h"

Endpoint::Endpoint(HiveEntropyNode *node) : node(
        node) {}

Endpoint::~Endpoint() {}

void Endpoint::addMessageCallback(std::string url, HttpMethod httpMethod, coap_method_handler_t callbackFunction) {
    (*node).registerMessageHandler(url, httpMethod, callbackFunction);
}
