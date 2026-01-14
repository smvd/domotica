#ifndef INCLUDE_GPIO
#define INCLUDE_GPIO

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/openthread.h>
#include <openthread/coap.h>

#include "coap.h"

uint8_t GPIO_Init();
void GPIO_ButtonCallback(const struct device * dev, struct gpio_callback * cb, uint32_t pins);
void GPIO_Handler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);
uint8_t GPIO_PutHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);
uint8_t GPIO_GetHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);

#endif
