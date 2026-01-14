#ifndef INCLUDED_SHELL
#define INCLUDED_SHELL

#include "uart.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdint.h>

#define SHELL_COMMAND_COUNT 4

uint8_t SHELL_Init();
void SHELL_HandleCommand();
uint8_t SHELL_Match(char * a, char * b);


#endif
