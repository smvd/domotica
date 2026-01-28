#include "shell.h"

LOG_MODULE_REGISTER(SHELL, LOG_LEVEL_INF);

/*
    SCAN - Multicasts please report your hwid and who you are reporting to if you exist
         - Return all HWID
*/

const char * SHELL_COMMANDS[SHELL_COMMAND_COUNT] = {
    "NONE",
    "ACTION",
    "LINK",
    "SCAN",
    "HWID"
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
            UART_Write("NAN\n", 4);
        } else if (commandID == 1) {
            UART_Write("ACT\n", 4);
        } else if (commandID == 2) {
            UART_Write("LNK\n", 4);
        } else if (commandID == 3) {
            UART_Write("1\r\n", 3);
            CMD_SendScan();
            UART_Write("2\r\n", 3);

            k_msleep(1000);
            UART_Write("3\r\n", 3);

            LOG_INF("Node: %lld %lld", CMD_nodes[0].sourceHwid, CMD_nodes[0].targetHwid);

            UART_Write("NDS\r\n", 5);
        } else if (commandID == 4) {
            UART_Write("HWID\n", 5);
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
