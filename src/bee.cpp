//
// Created by alban on 02/04/2021.
//

#include "bee.h"
#include "endpoint.h"
#include "callback.h"
#include <HiveEntropyAPI/full.h>

//-----------------------------------------------------------------------------------

Bee::Bee(Endpoint *endpoint, Processor *processor) : endpoint(endpoint), processor(processor) {}

Bee::~Bee() {

}

void Bee::run() {
    // Ajoute les callbacks pour les urls existantes
    endpoint->addMessageCallback("health", HttpMethod::GET, Callback::health);
    endpoint->addMessageCallback("require-help", HttpMethod::GET, Callback::requireHelp);
    endpoint->addMessageCallback("task/multiplication/cannon", HttpMethod::POST, Callback::cannonMultiplication);
    endpoint->addMessageCallback("task/multiplication/rowcol", HttpMethod::GET, Callback::rowColMultiplication);
}


