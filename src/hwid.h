#ifndef INCLUDED_HWID
#define INCLUDED_HWID


	#include <zephyr/drivers/hwinfo.h>
	#include <zephyr/sys/printk.h>

	uint64_t get_unique_id();

#endif