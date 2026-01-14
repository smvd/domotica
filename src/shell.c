#include "shell.h"

LOG_MODULE_REGISTER(SHELL, LOG_LEVEL_INF);

const char * SHELL_COMMANDS[SHELL_COMMAND_COUNT] = {
    "NONE",
    "ACTION",
    "LINK",
    "NODES"
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
            UART_Write("NDS\n", 4);
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
