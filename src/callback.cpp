//
// Created by alban on 24/04/2021.
//

#include <HiveEntropyAPI/full.h>
#include "endpoint.h"
#include "bee.h"
#include "callback.h"

//Forward declaration
template<typename T>
Message templatedCannon(Message &inputMessage);

template<typename T>
Message templatedRowCol(Message &inputMessage);

Message Callback::health(Message message) {
    return ResponseBuilder::heartbeatMessage();
}

Message Callback::hardware(Message message) {
    return ResponseBuilder::hardwareMessage();
}

Message Callback::requireHelp(Message message) {
    return ResponseBuilder::assistanceResponseMessage(true);
}

Message Callback::rowColMultiplication(Message message) {
    // Get the matrix type
    string matrixType = message.getHeaders()[Headers::ELEMENT_TYPE];

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

    // Call the template method with the corresponding type
    if (matrixType == typeid(int).name()) {
        return templatedCannon<int>(message);
    } else if (matrixType == typeid(double).name()) {
        return templatedCannon<double>(message);
    } else if (matrixType == typeid(float).name()) {
        return templatedCannon<float>(message);
    }
}

Message Callback::latency(Message message){
    cout << "[received] "+message.getContent() << endl;
    return ResponseBuilder::heartbeatMessage();
}


// ======================
// Write once, use everywhere
// ======================
//                                          -Java
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

    // Deserialize the row and column
    std::pair<Row<T>, Column<T>> rowCol = Serializer::unserializeRowColumn<T>(inputMessage.getContent());
    // Multiply the row and column
    T result = rowCol.first * rowCol.second;

    // Add the result to the output message
    return ResponseBuilder::matrixMultiplicationResultFragmentMessage(calculationId, startRow, startCol, result);
}