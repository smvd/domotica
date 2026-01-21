#ifndef INCLUDED_HWID
#define INCLUDED_HWID

	#include <zephyr/drivers/hwinfo.h>
	#include <zephyr/sys/printk.h>
	#include <zephyr/logging/log.h>
	#include <zephyr/kernel.h>

	#include "uart.h"

    extern uint64_t HWID_id;

	uint8_t HWID_Init();
	uint8_t HWID_MatchRouter();
	void HWID_ToString(char * buffer);

#endif
