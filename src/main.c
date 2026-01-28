#include "main.h"

LOG_MODULE_REGISTER(CORE, LOG_LEVEL_INF);

void main() {
	LOG_INF("Starting main");

    uint8_t errors = 0;

    errors += SHELL_Init();
    errors += HWID_Init();
    errors += COAP_Init();
    errors += GPIO_Init();
    errors += CMD_Init();

    if (errors != 0) {
        GPIO_SetStatus(STATE_ERROR);
    }

    while (1) {
        SHELL_HandleCommand();
        k_msleep(5);
    }
}
