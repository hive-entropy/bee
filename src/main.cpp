#include <iostream>
#include<unistd.h>
#include <list>
#include "bee.h"
#include "endpoint.h"
#include "processor.h"
#include <HiveEntropyAPI/full.h>

#define SEC 5

void waitXsec(int x){
    int sec = 0, time;
    const auto p1 = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
    sec = time;
    while(sec<time+x){
        //sleep(1);
        sec++;

    }
        cout << x << "second waited" << endl;
 

}





int main(int argc, char* argv[]) {
    // Dependency injection
    HiveEntropyNode *hiveEntropyNode;
    if(argc>=2){
        cout << "Found IP address "+std::string(argv[1])+" to use" << endl;
        hiveEntropyNode = new  HiveEntropyNode(std::string(argv[1])+":9999");
    }
    else
         hiveEntropyNode = new  HiveEntropyNode("127.0.0.1:9999");

    Endpoint endpoint(hiveEntropyNode);
    Endpoint endpoint2(hiveEntropyNode);
    Processor processor;
    Processor processor2;

    Bee bee(&endpoint, &processor);
    Bee bee2(&endpoint2, &processor2);

    // Start the worker
    std::cout << "Starting worker" << std::endl;
    bee.run();
    bee2.run();
    std::cout << "Worker ready" << std::endl;
    hiveEntropyNode->keepAlive();

    return 0;
}