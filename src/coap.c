#include "coap.h"

LOG_MODULE_REGISTER(COAP, LOG_LEVEL_INF);

otInstance * COAP_openThread;

uint8_t COAP_Init() {
    COAP_openThread = openthread_get_default_instance();
    if (!COAP_openThread) {
        LOG_ERR("Failed to get openthread instance");
        return 1;
    }

    otCoapSetDefaultHandler(COAP_openThread, COAP_DefaultHandler, NULL);

    if (otCoapStart(COAP_openThread, OT_DEFAULT_COAP_PORT) != OT_ERROR_NONE) {
        LOG_ERR("Failed to start CoAP");
        return 1;
    }

    LOG_INF("CoAP Server ready");

    return 0;
}

void COAP_DefaultHandler(void * context, otMessage * message, const otMessageInfo * messageInfo) {
    LOG_WRN("Received CoAP message that does not match any handler");
}
