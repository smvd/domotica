#include "hwid.h"

LOG_MODULE_REGISTER(HWID, LOG_LEVEL_INF);

uint64_t HWID_id = 0;

uint8_t HWID_Init() {
	size_t ret = hwinfo_get_device_id((uint8_t*)&HWID_id, sizeof(HWID_id));

	if (ret <= 0) {
	    LOG_ERR("Failed to find hardware ID");
		return 1;
	}

	LOG_INF("HWID: %016llx", HWID_id);

	return 0;
}

void HWID_ToString(uint64_t id, char * buffer) {
    const char hex_chars[] = "0123456789abcdef";

    for (int i = 15; i >= 0; i--) {
        buffer[i] = hex_chars[id & 0xF];
        id >>= 4;
    }
}

uint64_t HWID_FromString(char * buffer) {
    uint64_t value = 0;

    for (int i = 0; i < 16; i++) {
        char c = tolower(buffer[i]);
        uint64_t nibble =
            (c >= '0' && c <= '9') ? (c - '0') :
            (c >= 'a' && c <= 'f') ? (c - 'a' + 0xA) : 0;

        value = (value << 4) | nibble;
    }

    return value;
}

const char* HWID_ToMulticast(uint64_t hwid)
{
    static char buf[40];

    snprintf(buf, sizeof(buf),
        "ff03::1:%04x:%04x:%04x:%04x",
        (uint16_t)(hwid >> 48),
        (uint16_t)(hwid >> 32),
        (uint16_t)(hwid >> 16),
        (uint16_t)(hwid));

    return buf;
}
