#ifndef INCLUDED_UART
#define INCLUDED_UART

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/logging/log.h>

#include "usb.h"
#include <stdint.h>

#define UART_BUFFER_SIZE 64

extern uint8_t UART_commandReady;
extern struct ring_buf UART_buffer;
extern char UART_commandBuffer[UART_BUFFER_SIZE];
extern size_t UART_commandBufferIndex;

uint8_t UART_Init();
void UART_SampleCallback(struct usbd_context * const ctx, const struct usbd_msg * msg);
void UART_InteruptHandler(const struct device * dev, void * user_data);
uint8_t UART_EnableUSB();
void UART_EnableInput();
void UART_DisableInput();
void UART_Write(char * string, size_t length);

#endif
