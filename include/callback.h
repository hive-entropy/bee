//
// Created by alban on 24/04/2021.
//

#ifndef PDS_BEE_CALLBACK_H
#define PDS_BEE_CALLBACK_H

#include <HiveEntropyAPI/full.h>

class Callback {
public:
    static void health(Message message);
    static void hardware(Message message);
    static void requireHelp(Message message);
    static void rowColMultiplication(Message message);
    static void cannonMultiplication(Message message);
};


#endif //PDS_BEE_CALLBACK_H
