#include "hwid.h"

uint64_t get_unique_id() {
	uint64_t id = 0;
	ssize_t ret = hwinfo_get_device_id((uint8_t*)&id, sizeof(id));

	if (ret <= 0) {
		return 0;
	}
	return id;
}