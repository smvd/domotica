#ifndef INCLUDE_GPIO
#define INCLUDE_GPIO

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>

uint8_t GPIO_Init();
void GPIO_ButtonCallback(const struct device * dev, struct gpio_callback * cb, uint32_t pins);

#endif
