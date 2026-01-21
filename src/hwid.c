#include "hwid.h"

LOG_MODULE_REGISTER(HWID, LOG_LEVEL_INF);

uint64_t HWID_id = 0;

uint8_t HWID_Init() {
	size_t ret = hwinfo_get_device_id((uint8_t*)&HWID_id, sizeof(HWID_id));

	if (ret <= 0) {
	    LOG_ERR("Failed to find hardware ID");
		return 1;
	}

	LOG_INF("HWID: %#016llx", HWID_id);

	return 0;
}

uint8_t HWID_MatchRouter() {
    return HWID_id == 0x8383564009964fec;
}

void HWID_ToString(char * buffer) {
    const char hex_chars[] = "0123456789abcdef";

    for (int i = 15; i >= 0; i--) {
        buffer[i] = hex_chars[HWID_id & 0xF];
        HWID_id >>= 4;
    }

    buffer[16] = '\0';
}
