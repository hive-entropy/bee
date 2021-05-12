#include <iostream>
#include "bee.h"
#include "endpoint.h"
#include "processor.h"
#include <HiveEntropyAPI/full.h>


int main(int argc, char* argv[]) {

    // Dependency injection
    HiveEntropyNode *hiveEntropyNode;
    if(argc>=2)
        hiveEntropyNode = new  HiveEntropyNode(std::string(argv[1])+":9999");
    else
         hiveEntropyNode = new  HiveEntropyNode("127.0.0.1:9999");

    Endpoint endpoint(hiveEntropyNode);
    Processor processor;

    Bee bee(&endpoint, &processor);

    // Start the worker
    std::cout << "Starting worker" << std::endl;
    bee.run();
    std::cout << "Worker ready" << std::endl;

    hiveEntropyNode->keepAlive();

    return 0;
}
