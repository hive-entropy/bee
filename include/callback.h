//
// Created by alban on 24/04/2021.
//

#ifndef PDS_BEE_CALLBACK_H
#define PDS_BEE_CALLBACK_H

#include <HiveEntropyAPI/full.h>

class Callback {
public:
    static Message health(Message message);
    static Message hardware(Message message);
    static Message requireHelp(Message message);
    static Message rowColMultiplication(Message message);
    static Message cannonMultiplication(Message message);

    //WEBSERVICE CALLBACKS
    static Message identity(Message message);
    static Message stop(Message message);
    static Message pause(Message message);
    static Message resume(Message message);
    static Message logs(Message message);
};


#endif //PDS_BEE_CALLBACK_H
