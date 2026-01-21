#include "scan.h"

LOG_MODULE_REGISTER(CMD, LOG_LEVEL_INF);

otCoapResource CMD_rsc = {
    .mUriPath = "command",
    .mHandler = CMD_Handler,
    .mContext = NULL,
    .mNext = NULL
};

void CMD_Init() {
    otCoapAddResource(COAP_openThread, &CMD_rsc);

    LOG_INF("Commands registed and ready");
}

void CMD_Handler(void * ctx, otMessage * message, const otMessageInfo * messageInfo) {
    COAP_RequestHandler(ctx, message, messageInfo, CMD_PutHandler, CMD_GetHandler);
}

// scan, hwid
// update, hwid
// scanReturn, hwidSrc, hwidTarget
// identify, on/off

// CMD   HWID_A                                   HWID_B
// 0x00  0x11 0x11 0x11 0x11 0x11 0x11 0x11 0x11  0x22 0x22 0x22 0x22 0x22 0x22 0x22 0x22

void CMD_Serialize(uint8_t * commandBuffer, uint8_t command, uint64_t hwidA, uint64_t hwidB) {
    commandBuffer[0] = command;
    memcpy(commandBuffer + 1, hwidA, 8);
    memcpy(commandBuffer + 9, hwidB, 8);
}

struct CMD_Command CMD_Deserialize() {
    struct CMD_Command command;
    command.command = COAP_buffer[0];
    memcpy(command.hwidA, COAP_buffer + 1, 8);
    memcpy(command.hwidB, COAP_buffer + 9, 8);

    return command;
}

uint8_t CMD_PutHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo) {
    struct CMD_Command command = CMD_Deserialize();

    switch (command.command) {
        case COMMAND_SCAN:
            uint8_t commandBuffer[COAP_BUFFER_LIMIT];
            CMD_Serialize(commandBuffer, COMMAND_SCAN_UPDATE, HWID_id, GPIO_targetHWID);
            COAP_SendRequest(HWID_ToMulticast(command.hwidA), "command", OT_COAP_CODE_PUT, NULL, 0);
            break;
        case COMMAND_SCAN_UPDATE:
            break;
        case COMMAND_UPDATE:
            GPIO_targetHWID = command.hwidA;
            break;
        case COMMAND_IDENTIFY:
            if (command.hwidA == 0xFFFFFFFF) {
                GPIO_state = STATE_IDENTIFY;
            } else {
                GPIO_SetStatusDefault();
            }
            break;
    }
    
    return 0;
}

uint8_t CMD_GetHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo) {
    return 0;
}
