#include <iostream>
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




int main() {
    // Dependency injection
    HiveEntropyNode hiveEntropyNode("127.0.0.1:6969");
    HiveEntropyNode hiveEntropyNode2("127.0.0.1:9999");

    Endpoint endpoint(&hiveEntropyNode);
    Endpoint endpoint2(&hiveEntropyNode2);
    Processor processor;
    Processor processor2;

    Bee bee(&endpoint, &processor);
    Bee bee2(&endpoint2, &processor2);

    // Start the worker
    std::cout << "Starting worker" << std::endl;
    bee.run();
    bee2.run();
    std::cout << "Worker ready" << std::endl;

    //On crée les matrices
    float a[] = {1,2,3,4,5,6,7,8,9,
                1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,
                9,2,7,4,5,6,3,8,1};
    Matrix<float> A(9,9,a);
    Matrix<float> B(9,9,MatrixArchetype::ONES);
    Matrix<float> C(9,9,MatrixArchetype::ZEROS);

    /// Function
    std::string matrixName = "matrixResult";
    std::string collectionName = "boolCollection";
    std::string id="potato";
/*
    std::map<std::string, bool> boolCollection;
    boolCollection[id] = false;
    

    GlobalContext<std::map<std::string, bool>>::registerObject(collectionName,boolCollection);
*/
    ///

    hiveEntropyNode.registerResponseHandler(MessageHandler::handleMessage);

    //On vérif que le noeud est bien en vie (why not? Ça sert pas dans le calcul mais bon)
    

    //CalculationId
    
    
    //On prépare une place pour ranger le résultat
    Matrix<float> result(9,9);
    GlobalContext<Matrix<float>>::registerObject(id,result);


    //On envoie un bloc d'exemple au noeud
    cout << "Sending..." << endl;
    //hiveEntropyNode.sendMatrixMultiplicationTask("coap://127.0.0.1:9999",A.getSubmatrix(0,0,2,2),B.getSubmatrix(0,0,2,2),0,0,1,"whatever",id);

    //Alternativement si on veut la méthode RowColumn
    //n.sendMatrixMultiplicationTask("coap://127.0.0.1:9999",Row<float>(9,1,A.getRow(1)),Column<float>(9,0,B.getColumn(0)),id);

 
    //On montre le résultat tel que calculé en local
    cout << "The result from local:" << endl;
    Matrix<float> c = A.getSubmatrix(0,0,2,2)*B.getSubmatrix(0,0,2,2);
    c.show();


    hiveEntropyNode.keepAlive();

    return 0;
}
