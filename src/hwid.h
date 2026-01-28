#ifndef INCLUDED_HWID
#define INCLUDED_HWID

	#include <zephyr/drivers/hwinfo.h>
	#include <zephyr/sys/printk.h>
	#include <zephyr/logging/log.h>
	#include <zephyr/kernel.h>
	#include <stdio.h>

	#include "uart.h"

    extern uint64_t HWID_id;

	uint8_t HWID_Init();
	uint8_t HWID_MatchRouter();
	void HWID_ToString(uint64_t id, char * buffer);
	uint64_t HWID_FromString(char * buffer);
	const char * HWID_ToMulticast(uint64_t hwid);
#endif
