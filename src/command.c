#include "command.h"

LOG_MODULE_REGISTER(CMD, LOG_LEVEL_INF);

struct CMD_Nodes CMD_nodes[CMD_NODE_COUNT];
uint8_t CMD_nodeIndex = 0;

otCoapResource CMD_rsc = {
    .mUriPath = "command",
    .mHandler = CMD_Handler,
    .mContext = NULL,
    .mNext = NULL
};

void CMD_Init() {
    otCoapAddResource(COAP_openThread, &CMD_rsc);

    if (CMD_SubscribeMulticast(HWID_ToMulticast(HWID_id))) {
        LOG_ERR("Failed to subscribe to HWID_id");
        return 1;
    }

    if (CMD_SubscribeMulticast("ff03::1")) {
        LOG_ERR("Failed to subscribe to ff03::1");
        return 1;
    }

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

void CMD_SendScan() {
    uint8_t commandBuffer[COAP_BUFFER_LIMIT];
    UART_Write("1A\r\n", 3);
    CMD_Serialize(commandBuffer, COMMAND_SCAN_UPDATE, HWID_id, 0);
    UART_Write("1B\r\n", 3);
    COAP_SendRequest("ff03::1", "command", OT_COAP_CODE_PUT, commandBuffer, 17);
    UART_Write("1C\r\n", 3);
}

uint8_t CMD_SubscribeMulticast(const char* multicastAddr) {
    otError error;
    otIp6Address addr;
    error = otIp6AddressFromString(multicastAddr, &addr);
    if (error != OT_ERROR_NONE) {
        LOG_ERR("Failed to parse multicast address: %s", multicastAddr);
        return 1;
    }
    error = otIp6SubscribeMulticastAddress(COAP_openThread, &addr);
    if (error != OT_ERROR_NONE && error != OT_ERROR_ALREADY) {
        LOG_ERR("Failed to subscribe to: %s", multicastAddr);
        return 1;
    }

    LOG_INF("Subscribed to multicast: %s", multicastAddr);
    return 0;
}

uint8_t CMD_PutHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo) {
    struct CMD_Command command = CMD_Deserialize();

    switch (command.command) {
        case COMMAND_SCAN:
            uint8_t commandBuffer[COAP_BUFFER_LIMIT];
            CMD_Serialize(commandBuffer, COMMAND_SCAN_UPDATE, HWID_id, GPIO_targetHWID);
            COAP_SendRequest(HWID_ToMulticast(command.hwidA), "command", OT_COAP_CODE_PUT, commandBuffer, 17);
            break;
        case COMMAND_SCAN_UPDATE:
            CMD_nodes[CMD_nodeIndex].sourceHwid = command.hwidA;
            CMD_nodes[CMD_nodeIndex].targetHwid = command.hwidB;
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
