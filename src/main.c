#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(CORE, LOG_LEVEL_INF);

void main(void) {
	LOG_INF("Starting main");

	while (1) {
		k_msleep(500);   // delay 500 ms
	}
}
