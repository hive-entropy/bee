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

void Bee::cannonMulCallback(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                          coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                          coap_pdu_t *response) {
    /*
     * Extract the inputMessage.
     * Get the headers : inputMessage.getHeaders()
     *
     * HEADERS: serializedTyped, calculationId, taskId, elementType, steps, inser_at_...,
     * BODY: list (vector) of matrix (two)
     *
     * Get the IDs
     * Deserialize the matrix (Serializer::unserializeMatrices(string from inputMessage))
     * Multiply the matrices (the first with the second)
     * If the IDs are new, store the result matrix of the size of the resulting block.
     * If IDs are not new, add result to stored matrix
     * Increment the step counter
     * If step_counter == max_step,
         * Then Send a heartbeat inputMessage
         * Else send matrixMultiplicationResultFragmentMessage with INSERT_AT_X and Y
     */
    // Extract the inputMessage
    Message inputMessage(session, request);

    // Get the necessary headers
    std::basic_string<char> calculationId = inputMessage.getHeaders()[Headers::CALCULATION_ID];
    std::basic_string<char> taskId = inputMessage.getHeaders()[Headers::TASK_ID];
    std::basic_string<char> matrixType = inputMessage.getHeaders()[Headers::ELEMENT_TYPE];
    int startRow = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_X]);
    int startCol = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_Y]);
    int steps = stoi(inputMessage.getHeaders()[Headers::STEPS]);

    std::string globalId = calculationId + taskId;

    if (matrixType == typeid(int).name()) {
        // Deserialize the matrices
        vector<Matrix<int>> matrices = Serializer::unserializeMatrices<int>(inputMessage.getContent());
        // Multiply the matrices
        Matrix<int> result = matrices[0] * matrices[1];

        // Check the actual step of the computation
        int *stepCounter;
        if ((stepCounter = GlobalContext<int>::get(globalId)) != nullptr) {
            // The computation is going-on
            // Add the result to the stored matrix
            Matrix<int> *storedMatrix = GlobalContext<Matrix<int>>::get(globalId);
            (*storedMatrix) += result;
            // Increment the actual step
            (*stepCounter)++;
        } else {
            // The computation starts
            // Store the result matrix
            GlobalContext<Matrix<int>>::registerObject(globalId, result); // TODO: Check if it creates a copy
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
            GlobalContext<Matrix<int>>::unregisterObject(globalId);
        } else
        {
            // Send a acknowledgement message
            outputMessage = ResponseBuilder::heartbeatMessage();
        }

        // Send the response
        response = outputMessage.toCoapMessage(session);

    } else if (matrixType == typeid(double).name()) {
        vector<Matrix<double>> matrices = Serializer::unserializeMatrices<double>(inputMessage.getContent());
        // TODO: Duplicate code here
    } else if (matrixType == typeid(char).name()) {
        vector<Matrix<char>> matrices = Serializer::unserializeMatrices<char>(inputMessage.getContent());
        // TODO: And here
    }
}