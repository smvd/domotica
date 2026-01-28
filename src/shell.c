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
            UART_Write("INVALID COMMAND\r\n", 17);
        } else if (commandID == 1) {
            UART_Write("ACT\r\n", 5);
        } else if (commandID == 2) {
            UART_Write("LNK\r\n", 5);
        } else if (commandID == 3) {
            UART_Write("Scanning for nodes\r\n", 20);
            CMD_SendScan();
            k_msleep(1000);
            char * buffer[18];
            for (uint8_t i = 0; i < CMD_nodeIndex; i += 1) {
                HWID_ToString(HWID_id, buffer);
                buffer[16] = '\r';
                buffer[17] = '\n';
                UART_Write(buffer, 18);
            }
            UART_Write("NDS\r\n", 5);
        } else if (commandID == 4) {
            UART_Write("HWID\r\n", 6);
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
