#include "shell.h"

LOG_MODULE_REGISTER(SHELL, LOG_LEVEL_INF);

/*
    SCAN - Multicasts please report your hwid and who you are reporting to if you exist
         - Return all HWID
*/

const char * SHELL_COMMANDS[SHELL_COMMAND_COUNT] = {
    "NONE",
    "UPDATE",
    "SCAN",
    "IDENTIFY"
};

uint8_t SHELL_Init() {
    if (UART_Init()) {
        return 1;
    }

    return 0;
}

void SHELL_HandleCommand() {
    if (UART_commandReady == 1) {
        uint8_t commandID = 0;
        for (uint8_t i = 0; i < SHELL_COMMAND_COUNT; i += 1) {
            if (SHELL_Match(SHELL_COMMANDS[i], UART_commandBuffer)) {
                commandID = i;
                break;
            }
        }

        if (commandID == 0) {
            UART_Write("INVALID COMMAND\r\n", 17);
        } else if (commandID == 1) {
            uint64_t deviceToUpdate = HWID_FromString(UART_commandBuffer + 7);
            uint64_t newTarget = HWID_FromString(UART_commandBuffer + 24);
            CMD_SendUpdate(deviceToUpdate, newTarget);
        } else if (commandID == 2) {
            CMD_SendScan();
            k_msleep(1000);
            char * buffer[16];
            for (uint8_t i = 0; i < CMD_nodeIndex; i += 1) {
                HWID_ToString(CMD_nodes[i].sourceHwid, buffer);
                UART_Write(buffer, 16);
                UART_Write(" ", 1);
                HWID_ToString(CMD_nodes[i].targetHwid, buffer);
                UART_Write(buffer, 16);
                UART_Write("\r\n", 2);
            }
        } else if (commandID == 3) {
            uint64_t hwid = HWID_FromString(UART_commandBuffer + 9);
            CMD_SendIdentify(hwid, *(UART_commandBuffer + 26) - '0');
        }

        UART_EnableInput();
    }
}

uint8_t SHELL_Match(char * a, char * b) {
    while (*a != '\0') {
        if (*a++ != *b++) {return 0;}
    }

    return 1;
}
