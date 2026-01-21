#ifndef INCLUDE_CMD
#define INCLUDE_CMD

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdint.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/openthread.h>
#include <openthread/coap.h>

#include "hwid.h"
#include "coap.h"
#include "gpio.h"

#define COMMAND_SCAN        0x00
#define COMMAND_SCAN_RETURN 0x01
#define COMMAND_UPDATE      0x02
#define COMMAND_IDENTIFY    0x03

struct CMD_Command {
    uint8_t command;
    uint64_t hwidA;
    uint64_t hwidB;
};

void CMD_Init();
void CMD_Handler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);
void CMD_Serialize(uint8_t * commandBuffer, uint8_t command, uint64_t hwidA, uint64_t hwidB);
struct CMD_Command CMD_Deserialize();
uint8_t CMD_PutHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);
uint8_t CMD_GetHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);

#endif
