//
// Created by alban on 24/04/2021.
//

#include <HiveEntropyAPI/full.h>
#include "endpoint.h"
#include "bee.h"
#include "callback.h"

//Forward declaration
template<typename T>
void templatedCannon(coap_pdu_t* response, Message &inputMessage);

template<typename T>
void templatedRowCol(coap_pdu_t* response, Message &inputMessage);

void Callback::health(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                      coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                      coap_pdu_t *response){
    ResponseBuilder::heartbeatMessage().fillResponse(response);
}

void Callback::hardware(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                        coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                        coap_pdu_t *response) {
    ResponseBuilder::hardwareMessage().fillResponse(response);
}

void Callback::requireHelp(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                           coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                           coap_pdu_t *response) {
    ResponseBuilder::assistanceResponseMessage(true).fillResponse(response);
}

void Callback::rowColMultiplication(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                                    coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                                    coap_pdu_t *response) {
    // Extract the inputMessage
    Message inputMessage(session, request);

    // Get the matrix type
    string matrixType = inputMessage.getHeaders()[Headers::ELEMENT_TYPE];

    // Call the template method with the corresponding type
    if (matrixType == typeid(int).name()) {
        templatedRowCol<int>(response, inputMessage);
    } else if (matrixType == typeid(double).name()) {
        templatedRowCol<double>(response, inputMessage);
    } else if (matrixType == typeid(float).name()) {
        templatedRowCol<float>(response, inputMessage);
    }
}

void Callback::cannonMultiplication(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                                    coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                                    coap_pdu_t *response) {
    // Extract the inputMessage
    Message inputMessage(session, request);

    // Get the matrix type
    string matrixType = inputMessage.getHeaders()[Headers::ELEMENT_TYPE];

    // Call the template method with the corresponding type
    if (matrixType == typeid(int).name()) {
        templatedCannon<int>(response, inputMessage);
    } else if (matrixType == typeid(double).name()) {
        templatedCannon<double>(response, inputMessage);
    } else if (matrixType == typeid(float).name()) {
        templatedCannon<float>(response, inputMessage);
    }
}

// ======================
// Write once, use everywhere
// ======================
//                                          -Java
template<typename T>
void templatedCannon(coap_pdu_t* response, Message &inputMessage) {
    // Extract the necessary information
    string calculationId = inputMessage.getHeaders()[Headers::CALCULATION_ID];
    string taskId = inputMessage.getHeaders()[Headers::TASK_ID];
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
    }
    else{
        // The computation starts
        // Store the result matrix
        GlobalContext<Matrix<T>>::registerObject(globalId, result); // TODO: Check if it creates a copy
        // Create and store the step counter
        GlobalContext<int>::registerObject(globalId, 1);
    }

    Message outputMessage;
    // Check if the computation is finished
    if (stepCounter != nullptr && steps == (*stepCounter)){
        // Send the result
        outputMessage = ResponseBuilder::matrixMultiplicationResultFragmentMessage(calculationId, taskId, startRow,startCol, result);

        // Empty the GlobalContext
        GlobalContext<int>::unregisterObject(globalId);
        GlobalContext<Matrix<T>>::unregisterObject(globalId);
    }
    else{
        // Send a acknowledgement message
        outputMessage = ResponseBuilder::heartbeatMessage();
    }

    // Fill the response to be sent
    outputMessage.fillResponse(response);
}

template<typename T>
void templatedRowCol(coap_pdu_t* response, Message &inputMessage) {
    // Extract the necessary information
    string calculationId = inputMessage.getHeaders()[Headers::CALCULATION_ID];
    int startRow = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_X]);
    int startCol = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_Y]);

    // Deserialize the row and column
    std::pair<Row<T>, Column<T>> rowCol = Serializer::unserializeRowColumn<T>(inputMessage.getContent());
    // Multiply the row and column
    T result = rowCol.first * rowCol.second;

    Message outputMessage;
    // Add the result to the output message
    outputMessage = ResponseBuilder::matrixMultiplicationResultFragmentMessage(calculationId, startRow, startCol, result);

    // Fill the response to be sent
    outputMessage.fillResponse(response);
}