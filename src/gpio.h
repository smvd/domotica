#ifndef INCLUDE_GPIO
#define INCLUDE_GPIO

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/openthread.h>
#include <openthread/coap.h>

#include "hwid.h"
#include "coap.h"

#define GPIO_STATUS_LED_COUNT 3

extern uint64_t GPIO_targetHWID;

enum _GPIO_STATE {
    STATE_UNKNOWN,
    STATE_NODE,
    STATE_ERROR,
    STATE_IDENTIFY
};
extern enum _GPIO_STATE GPIO_state;

uint8_t GPIO_Init();
void GPIO_ButtonCallback(const struct device * dev, struct gpio_callback * cb, uint32_t pins);
void GPIO_Handler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);
uint8_t GPIO_PutHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);
uint8_t GPIO_GetHandler(void * ctx, otMessage * message, const otMessageInfo * messageInfo);
void GPIO_SetStatus(enum _GPIO_STATE state);
void GPIO_SetStatusDefault();

#endif
