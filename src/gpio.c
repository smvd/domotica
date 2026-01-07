#include "gpio.h"

LOG_MODULE_REGISTER(GPIO, LOG_LEVEL_INF);
static const struct gpio_dt_spec GPIO_LED = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec GPIO_BUTTON = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static struct gpio_callback GPIO_buttonCallbackData;

uint8_t GPIO_Init() {
	if (!gpio_is_ready_dt(&GPIO_LED)) {
        LOG_ERR("led device not ready");
		return 1;
	}
	
	if (gpio_pin_configure_dt(&GPIO_LED, GPIO_OUTPUT_HIGH) < 0) {
        LOG_ERR("led configuration failed");
		return 1;
	}
	

	if (!gpio_is_ready_dt(&GPIO_BUTTON)) {
        LOG_ERR("button device not ready");
        return 1;
	}

	if (gpio_pin_configure_dt(&GPIO_BUTTON, GPIO_INPUT) != 0) {
        LOG_ERR("failed to configure button");
        return 1;
	}

	if (gpio_pin_interrupt_configure_dt(&GPIO_BUTTON, GPIO_INT_EDGE_TO_ACTIVE) != 0) {
        LOG_ERR("failed to configure button interupt");
        return 1;
	}

	gpio_init_callback(&GPIO_buttonCallbackData, GPIO_ButtonCallback, BIT(GPIO_BUTTON.pin));
	gpio_add_callback_dt(&GPIO_BUTTON, &GPIO_buttonCallbackData);

	LOG_INF("GPIO ready");

    return 0;
}

void GPIO_ButtonCallback(const struct device * dev, struct gpio_callback * cb, uint32_t pins) {
    gpio_pin_toggle_dt(&GPIO_LED);
}
