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
    endpoint->addAsynchronousMessageCallback<Callback::cannonMultiplication>("task/multiplication/cannon", HttpMethod::POST);
    endpoint->addAsynchronousMessageCallback<Callback::rowColMultiplication>("task/multiplication/rowcol", HttpMethod::POST);
    endpoint->addMessageCallback<Callback::latency>("latency",HttpMethod::POST);
    endpoint->addMessageCallback<Callback::hardware>("hardware",HttpMethod::POST);
}


