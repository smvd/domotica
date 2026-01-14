#include "coap.h"

LOG_MODULE_REGISTER(COAP, LOG_LEVEL_INF);

otInstance * COAP_openThread;
uint8_t COAP_buffer[COAP_BUFFER_LIMIT];
size_t COAP_bufferLength;

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

uint8_t COAP_SendRequest(const char * addr, const char * uri, otCoapCode code, uint8_t * buffer, size_t bufferLength) {
	otMessageInfo messageInfo;
    memset(&messageInfo, 0, sizeof(messageInfo));
	otIp6AddressFromString(addr, &messageInfo.mPeerAddr);
	messageInfo.mPeerPort = OT_DEFAULT_COAP_PORT;

    otMessage * message;
    message = otCoapNewMessage(COAP_openThread, NULL);
    if (!message) {
        LOG_ERR("Failed to create CoAP message");
        otMessageFree(message);
        return 1;
    }

	otCoapMessageInit(message, OT_COAP_TYPE_NON_CONFIRMABLE, code);

    otError err = otCoapMessageAppendUriPathOptions(message, uri);
	if (err != OT_ERROR_NONE) {
		LOG_ERR("Failed to append URI to message");
		otMessageFree(message);
        return 1;
	}

    err = otCoapMessageSetPayloadMarker(message);
	if (err != OT_ERROR_NONE) {
		LOG_ERR("Failed to set payload marker for message");
		otMessageFree(message);
        return 1;
	}

	err = otMessageAppend(message, buffer, bufferLength);
	if (err != OT_ERROR_NONE) {
		LOG_ERR("Failed to append buffer to message");
		otMessageFree(message);
		return 1;
	}

	err = otCoapSendRequest(COAP_openThread, message, &messageInfo, NULL, NULL);
	if (err != OT_ERROR_NONE) {
		LOG_ERR("Failed to send request");
		otMessageFree(message);
		return 1;
	}

	otMessageFree(message);
    return 0;
}

uint8_t COAP_RequestHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo, COAP_Function_t putFunction, COAP_Function_t getFunction) {
    otCoapCode messageCode = otCoapMessageGetCode(message);
    otCoapType messageType = otCoapMessageGetType(message);

    if (messageType != OT_COAP_TYPE_NON_CONFIRMABLE) {
        return 1;
    }

    if (messageCode == OT_COAP_CODE_PUT && putFunction != NULL) {
		COAP_bufferLength = otMessageGetLength(message) - otMessageGetOffset(message);
		otMessageRead(message, otMessageGetOffset(message), COAP_buffer, COAP_bufferLength);
		
        return putFunction(ctx, message, messageInfo);
    } else if (messageCode == OT_COAP_CODE_GET && getFunction != NULL) {
        return getFunction(ctx, message, messageInfo);
    }

    return 1;
}
