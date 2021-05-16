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
    endpoint->addMessageCallback<Callback::health>("health", HttpMethod::GET);
    endpoint->addMessageCallback<Callback::requireHelp>("require-help", HttpMethod::GET);
    endpoint->addMessageCallback<Callback::cannonMultiplication>("task/multiplication/cannon", HttpMethod::POST);
    endpoint->addMessageCallback<Callback::rowColMultiplication>("task/multiplication/rowcol", HttpMethod::POST);
}


