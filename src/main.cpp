#include <iostream>
#include<unistd.h>
#include <list>
#include "bee.h"
#include "endpoint.h"
#include "processor.h"
#include <HiveEntropyAPI/full.h>



int main() {
    /*
    float a[] = {1,2,3,4,5,6,7,8,9,
                        1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,
                        9,2,7,4,5,6,3,8,1};
    Matrix<float> A(9,9,a);
    Matrix<float> B(9,9,MatrixArchetype::ONES);
    Matrix<float> C(9,9,MatrixArchetype::ZEROS);
*/
    // Dependency injection
    HiveEntropyNode hiveEntropyNode("127.0.0.1:9999");

    Endpoint endpoint(&hiveEntropyNode);
    Processor processor;

    Bee bee(&endpoint, &processor);

    // Start the worker
    std::cout << "Starting worker" << std::endl;
    bee.run();
    std::cout << "Worker ready" << std::endl;
/*
    hiveEntropyNode.queryNodeAvailability();
    
    std::string id="potato";
    GlobalContext<Matrix<float>>::registerObject(id, C);

    cout << "Sending..." << endl;
    MessageHandler::divideCalculInTasks(&hiveEntropyNode, A, B, id);

    cout << "result from nodes " << endl; 
    Matrix<float>* total = GlobalContext<Matrix<float>>::get("result"+id);
    total->show();
    cout << "result " << endl;
    C = A*B;
    C.show();
    cout << "end" << endl;
    */
    hiveEntropyNode.keepAlive();

    return 0;
}