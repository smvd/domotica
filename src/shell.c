#include "shell.h"

LOG_MODULE_REGISTER(SHELL, LOG_LEVEL_INF);

const char * SHELL_COMMANDS[SHELL_COMMAND_COUNT] = {
    "NONE",
    "HELP",
    "WHOAMI",
    "NONE",
    "NONE"
};

uint8_t SHELL_Init() {
    if (UART_Init()) {
        return 1;
    }

    return 0;
}

void SHELL_WritePrompt() {
    UART_Write("> ", 2);
}

void SHELL_HandleCommand() {
    if (UART_commandReady == 1) {
        LOG_INF("Command: |%s|", UART_commandBuffer);

        SHELL_WritePrompt();
        UART_EnableInput();
    }
}
