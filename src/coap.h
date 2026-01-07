#ifndef INCLUDED_COAP
#define INCLUDED_COAP

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdint.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/openthread.h>
#include <openthread/coap.h>

extern otInstance * COAP_openThread;

uint8_t COAP_Init();
void COAP_DefaultHandler(void * context, otMessage * message, const otMessageInfo * messageInfo);

#endif
