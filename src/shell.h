#ifndef INCLUDED_SHELL
#define INCLUDED_SHELL

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/logging/log.h>

#include "usb.h"
#include <stdint.h>

#define SHELL_UART_BUFFER_SIZE 64
#define SHELL_UART_RING_BUFFER_SIZE 1024

uint8_t SHELL_Init();
void SHELL_SampleCallback(struct usbd_context * const ctx, const struct usbd_msg * msg);
void SHELL_UartInteruptHandler(const struct device * dev, void * user_data);
uint8_t SHELL_EnableUSB();

#endif
