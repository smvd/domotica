#include "shell.h"

LOG_MODULE_REGISTER(SHELL, LOG_LEVEL_INF);

const char * SHELL_COMMANDS[SHELL_COMMAND_COUNT] = {
    "NONE",
    "HELP",
    "LED",
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
        SHELL_ToUpper(UART_commandBuffer, UART_commandBufferIndex);

        SHELL_WritePrompt();
        UART_EnableInput();
    }
}

void SHELL_ToUpper(char * string, size_t length) {
    for (size_t i = 0; i < length; i += 1) {
        if (string[i] >= 'a' && string[i] <= 'z') {
            string[i] -= 32;
        }
    }
}
