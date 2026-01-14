#ifndef INCLUDED_COAP
#define INCLUDED_COAP

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdint.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/openthread.h>
#include <openthread/coap.h>

#define COAP_BUFFER_LIMIT 256

extern otInstance * COAP_openThread;
extern uint8_t COAP_buffer[COAP_BUFFER_LIMIT];
extern size_t COAP_bufferLength;

typedef uint8_t (*COAP_Function_t)(void *ctx, otMessage *message, const otMessageInfo *messageInfo);

uint8_t COAP_Init();
void COAP_DefaultHandler(void * context, otMessage * message, const otMessageInfo * messageInfo);
uint8_t COAP_SendRequest(const char * addr, const char * uri, otCoapCode code, uint8_t * buffer, size_t bufferLength);
uint8_t COAP_RequestHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo, COAP_Function_t putFunction, COAP_Function_t getFunction);

#endif
