//
// Created by alban on 02/04/2021.
//

#include "endpoint.h"

Endpoint::Endpoint(HiveEntropyNodeInterface *nodeInterface, ResponseBuilder *responseBuilder) : nodeInterface(
        nodeInterface), responseBuilder(responseBuilder) {}

Endpoint::~Endpoint() {

}
