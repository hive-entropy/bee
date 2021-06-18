#include <iostream>
#include<unistd.h>
#include <list>
#include "bee.h"
#include "endpoint.h"
#include "processor.h"

#include <HiveEntropyAPI/full.h>
#include <spdlog/sinks/stdout_sinks.h>

static auto logger = spdlog::stdout_logger_mt("logger");


int main(int argc, char* argv[]) {

    spdlog::set_default_logger(logger);

    //Setting log message pattern
    logger.get()->set_pattern("[%H:%M:%S.%e] [%!] (%l) %v");
    logger.get()->set_level(spdlog::level::debug);

    // Dependency injection
    HiveEntropyNode *hiveEntropyNode;
    if(argc>=2){
        logger.get()->info("Found IP address {} to use",argv[1]);
        hiveEntropyNode = new  HiveEntropyNode(std::string(argv[1])+":9999");
    }
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