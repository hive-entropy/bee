//
// Created by alban on 24/04/2021.
//

#include <HiveEntropyAPI/full.h>
#include <spdlog/spdlog.h>

#include "endpoint.h"
#include "bee.h"
#include "callback.h"

bool Callback::paused = false;

//Forward declaration
template<typename T>
Message templatedCannon(Message &inputMessage);

template<typename T>
Message templatedRowCol(Message &inputMessage);

template<typename T>
Message templatedConvolution(Message &input);

Message Callback::health(Message message) {
    spdlog::info("Received: Healthcheck");
    spdlog::info("Sent: Heartbeat");
    return ResponseBuilder::heartbeatMessage();
}

Message Callback::hardware(Message message) {
    spdlog::info("Received: Hardware Query");
    spdlog::info("Sent: Hardware characteristics response");
    return ResponseBuilder::hardwareMessage();
}

Message Callback::requireHelp(Message message) {
    spdlog::info("Received: Assistance Query");

    //TODO Determine response (if busy and not taking too many resources)
    bool response = true;


    spdlog::info("Sent: Assistance Response ({})",response);
    return ResponseBuilder::assistanceResponseMessage(response);
}

Message Callback::rowColMultiplication(Message message){
    // Get the matrix type
    std::string matrixType = message.getHeaders()[Headers::ELEMENT_TYPE];
    spdlog::info("Received: Row-Column multiplication task for type {}",matrixType);

    // Call the template method with the corresponding type
    if (matrixType == typeid(int).name()) {
        return templatedRowCol<int>(message);
    } else if (matrixType == typeid(double).name()) {
        return templatedRowCol<double>(message);
    } else if (matrixType == typeid(float).name()) {
        return templatedRowCol<float>(message);
    }
}

Message Callback::cannonMultiplication(Message message) {
    // Get the matrix type
    string matrixType = message.getHeaders()[Headers::ELEMENT_TYPE];
    spdlog::info("Received: Cannon multiplication task for type {}",matrixType);
    // Call the template method with the corresponding type
    if (matrixType == typeid(int).name()) {
        return templatedCannon<int>(message);
    } else if (matrixType == typeid(double).name()) {
        return templatedCannon<double>(message);
    } else if (matrixType == typeid(float).name()) {
        return templatedCannon<float>(message);
    }
}

Message Callback::identity(Message message){
    spdlog::info("Received: State Query");
    Message output;

    //TODO Determine if busy

    bool busy = false;

    output.setType(MessageType::ACK);
    output.setHttpMethod(HttpMethod::OK);
    output.addHeader(Headers::PURPOSE,"identity");
    output.setContent((busy) ? "busy" : (paused) ? "paused" : "idle");
    spdlog::info("Send State response ({})",output.getContent());
    return output;
}

Message Callback::stop(Message message){
    spdlog::info("Received: Stop signal from network");
    exit(0);
}

Message Callback::pause(Message message){
    spdlog::info("Received: Pause signal from network");
    spdlog::info("Pausing Worker");
    paused = true;
}

Message Callback::resume(Message message){
    spdlog::info("Received: Resume signal from network");
    spdlog::info("Resuming Worker");
    paused = false;
}

Message Callback::logs(Message message){
    spdlog::info("Received: Logs Query");

    //TODO Output logs to string and send
}

Message Callback::restart(Message input){
    spdlog::info("Received: Restart Signal from network");
    exit(99);
}

Message Callback::returnToSender(Message message){
    spdlog::info("Received: Boomerang message");
    Message output;
    output.setContent(message.getContent());
    output.setType(MessageType::ACK);
    output.setHttpMethod(HttpMethod::OK);
    output.addHeader(Headers::PURPOSE,"around");
    spdlog::info("Sending: Message with same input");
    return output;
}

Message Callback::convolution(Message message){
     // Get the matrix type
    string matrixType = message.getHeaders()[Headers::ELEMENT_TYPE];

    spdlog::info("Received: Convolution task for type {}",matrixType);
    // Call the template method with the corresponding type
    if (matrixType == typeid(int).name()) {
        return templatedConvolution<int>(message);
    } 
    else if (matrixType == typeid(double).name()) {
        return templatedConvolution<double>(message);
    } 
    else if (matrixType == typeid(float).name()) {
        return templatedConvolution<float>(message);
    }
    else if(matrixType==typeid(unsigned short).name()){
        return templatedConvolution<unsigned short>(message);
    }
}


// ======================
// Write once, use everywhere
// ======================
//                                          -Java

//TODO Add logs over here
template<typename T>
Message templatedCannon(Message &inputMessage) {
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
    int stepCounter;
    try {
        stepCounter = GlobalContext<int>::get(globalId);
    }
    catch (exception &e) {
        stepCounter = 0;
    }

    if (stepCounter == 0) {
        // The computation starts
        // Store the result matrix
        GlobalContext<Matrix<T>>::registerObject(globalId, result);
        // Create and store the step counter
        GlobalContext<int>::registerObject(globalId, 1);
        stepCounter = GlobalContext<int>::get(globalId);
    } else {
        // The computation is going-on
        // Add the result to the stored matrix
        GlobalContext<Matrix<T>>::get(globalId) += result;
        // Increment the actual step
        stepCounter++;
    }

    // Check if the computation is finished
    if (steps == stepCounter){
        // Empty the GlobalContext
        GlobalContext<int>::unregisterObject(globalId);
        GlobalContext<Matrix<T>>::unregisterObject(globalId);

        // Send the result
        return ResponseBuilder::matrixMultiplicationResultFragmentMessage(calculationId, taskId, startRow,startCol, result);
    }
    else{
        // Send a acknowledgement message
        return ResponseBuilder::heartbeatMessage();
    }
}

template<typename T>
Message templatedRowCol(Message &inputMessage) {
    // Extract the necessary information
    string calculationId = inputMessage.getHeaders()[Headers::CALCULATION_ID];
    int startRow = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_X]);
    int startCol = stoi(inputMessage.getHeaders()[Headers::INSERT_AT_Y]);
    string serializedType = inputMessage.getHeaders()[Headers::SERIALIZED_TYPE];

    spdlog::info("Extracted insertion point ({},{}) for calculation UID={}",startRow,startCol,calculationId);

    if(serializedType==SERIALIZED_TYPE_ROWCOL){
        spdlog::info("Using Single Row-Column pair mode");
        // Deserialize the row and column
        std::pair<Row<T>, Column<T>> rowCol = Serializer::unserializeRowColumn<T>(inputMessage.getContent());
        // Multiply the row and column
        T result = rowCol.first * rowCol.second;

        spdlog::info("Sending: Multiplication Result (single element)");
        // Add the result to the output message
        return ResponseBuilder::matrixMultiplicationResultFragmentMessage(calculationId, startRow, startCol, result);
    }
    else if(serializedType==SERIALIZED_TYPE_MATRICES){
        spdlog::info("Using Multiple Row-Column bands mode");
        // Deserialize the row and column
        std::vector<Matrix<T>> matrices = Serializer::unserializeMatrices<T>(inputMessage.getContent());

        // Multiply the row and column
        Matrix<T> result = matrices[0] * matrices[1];
        
        spdlog::info("Sending: Multiplication Result (submatrix block)");
        // Add the result to the output message
        return ResponseBuilder::matrixMultiplicationResultFragmentMessage(calculationId, startRow, startCol, result);
    }

}

template<typename T>
Message templatedConvolution(Message &input){
    string calculationId = input.getHeaders()[Headers::CALCULATION_ID];
    int startRow = stoi(input.getHeaders()[Headers::INSERT_AT_X]);
    int startCol = stoi(input.getHeaders()[Headers::INSERT_AT_Y]);

    spdlog::info("Extracted insertion point ({},{}) for calculation UID={}",startRow,startCol,calculationId);

    std::vector<Matrix<T>> matrices = Serializer::unserializeMatrices<T>(input.getContent());
    spdlog::debug("Extracted operand matrices");
    Matrix<T> result = matrices[0].convolve(matrices[1],EdgeHandling::Crop);
    spdlog::debug("Calculated convolution result");

    spdlog::info("Sending: Convolution result");
    return ResponseBuilder::matrixConvolutionResultFragmentMessage(calculationId,startRow,startCol,result);
}