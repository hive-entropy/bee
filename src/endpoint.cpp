//
// Created by alban on 02/04/2021.
//

#include "HiveEntropyAPI/HiveEntropyNodeInterface.h"
#include "HiveEntropyAPI/HiveEntropyNode.h"
#include "HiveEntropyAPI/ResponseBuilder.h"
#include "HiveEntropyAPI/GlobalContext.h"
#include "HiveEntropyAPI/Message.h"

#include "endpoint.h"

Endpoint::Endpoint(HiveEntropyNodeInterface *nodeInterface) : nodeInterface(
        nodeInterface) {}

Endpoint::~Endpoint() {}

void Endpoint::addMessageCallback(std::string url, HttpMethod httpMethod, coap_method_handler_t callbackFunction) {
    (*nodeInterface).registerMessageHandler(url, httpMethod, callbackFunction);
}
