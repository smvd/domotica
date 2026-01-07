#ifndef INCLUDED_SHELL
#define INCLUDED_SHELL

#include "uart.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdint.h>

#define SHELL_COMMAND_COUNT 5

uint8_t SHELL_Init();
void SHELL_HandleCommand();
void SHELL_WritePrompt();

#endif
