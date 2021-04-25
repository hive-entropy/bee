#include <iostream>
#include "bee.h"
#include "endpoint.h"
#include "processor.h"
#include <HiveEntropyAPI/full.h>


int main() {
    // Dependency injection
    HiveEntropyNode hiveEntropyNode("127.0.0.1:9999");

    Endpoint endpoint(&hiveEntropyNode);
    Processor processor;

    Bee bee(&endpoint, &processor);

    // Start the worker
    std::cout << "Starting worker" << std::endl;
    bee.run();
    std::cout << "Worker ready" << std::endl;

    hiveEntropyNode.keepAlive();

    return 0;
}
