//
// Created by alban on 24/04/2021.
//

#ifndef PDS_BEE_CALLBACK_H
#define PDS_BEE_CALLBACK_H

#include <HiveEntropyAPI/full.h>

class Callback {
public:
    static void health(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                       coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                       coap_pdu_t *response);
    static void hardware(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                       coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                       coap_pdu_t *response);
    static void requireHelp(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                            coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                            coap_pdu_t *response);
    static void rowColMultiplication(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                                     coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                                     coap_pdu_t *response);
    static void cannonMultiplication(coap_context_t *context, coap_resource_t *resource, coap_session_t *session,
                                     coap_pdu_t *request, coap_binary_t *token, coap_string_t *query,
                                     coap_pdu_t *response);
};


#endif //PDS_BEE_CALLBACK_H
