#include "main.h"

LOG_MODULE_REGISTER(CORE, LOG_LEVEL_INF);

/*
    
*/

/*
D3:6F:C5:73:8E:C3
    0x8383564009964fec
FC:2F:2F:41:D9:65
    0xd331cd54d7dd0f9e
E3:AF:F6:DD:06:3F
    0x51b317d464fcdb96
E1:A6:E9:B8:C7:EA
    0xde02c618e7a52ab5
*/

int main() {
	LOG_INF("Starting main");

    SHELL_Init();
    HWID_Init();
    COAP_Init();
    GPIO_Init();

    while (1) {
        SHELL_HandleCommand();
        k_msleep(5);
    }
}
