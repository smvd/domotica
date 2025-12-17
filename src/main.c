#include "shell.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(CORE, LOG_LEVEL_INF);

void main() {
	LOG_INF("Starting main");

    SHELL_Init();

    while (1) {
        k_msleep(500);
    }
}
