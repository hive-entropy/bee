//
// Created by alban on 02/04/2021.
//

#include <HiveEntropyAPI/full.h>
#include "endpoint.h"

Endpoint::Endpoint(shared_ptr<HiveEntropyNode> node) : node(
        node) {}

Endpoint::~Endpoint() {}