//
// Created by alban on 02/04/2021.
//

#include "bee.h"
#include "endpoint.h"
#include "HiveEntropyAPI/Message.h"
#include "HiveEntropyAPI/ResponseBuilder.h"
#include "HiveEntropyAPI/GlobalContext.h"

Bee::Bee(Endpoint *endpoint, Processor *processor) : endpoint(endpoint), processor(processor) {}

Bee::~Bee() {

}

void Bee::run() {
    // Ajoute les callbacks pour les urls existantes
    // URL : /health
    endpoint->addMessageCallback("/health", HttpMethod::GET, &healthCallback);
    // URL : /task/multiplication/cannon
    endpoint->addMessageCallback("/task/multiplication/cannon", HttpMethod::GET, &cannonMulCallback);
    // URL : /task/multiplication/rowcol
}

void Bee::healthCallback(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                          coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                          coap_pdu_t *response) {

}


template<typename T>
void templatedCannon(coap_session_t *session, Message &inputMessage);
void Bee::cannonMulCallback(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                          coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                          coap_pdu_t *response) {
    // Extract the inputMessage
    Message inputMessage(session, request);

    // Get the matrix type
    std::basic_string<char> matrixType = inputMessage.getHeaders()[Headers::ELEMENT_TYPE];

    // Call the template method with the corresponding type
    if (matrixType == typeid(int).name()) {
        templatedCannon<int>(session, inputMessage);
    } else if (matrixType == typeid(double).name()) {
        templatedCannon<double>(session, inputMessage);
    } else if (matrixType == typeid(float).name()) {
        templatedCannon<float>(session, inputMessage);
    }
}

template<typename T>
void templatedCannon(coap_session_t *session, Message &inputMessage) {
    coap_pdu_t *response;// Extract the necessary informations
    basic_string<char> calculationId = inputMessage.getHeaders()[Headers::CALCULATION_ID];
    basic_string<char> taskId = inputMessage.getHeaders()[Headers::TASK_ID];
    int startRow = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_X]);
    int startCol = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_Y]);
    int steps = stoi(inputMessage.getHeaders()[Headers::STEPS]);

    string globalId = calculationId + taskId;

    // Deserialize the matrices
    vector<Matrix<T>> matrices = Serializer::unserializeMatrices<T>(inputMessage.getContent());
    // Multiply the matrices
    Matrix<T> result = matrices[0] * matrices[1];

    // Check the actual step of the computation
    int *stepCounter;
    if ((stepCounter = GlobalContext<int>::get(globalId)) != nullptr) {
        // The computation is going-on
        // Add the result to the stored matrix
        Matrix<T> *storedMatrix = GlobalContext<Matrix<T>>::get(globalId);
        (*storedMatrix) += result;
        // Increment the actual step
        (*stepCounter)++;
    } else {
        // The computation starts
        // Store the result matrix
        GlobalContext<Matrix<T>>::registerObject(globalId, result); // TODO: Check if it creates a copy
        // Create and store the step counter
        GlobalContext<int>::registerObject(globalId, 1);
    }

    Message outputMessage;
    // Check if the computation is finished
    if (stepCounter != nullptr && steps == (*stepCounter)) {
        // Send the result
        outputMessage = ResponseBuilder::matrixMultiplicationResultFragmentMessage(calculationId, taskId, startRow,startCol, result);

        // Empty the GlobalContext
        GlobalContext<int>::unregisterObject(globalId);
        GlobalContext<Matrix<T>>::unregisterObject(globalId);
    } else
    {
        // Send a acknowledgement message
        outputMessage = ResponseBuilder::heartbeatMessage();
    }

    // Send the response
    response = outputMessage.toCoapMessage(session);
}
